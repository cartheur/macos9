// *******************************************************************************************// Miscellany.c// *******************************************************************************************#include "Miscellany.h"// ииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииии global variablesDeviceLoopDrawingUPP	gDeviceLoopDrawUPP;Boolean								gDone;WindowRef							gWindowRef;ControlRef						gBevelButtonControlRef;ProcessSerialNumber		gProcessSerNum;Boolean								gMultiMonitorsDrawDemo = false;Boolean								gColourPickerDemo  = false;Boolean								gHelpTagsDemo = false;RGBColor							gWhiteColour = { 0xFFFF, 0xFFFF, 0xFFFF };RGBColor							gBlueColour  = { 0x6666, 0x6666, 0x9999 };Rect									controlRect = { 65,10,155,100 };// ************************************************************************************** mainvoid  main(void){	MenuBarHandle	menubarHdl;	SInt32				response;	MenuRef				menuRef;	EventRecord		eventStructure;	// ииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииии do preliminaries	doPreliminaries();	// ииииииииииииииииииииииииииииииииииииииииииииииииииииии create universal procedure pointer	gDeviceLoopDrawUPP = NewDeviceLoopDrawingUPP((DeviceLoopDrawingProcPtr) deviceLoopDraw);	// иииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииии set up menu bar and menus	menubarHdl = GetNewMBar(rMenubar);	if(menubarHdl == NULL)		ExitToShell();	SetMenuBar(menubarHdl);	DrawMenuBar();	Gestalt(gestaltMenuMgrAttr,&response);	if(response & gestaltMenuMgrAquaLayoutMask)	{		menuRef = GetMenuRef(mFile);		if(menuRef != NULL)		{			DeleteMenuItem(menuRef,iQuit);			DeleteMenuItem(menuRef,iQuit - 1);			DisableMenuItem(menuRef,0);		}	}	// иииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииии open window 	if(!(gWindowRef = GetNewCWindow(rWindow,NULL,(WindowRef)-1)))		ExitToShell();	SetPortWindowPort(gWindowRef);	TextSize(10);	// ииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииии create help tags	CreateBevelButtonControl(gWindowRef,&controlRect,CFSTR("Control"),												   kControlBevelButtonNormalBevel,kControlBehaviorPushbutton,													 NULL,0,0,0,&gBevelButtonControlRef);	HideControl(gBevelButtonControlRef);	doHelpTagControl();	doHelpTagWindow();	HMSetHelpTagsDisplayed(false);	// иииииииииииииииииииииииииииииииииииииииииииииии get process serial number of this process	GetCurrentProcess(&gProcessSerNum);		// иииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииии enter eventLoop	gDone = false;	while(!gDone)	{		if(WaitNextEvent(everyEvent,&eventStructure,1,NULL))			doEvents(&eventStructure);		else		{			if(eventStructure.what == nullEvent)				doIdle();		}	}}// *************************************************************************** doPreliminariesvoid  doPreliminaries(void){	OSErr	osError;	MoreMasterPointers(64);	InitCursor();	FlushEvents(everyEvent,0);	osError = AEInstallEventHandler(kCoreEventClass,kAEQuitApplication,														NewAEEventHandlerUPP((AEEventHandlerProcPtr) quitAppEventHandler),														0L,false);	if(osError != noErr)		ExitToShell();}// **************************************************************************** doQuitAppEventOSErr  quitAppEventHandler(AppleEvent *appEvent,AppleEvent *reply,SInt32 handlerRefcon){	OSErr			osError;	DescType	returnedType;	Size			actualSize;	osError = AEGetAttributePtr(appEvent,keyMissedKeywordAttr,typeWildCard,&returnedType,NULL,0,															&actualSize);	if(osError == errAEDescNotFound)	{		gDone = true;		osError = noErr;	} 	else if(osError == noErr)		osError = errAEParamMissed;	return osError;}// ********************************************************************************** doEventsvoid	doEvents(EventRecord *eventStrucPtr){	WindowPartCode	partCode, zoomPart;	WindowRef				windowRef;	SInt32					userData;	Rect						constraintRect, mainScreenRect, portRect;	Point						standardStateHeightAndWidth;	RgnHandle				regionHdl;	switch(eventStrucPtr->what)	{		case kHighLevelEvent:			AEProcessAppleEvent(eventStrucPtr);			break;		case mouseDown:			partCode = FindWindow(eventStrucPtr->where,&windowRef);			switch(partCode)			{				case inMenuBar:					doMenuChoice(MenuSelect(eventStrucPtr->where));					break;				case inContent:					if(windowRef != FrontWindow())						SelectWindow(windowRef);					break;				case inDrag:					DragWindow(windowRef,eventStrucPtr->where,NULL);					doHelpTagWindow();					break;				case inGrow:					constraintRect.top  = 302; 					constraintRect.left = 445;					constraintRect.bottom = constraintRect.right = 32767;					ResizeWindow(windowRef,eventStrucPtr->where,&constraintRect,NULL);					GetWindowPortBounds(windowRef,&portRect);					InvalWindowRect(windowRef,&portRect);					doHelpTagWindow();					break;				case inZoomIn:				case inZoomOut:					GetAvailableWindowPositioningBounds(GetMainDevice(),&mainScreenRect);					standardStateHeightAndWidth.v = mainScreenRect.bottom;					standardStateHeightAndWidth.h = mainScreenRect.right;										if(IsWindowInStandardState(windowRef,&standardStateHeightAndWidth,NULL))						zoomPart = inZoomIn;					else						zoomPart = inZoomOut;					if(TrackBox(windowRef,eventStrucPtr->where,zoomPart))					{						ZoomWindowIdeal(windowRef,zoomPart,&standardStateHeightAndWidth);						doHelpTagWindow();					}					break;			}			break;		case keyDown:			if((eventStrucPtr->modifiers & cmdKey) != 0)				doMenuChoice(MenuEvent(eventStrucPtr));			break;		case updateEvt:			windowRef = (WindowRef) eventStrucPtr->message;			BeginUpdate(windowRef);			if(gMultiMonitorsDrawDemo)			{				RGBBackColor(&gWhiteColour);				userData = (SInt32) windowRef;				regionHdl = NewRgn();				if(regionHdl)				{					GetPortVisibleRegion(GetWindowPort(windowRef),regionHdl);					DeviceLoop(regionHdl,gDeviceLoopDrawUPP,userData,0);					DisposeRgn(regionHdl);				}			}			else if(gColourPickerDemo )			{				RGBBackColor(&gBlueColour);				GetWindowPortBounds(windowRef,&portRect);				EraseRect(&portRect);				doDrawColourPickerChoice();			}			else			{				RGBBackColor(&gBlueColour);				GetWindowPortBounds(windowRef,&portRect);				EraseRect(&portRect);				if(gHelpTagsDemo)				{					Draw1Control(gBevelButtonControlRef);					RGBForeColor(&gWhiteColour);					MoveTo(10,20);					DrawString("\pHover the cursor in the window, and over the bevel button, ");					DrawString("\puntil the Help tag appears.");					MoveTo(10,35);					DrawString("\pPress the Command key to invoke the maximum content.");					MoveTo(10,50);					DrawString("\pDrag the window to a new location.");				}			}			EndUpdate(windowRef);			break;		case osEvt:			doOSEvent(eventStrucPtr);			break;	}}// ****************************************************************************** doMenuChoicevoid  doMenuChoice(SInt32 menuChoice){	MenuID				menuID;	MenuItemIndex	menuItem;	Rect					portRect;	menuID = HiWord(menuChoice);	menuItem = LoWord(menuChoice);	if(menuID == 0)		return;	switch(menuID)	{		case mAppleApplication:			if(menuItem == iAbout)				SysBeep(10);			break;		case mFile:			if(menuItem == iQuit)				ExitToShell();			break;		case mDemonstration:			gMultiMonitorsDrawDemo = gColourPickerDemo = gHelpTagsDemo = false;			if(HMAreHelpTagsDisplayed)				HMSetHelpTagsDisplayed(false);			HideControl(gBevelButtonControlRef);			GetWindowPortBounds(gWindowRef,&portRect);			switch(menuItem)			{				HideControl(gBevelButtonControlRef);				case iNotification:					RGBBackColor(&gBlueColour);					EraseRect(&portRect);					doSetUpNotification();					break;				case iProgress:					RGBBackColor(&gBlueColour);					EraseRect(&portRect);					doProgressIndicator();					break;				case iColourPicker:					gColourPickerDemo = true;					doColourPicker();					break;									case iMultiMonitors:					gMultiMonitorsDrawDemo = true;					InvalWindowRect(gWindowRef,&portRect);					break;				case iHelpTag:					gHelpTagsDemo = true;					InvalWindowRect(gWindowRef,&portRect);					ShowControl(gBevelButtonControlRef);					HMSetHelpTagsDisplayed(true);					break;			}						break;	}	HiliteMenu(0);}// *******************************************************************************************