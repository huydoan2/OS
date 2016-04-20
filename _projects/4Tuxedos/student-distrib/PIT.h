#ifndef PIT_H
#define PIT_H

#include "lib.h"

#define CHANNEL_0		0x40
#define CMD		  		0x43

#define INIT_VALUE		0x36		//Channel0 - Latch - square wave gen
#define CONSTANT		1193182
		
/*
I/O port     Usage
0x40         Channel 0 data port (read/write)
0x41         Channel 1 data port (read/write)
0x42         Channel 2 data port (read/write)
0x43         Mode/Command register (write only, a read is ignored)
*/

/*The Mode/Command register at I/O address 0x43 contains the following:
Bits         Usage
6 and 7      Select channel :
                 0 0 = Channel 0
                 0 1 = Channel 1
                 1 0 = Channel 2
                 1 1 = Read-back command (8254 only)
 4 and 5      Access mode :
                 0 0 = Latch count value command
                 0 1 = Access mode: lobyte only
                 1 0 = Access mode: hibyte only
                 1 1 = Access mode: lobyte/hibyte
 1 to 3       Operating mode :
                 0 0 0 = Mode 0 (interrupt on terminal count)
                 0 0 1 = Mode 1 (hardware re-triggerable one-shot)
                 0 1 0 = Mode 2 (rate generator)
                 0 1 1 = Mode 3 (square wave generator)
                 1 0 0 = Mode 4 (software triggered strobe)
                 1 0 1 = Mode 5 (hardware triggered strobe)
                 1 1 0 = Mode 2 (rate generator, same as 010b)
                 1 1 1 = Mode 3 (square wave generator, same as 011b)
 0            BCD/Binary mode: 0 = 16-bit binary, 1 = four-digit BCD
*/

void pit_init();

void pit_set_freq(uint32_t freq);

#endif

/*
Argument	Freq (Hz)	Period (ms)
15000	79.54546667	12.57142666
16000	74.573875	13.40952177
17000	70.18717647	14.24761688
18000	66.28788889	15.08571199
19000	62.79905263	15.9238071
<<<<<<< .mine
20000	59.6591	          16.76190221
=======
20000	59.6591	        16.76190221
>>>>>>> .r16188
21000	56.81819048	17.59999732
22000	54.23554545	18.43809243
23000	51.87747826	19.27618754
24000	49.71591667	20.11428265
25000	47.72728	20.95237776
26000	45.89161538	21.79047287
27000	44.19192593	22.62856798
28000	42.61364286	23.46666309
29000	41.1442069	24.3047582
30000	39.77273333	25.14285331
31000	38.48974194	25.98094842
32000	37.2869375	26.81904353
33000	36.1570303	27.65713864
34000	35.09358824	28.49523375
35000	34.09091429	29.33332886
36000	33.14394444	30.17142397
37000	32.24816216	31.00951908
38000	31.39952632	31.84761419
39000	30.59441026	32.68570931
40000	29.82955	33.52380442
41000	29.102	        34.36189953
42000	28.40909524	35.19999464
43000	27.7484186	36.03808975
44000	27.11777273	36.87618486
45000	26.51515556	37.71427997
46000	25.93873913	38.55237508
47000	25.38685106	39.39047019
48000	24.85795833	40.2285653
49000	24.35065306	41.06666041
50000	23.86364	41.90475552
51000	23.39572549	42.74285063
52000	22.94580769	43.58094574
53000	22.51286792	44.41904085
54000	22.09596296	45.25713596
55000	21.69421818	46.09523107
56000	21.30682143	46.93332618
57000	20.93301754	47.77142129
58000	20.57210345	48.6095164
59000	20.22342373	49.44761151
60000	19.88636667	50.28570662

*/
