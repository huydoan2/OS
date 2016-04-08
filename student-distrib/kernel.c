/* kernel.c - the C part of the kernel
 * vim:ts=4 noexpandtab
 */

#include "multiboot.h"
#include "types.h"
#include "x86_desc.h"
#include "lib.h"
#include "i8259.h"
#include "debug.h"
#include "keyboard.h"
#include "IDT.h"
#include "rtc.h"
#include "Paging.h"
#include "systemCalls.h"

/* Macros. */
/* Check if the bit BIT in FLAGS is set. */
#define CHECK_FLAG(flags,bit)   ((flags) & (1 << (bit)))

#define size_of_keys 128
#define rtc_num_byte 4
#define rtc_buff_size 5
#define file_name_max_size 32
#define test_virt_1 0x00800000
#define test_virt 0x00400000


#define test_phys 0x0000FFFF
#define file_buff_size 6200

/* Check if MAGIC is valid and print the Multiboot information structure
   pointed by ADDR. */
void
entry (unsigned long magic, unsigned long addr)
{
	multiboot_info_t *mbi;
	
	/*file system test variables*/

	uint32_t fileSys_startAddr;
	uint8_t buffer_0[file_buff_size] = {0};
	uint8_t buffer_1[file_buff_size] = {0};

	uint32_t offset;

	/*paging test variables*/
	uint32_t test_phys_addr;
	uint32_t test_virt_addr = test_virt;
	uint32_t test_val;
	
	/*RTC test variables*/
	int rtc_buff[rtc_buff_size] = {2,4,8,16,32};
	int rtc_index = 0;
	int rtc_read_buf[rtc_buff_size] = {2,4,8,16,32};

	/*keyboard and terminal test variables*/
	int keyboard_write_index;
	char buff[size_of_keys];
	int num_byte = size_of_keys;
	int32_t * buf;
	int32_t num_bytes;

   /*file descriptor variables*/
	int32_t fd_rtc;
	int32_t fd_file;
	int32_t fd_dir;

	int32_t file_size;

	/* Clear the screen. */
	clear();

	/* Am I booted by a Multiboot-compliant boot loader? */
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
	{
		printf ("Invalid magic number: 0x%#x\n", (unsigned) magic);
		return;
	}

	/* Set MBI to the address of the Multiboot information structure. */
	mbi = (multiboot_info_t *) addr;

	/* Print out the flags. */
	printf ("flags = 0x%#x\n", (unsigned) mbi->flags);

	/* Are mem_* valid? */
	if (CHECK_FLAG (mbi->flags, 0))
		printf ("mem_lower = %uKB, mem_upper = %uKB\n",
				(unsigned) mbi->mem_lower, (unsigned) mbi->mem_upper);

	/* Is boot_device valid? */
	if (CHECK_FLAG (mbi->flags, 1))
		printf ("boot_device = 0x%#x\n", (unsigned) mbi->boot_device);

	/* Is the command line passed? */
	if (CHECK_FLAG (mbi->flags, 2))
		printf ("cmdline = %s\n", (char *) mbi->cmdline);

	if (CHECK_FLAG (mbi->flags, 3)) {
		int mod_count = 0;
		int i;
		module_t* mod = (module_t*)mbi->mods_addr;
		fileSys_startAddr = mod->mod_start;
		while(mod_count < mbi->mods_count) {
			printf("Module %d loaded at address: 0x%#x\n", mod_count, (unsigned int)mod->mod_start);
			printf("Module %d ends at address: 0x%#x\n", mod_count, (unsigned int)mod->mod_end);
			printf("First few bytes of module:\n");
			for(i = 0; i<16; i++) {
				printf("0x%x ", *((char*)(mod->mod_start+i)));
			}
			printf("\n");
			mod_count++;
			mod++;
		}
	}
	/* Bits 4 and 5 are mutually exclusive! */
	if (CHECK_FLAG (mbi->flags, 4) && CHECK_FLAG (mbi->flags, 5))
	{
		printf ("Both bits 4 and 5 are set.\n");
		return;
	}

	/* Is the section header table of ELF valid? */
	if (CHECK_FLAG (mbi->flags, 5))
	{
		elf_section_header_table_t *elf_sec = &(mbi->elf_sec);

		printf ("elf_sec: num = %u, size = 0x%#x,"
				" addr = 0x%#x, shndx = 0x%#x\n",
				(unsigned) elf_sec->num, (unsigned) elf_sec->size,
				(unsigned) elf_sec->addr, (unsigned) elf_sec->shndx);
	}

	/* Are mmap_* valid? */
	if (CHECK_FLAG (mbi->flags, 6))
	{
		memory_map_t *mmap;

		printf ("mmap_addr = 0x%#x, mmap_length = 0x%x\n",
				(unsigned) mbi->mmap_addr, (unsigned) mbi->mmap_length);
		for (mmap = (memory_map_t *) mbi->mmap_addr;
				(unsigned long) mmap < mbi->mmap_addr + mbi->mmap_length;
				mmap = (memory_map_t *) ((unsigned long) mmap
					+ mmap->size + sizeof (mmap->size)))
			printf (" size = 0x%x,     base_addr = 0x%#x%#x\n"
					"     type = 0x%x,  length    = 0x%#x%#x\n",
					(unsigned) mmap->size,
					(unsigned) mmap->base_addr_high,
					(unsigned) mmap->base_addr_low,
					(unsigned) mmap->type,
					(unsigned) mmap->length_high,
					(unsigned) mmap->length_low);
	}

	/* Construct an LDT entry in the GDT */
	{
		seg_desc_t the_ldt_desc;
		the_ldt_desc.granularity    = 0;
		the_ldt_desc.opsize         = 1;
		the_ldt_desc.reserved       = 0;
		the_ldt_desc.avail          = 0;
		the_ldt_desc.present        = 1;
		the_ldt_desc.dpl            = 0x0;
		the_ldt_desc.sys            = 0;
		the_ldt_desc.type           = 0x2;

		SET_LDT_PARAMS(the_ldt_desc, &ldt, ldt_size);
		ldt_desc_ptr = the_ldt_desc;
		lldt(KERNEL_LDT);
	}

	/* Construct a TSS entry in the GDT */
	{
		seg_desc_t the_tss_desc;
		the_tss_desc.granularity    = 0;
		the_tss_desc.opsize         = 0;
		the_tss_desc.reserved       = 0;
		the_tss_desc.avail          = 0;
		the_tss_desc.seg_lim_19_16  = TSS_SIZE & 0x000F0000;
		the_tss_desc.present        = 1;
		the_tss_desc.dpl            = 0x0;
		the_tss_desc.sys            = 0;
		the_tss_desc.type           = 0x9;
		the_tss_desc.seg_lim_15_00  = TSS_SIZE & 0x0000FFFF;

		SET_TSS_PARAMS(the_tss_desc, &tss, tss_size);

		tss_desc_ptr = the_tss_desc;

		tss.ldt_segment_selector = KERNEL_LDT;
		tss.ss0 = KERNEL_DS;
		tss.esp0 = 0x800000;
		ltr(KERNEL_TSS);
	}

	/* Init the IDT */
	fill_interrupt_descriptor_table();

	/* Init the PIC */
	i8259_init();
	rtc_init();
	/* Initialize devices, memory, filesystem, enable device interrupts on the
	 * PIC, any other initialization stuff... */
	keyboard_open();
	//rtc_open(buf, num_bytes);
	paging_init();

	// /* test the memory accessing by paging */
	// test_phys_addr = get_physAddr(0x00400000+8);
	// test_val = *((uint32_t *)test_phys_addr);
	// printf("memory: %x\n", test_val);


	// printf("reach here ! \n");
	// uint32_t virtual_addr = 0x08000000;
	// map_page(0);
	// test_phys_addr = get_physAddr(0x08000000);

	// printf("memory: %x\n", test_phys_addr);

	// test_val = *((uint32_t *)virtual_addr);



	/*Set up IDT to handle system calls*/


	/* Enable interrupts */
	/* Do not enable the following until after you have set up your
	 * IDT correctly otherwise QEMU will triple fault and simple close
	 * without showing you any output */
	printf("Enabling Interrupts\n");

	//clear(); //need to clear screen in terminal driver init
	parsing_fileSystem(fileSys_startAddr); //initialize file system
	// init_FD();


	/*open rtc*/
	// fd_rtc = open((uint8_t*)"rtc");
	sti();

	/* Execute the first program (`shell') ... */
	
	/********TESTING FILE SYSTEM*******/
	/*open a directory file*/
	// fd_dir = open((uint8_t*)".");
	// while(read(fd_dir, buffer_0, 4))
	// {
	// 	printf("%s\n",buffer_0);
	// }
	// close(fd_dir);
	// fd_dir = open((uint8_t*)".");
	// while(read(fd_dir, buffer_0, file_name_max_size))
	// {
	// 	printf("%s\n",buffer_0);
	// }
	// close(fd_dir);

	// /*open a regular file and read*/
	//  fd_file = open((uint8_t*)"frame0.txt");
	// if((offset = read(fd_file, buffer_1, 33)) != 0)
	// {
	// 	printf("TEXT READ:\n");
	// 	keyboard_write((int8_t *)buffer_1,offset);
	// 	printf("number of Bytes read: %d \n",offset);
	// }
	// else 
	// {
	// 	printf("The end of the file has been reached!!\n");
	// }
	// if((offset = read(fd_file, buffer_1, file_buff_size)) != 0)
	// {
	// 	printf("TEXT READ:\n");
	// 	keyboard_write((int8_t *)buffer_1,offset);
	// 	printf("number of Bytes read: %d \n",offset);
	// }
	// else 
	// {
	// 	printf("The end of the file has been reached!!\n");
	// }

	// /*obtain the size of a given file*/
	// file_size = get_fileSize((uint8_t*)"frame0.txt");
	// if(file_size != -1)
	// {
	// 	printf("file name: %s\n", "frame0.txt");
	// 	printf("file size: %d Bytes\n", file_size);
	// }
	// virtual_addr = 0x08048000;
	// prog_loader((uint8_t*)"frame0.txt", (uint32_t*)virtual_addr);
	// virtual_addr = 0x08048000;

	
	// write(1, (int32_t*)virtual_addr,187);

	

	
	/********TESTING READ AND WRITE for Terminal and RTC*******/
	while(1)
	{
		//keyboard read write test
		keyboard_write_index = keyboard_read((int32_t*)buff, 0,num_byte);
		keyboard_write((int32_t*)buff, keyboard_write_index);
		printf("bytes written = %d\n",keyboard_write_index);
		while(keyboard_write_index>=0)
		{		
			buff[keyboard_write_index] = 0;
			keyboard_write_index--;
		}
		//rtc read write test
		// write(fd_rtc,rtc_buff+rtc_index, rtc_num_byte);
		// read(fd_rtc, rtc_read_buf, rtc_num_byte);
		// rtc_index++;
		// if(rtc_index == rtc_buff_size)
		// 	rtc_index = 0;
	}
	

	keyboard_close();
	rtc_close(buf, num_bytes);


	/* Spin (nicely, so we don't chew up cycles) */
	asm volatile(".1: hlt; jmp .1;");
}


