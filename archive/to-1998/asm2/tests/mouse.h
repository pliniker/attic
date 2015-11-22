// Mouse Handler
//This is a library of all the mouse handling routines

#include <dos.h>

void mouse_init(void);
void mouse_cursor_off(void);
void mouse_cursor_on(void);
int mouse_press (int *row, int *col, int *button);
int mouse_release (int *row,int *col,int *button);
void mouse_reset (int *mouse_present,int *buttons);
void mouse_set_graphics_cursor (int col_hot_spot,int row_hot_spot,
								unsigned mask_seg,unsigned mask_ofs);
void mouse_set_position (int *row,int *col);

void mouse_cursor_off(void)
//      This procedure hides the mouse pointer.

{
	asm {
	mov ax,2;
	int 0x33;
	}
}

void mouse_cursor_on(void)
//      This procedure shows the mouse pointer

{
	 _AX = 1;
	 geninterrupt(51);
}

int mouse_press (int *row,int *col,int *button)
//      This function tests whether the button passed as a parameter has been pressed.  If it   has then the function returns a 1 (true) and the co-ordinates of the button press.
//	INPUT:
//		button  :-This parameter defines which button the function will test to see if it                       has been pressed.
 //	OUTPUT:
 //		row     :-This variable holds the Y co-ords of the pointer when the button is                           pressed.
 //		col     :-This variable holds the X co-ords of the pointer when the button is                           pressed.
{
	 _AX=5;
	 _BX=*button;
	 geninterrupt(51);
	 *button=_BX;
	 *col=_CX;
	 *row=_DX;
	 return _AX;
}

void mouse_set_graphics_cursor (int col_hot_spot,int row_hot_spot,
								unsigned mask_seg, unsigned mask_ofs)
//      This procedure uses a predefined data block to set the shape of the mouse cursor                and sets the position of the pick point within the outline of the mouse cursor.
//	INPUT:
//		col_hot_spot    :-The X co-ords of the pick point within the cursor shape.
//		row_hot_spot    :-The Y co-ords of the pick point within the cursor shape.
//		mask_seg        :-The data segment the data for the cursor shape is held in.
//		mask_ofs        :-The data offset of the data block for the cursor shape.

{
	 _ES=mask_seg;
	 _AX=9;
	 _BX=col_hot_spot;
	 _CX=row_hot_spot;
	 _DX=mask_ofs;
	 geninterrupt(51);
}

void mouse_set_position (int *row, int *col)
//      This procedure places the mouse cursor at the co-ordinates given by the                         parameters.
//	INPUT:
//		row     :-The Y position the cursor should be placed at.
//		col     :-The X position the cursor should be placed at.

{
	 _AX=4;
	 _CX=*col;
	 _DX=*row;
	 geninterrupt(51);
}

void mouse_init(void)
//      This procedure defines a 32 byte array which holds the bitmap for the shape of the              cursor.  It then turns graphics on, calls mouse_set_graphics_cursor with the address    in memory of this 32 byte array and turns the cursor on.
{
	unsigned mask_seg,mask_off;
	unsigned mask[32];
	unsigned *maskptr;
	struct SREGS segs;

	mask[0]=0x3fff;
	mask[1]=0x1fff;
	mask[2]=0x0fff;
	mask[3]=0x07ff;
	mask[4]=0x03ff;
	mask[5]=0x01ff;
	mask[6]=0x00ff;
	mask[7]=0x007f;
	mask[8]=0x003f;
	mask[9]=0x001f;
	mask[10]=0x01fe;
	mask[11]=0x10ff;
	mask[12]=0x30ff;
	mask[13]=0xf87f;
	mask[14]=0xf87f;
	mask[15]=0xfc7f;
	mask[16]=0x0000;
	mask[17]=0x4000;
	mask[18]=0x6000;
	mask[19]=0x7000;
	mask[20]=0x7800;
	mask[21]=0x7c00;
	mask[22]=0x7e00;
	mask[23]=0x7f00;
	mask[24]=0x7f80;
	mask[25]=0x7c00;
	mask[26]=0x6c00;
	mask[27]=0x4600;
	mask[28]=0x0600;
	mask[29]=0x0300;
	mask[30]=0x0300;
	mask[31]=0x0000;

	mask_seg = FP_SEG(mask);
	mask_off = FP_OFF(mask);
	mouse_set_graphics_cursor(1,1,mask_seg,mask_off);

}
