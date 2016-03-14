/*terminal.c
 * - terminal driver for text user interface
 *
 */

 #include "terminal.h"


void scroll_screen(void){

	/*set the black color*/

	/*determine whether the end of screen is reached*/

		/*move the currnet video display up by a line*/

		/*set blank to the last line of the video*/

}

//DO we even need this ???
void blink_cursor(void){

	/*obtain the current location of the cursor*/

	/*send command to the CRT control register of the VGA controller
	  to create the blinking effect*/


}




extern void init_videoMem(){

	/*set the text-mode video memory pointer*/

	/*clear the screen*/

}


extern void set_text_color(text_t text_attrib){
	/*assign the color*/
	//what is the purpose of this function again??
	text_color = text_attrib;
	return;
}

