     h  h   nPascalObject {};	#endif#endifMultiMonitor.ccrc.catdmcesr 8.5 *TEXTCWIE                     ��ܘ  �  �*****	MacOS versioning structures			VersRec					Contents of a 'vers' resource		VersRecPtr				Pointer to a VersRecPtr		VersRe   // *******************************************************************************************// MultiMonitor.c                                        CARBONIZED.  USES CLASSIC EVENT MODEL// *******************************************************************************************#include "Miscellany.h"// **************************************************************************** deviceLoopDrawvoid  deviceLoopDraw(SInt16 depth,SInt16 deviceFlags,GDHandle targetDeviceHdl,SInt32 userData){	RGBColor	oldForeColour;	WindowRef windowRef;	Rect			portRect;	RGBColor	greenColour  = { 0x0000, 0xAAAA, 0x1111 };	RGBColor	redColour    = { 0xAAAA, 0x4444, 0x3333 };	RGBColor	blueColour   = { 0x5555, 0x4444, 0xFFFF };	RGBColor	ltGrayColour = { 0xDDDD, 0xDDDD, 0xDDDD };	RGBColor	grayColour   = { 0x9999, 0x9999, 0x9999 };	RGBColor	dkGrayColour = { 0x4444, 0x4444, 0x4444 };		GetForeColor(&oldForeColour);	windowRef = (WindowRef) userData;	GetWindowPortBounds(windowRef,&portRect);	EraseRect(&portRect);	   ��щ  TEXTk         H 	Monaco                             Q 
L\ Q 
L\�;��  �  �          h  h   nSORT� �  n MPSR   "MWBB   .TEXT   :pnot   F���      ���      k ��          ��      