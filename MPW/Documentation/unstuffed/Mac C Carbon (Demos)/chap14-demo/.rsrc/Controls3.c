     h  h   n��������������������������������Controls3.cPC.tdmeferencesr 8.5  TEXTCWIE                     �^o!  &  �����������������������������������������������������������������������*/typedef CALLBACK_API( long , WindowDefProcPtr )(short v   // *******************************************************************************************// Controls3.c                                                                          CARBON// *******************************************************************************************// ���������������������������������������������������������������������������������� includes#include "Controls3.h"// �������������������������������������������������������������������������� global variablesBoolean	gRunningOnX									= false;Boolean	gInBackground								= false;Boolean	gDone;Str255	gCurrentString;Boolean	gBevelAndImageActive				= false;Boolean	gGroupArrowsProgressActive	= false;Boolean	gSlidersActive							= false;SInt16	gPixelDepth;Boolean	gIsColourDevice;// ************************************************************************************** mainvoid  main(void){	MenuBarHandle	menubarHdl;	SInt32				response;	MenuRef				menuRef;	EventRecord		eventStructure;	// ���������������������   ���  TEXT"        H 	Monaco                             c Oa c Oa�+��  	)  	.          h  h   nSORT� �  n MPSR   "MWBB   .TEXT   :pnot   F���      ���      "��          ��      