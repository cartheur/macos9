// *******************************************************************************************// NewOpenCloseSave.c  // *******************************************************************************************// ииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииии includes#include "Files.h"// ииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииии global variablesNavDialogRef	gModalToApplicationNavDialogRef;SInt16				gCurrentNumberOfWindows = 0;Rect					gDestRect, gViewRect;Boolean				gCloseDocWindow = false;extern NavEventUPP	gGetFilePutFileEventFunctionUPP;extern SInt16				gAppResFileRefNum;extern Boolean			gQuittingApplication;extern Boolean			gRunningOnX;// ****************************************************************************** doNewCommandOSErr  doNewCommand(void){	WindowRef	windowRef;	OSErr 		osError;	OSType		documentType = kFileTypeTEXT;	osError = doNewDocWindow(true,documentType,&windowRef);	if(osError == noErr)		SetWindowProxyCreatorAndType(windowRef,kFileCreator,documentType,kUserDomain);			 /////	return osError;}// ***************************************************************************** doOpenCommandOSErr  doOpenCommand(void){	OSErr											osError = noErr;	NavDialogCreationOptions	dialogOptions;	Str255										applicationName;	NavTypeListHandle					fileTypeListHdl = NULL;	// ииииииииииииииииииииииииииииииииииииииииииииииииииии create application-modal Open dialog	osError = NavGetDefaultDialogCreationOptions(&dialogOptions);	if(osError == noErr)	{		GetIndString(applicationName,rMiscStrings,sApplicationName);		dialogOptions.clientName = CFStringCreateWithPascalString(NULL,applicationName,																															CFStringGetSystemEncoding());		dialogOptions.modality = kWindowModalityAppModal;		fileTypeListHdl = (NavTypeListHandle) GetResource('open',rOpenResource);		osError = NavCreateGetFileDialog(&dialogOptions,fileTypeListHdl,																		 gGetFilePutFileEventFunctionUPP,NULL,NULL,NULL,																		 &gModalToApplicationNavDialogRef);		if(osError == noErr && gModalToApplicationNavDialogRef != NULL)		{			osError = NavDialogRun(gModalToApplicationNavDialogRef);			if(osError != noErr)			{				NavDialogDispose(gModalToApplicationNavDialogRef);				gModalToApplicationNavDialogRef = NULL;			}		}				if(dialogOptions.clientName != NULL)			CFRelease(dialogOptions.clientName);		if(fileTypeListHdl != NULL)			ReleaseResource((Handle) fileTypeListHdl);	}	return osError;}// **************************************************************************** doCloseCommandOSErr  doCloseCommand(NavAskSaveChangesAction action){	WindowRef						windowRef;	SInt16							windowKind;	docStructureHandle	docStrucHdl;	OSErr								osError = noErr;	windowRef = FrontWindow();	windowKind = GetWindowKind(windowRef);	switch(windowKind)	{		case kApplicationWindowKind:			docStrucHdl = (docStructureHandle) GetWRefCon(windowRef);			// ииииииииииииииииииииииииииии if window has unsaved changes, create Save Changes alert			if((*docStrucHdl)->windowTouched == true)			{				if(IsWindowCollapsed(windowRef))					CollapseWindow(windowRef,false);				osError = doCreateAskSaveChangesDialog(windowRef,docStrucHdl,action);			}			// иииииииииииииииииииииииииииииииииииииииииииииииииии otherwise close file and clean up			else				osError = doCloseDocWindow(windowRef);			break;		case kDialogWindowKind:			// Hide or close modeless dialog, as required.			break;	}	return osError;}// ***************************************************************************** doSaveCommandOSErr  doSaveCommand(void){	WindowRef						windowRef;	docStructureHandle	docStrucHdl;	OSErr								osError = noErr;	Rect								portRect;	windowRef = FrontWindow();	docStrucHdl = (docStructureHandle) GetWRefCon(windowRef);	// иии if the document has a file ref number, write the file, otherwise call doSaveAsCommand	if((*docStrucHdl)->fileRefNum)	{		osError = doWriteFile(windowRef);				SetPortWindowPort(windowRef);		GetWindowPortBounds(windowRef,&portRect);		EraseRect(&portRect);		InvalWindowRect(windowRef,&portRect);	}	else		osError = doSaveAsCommand();	if(osError == noErr)																																	 /////		SetWindowModified(windowRef,false);																									 /////	return osError;}// *************************************************************************** doSaveAsCommandOSErr  doSaveAsCommand(void){	OSErr											osError = noErr;	NavDialogCreationOptions	dialogOptions;	WindowRef									windowRef;	Str255										windowTitle, applicationName;	docStructureHandle				docStrucHdl;	OSType										fileType;	// ииииииииииииииииииииииииииииииииииииииииииииииии create window-modal Save Location dialog	osError = NavGetDefaultDialogCreationOptions(&dialogOptions);	if(osError == noErr)	{		dialogOptions.optionFlags |= kNavNoTypePopup;		windowRef = FrontWindow();		GetWTitle(windowRef,windowTitle);		dialogOptions.saveFileName = CFStringCreateWithPascalString(NULL,windowTitle,																																CFStringGetSystemEncoding());		GetIndString(applicationName,rMiscStrings,sApplicationName);		dialogOptions.clientName = CFStringCreateWithPascalString(NULL,applicationName,																															CFStringGetSystemEncoding());		dialogOptions.parentWindow = windowRef;		dialogOptions.modality = kWindowModalityWindowModal;				docStrucHdl = (docStructureHandle) GetWRefCon(windowRef);		if((*docStrucHdl)->editStrucHdl != NULL)			fileType = kFileTypeTEXT;		else if((*docStrucHdl)->pictureHdl != NULL)			fileType = kFileTypePICT;		HLock((Handle) docStrucHdl);		osError = NavCreatePutFileDialog(&dialogOptions,fileType,kFileCreator,																		 gGetFilePutFileEventFunctionUPP ,																		 windowRef,&(*docStrucHdl)->modalToWindowNavDialogRef);		HUnlock((Handle) docStrucHdl);		if(osError == noErr && (*docStrucHdl)->modalToWindowNavDialogRef != NULL)		{			osError = NavDialogRun((*docStrucHdl)->modalToWindowNavDialogRef);			if(osError != noErr)			{				NavDialogDispose((*docStrucHdl)->modalToWindowNavDialogRef);				(*docStrucHdl)->modalToWindowNavDialogRef = NULL;			}		}		if(dialogOptions.saveFileName != NULL)			CFRelease(dialogOptions.saveFileName);		if(dialogOptions.clientName != NULL)			CFRelease(dialogOptions.clientName);	}	return osError;}// *************************************************************************** doRevertCommandOSErr  doRevertCommand(void){	OSErr											osError = noErr;	NavDialogCreationOptions	dialogOptions;	WindowRef									windowRef;	Str255										windowTitle;	docStructureHandle				docStrucHdl;	// иииииииииииииииииииииииииииииииииииииииииииииии create window-modal Discard Changes alert	osError = NavGetDefaultDialogCreationOptions(&dialogOptions);	if(osError == noErr)	{		windowRef = FrontWindow();		GetWTitle(windowRef,windowTitle);		dialogOptions.saveFileName = CFStringCreateWithPascalString(NULL,windowTitle,																																CFStringGetSystemEncoding());		dialogOptions.parentWindow = windowRef;		dialogOptions.modality = kWindowModalityWindowModal;		docStrucHdl = (docStructureHandle) GetWRefCon(windowRef);		if((*docStrucHdl)->askSaveDiscardEventFunctionUPP != NULL)		{			DisposeNavEventUPP((*docStrucHdl)->askSaveDiscardEventFunctionUPP);			(*docStrucHdl)->askSaveDiscardEventFunctionUPP = NULL;		}		(*docStrucHdl)->askSaveDiscardEventFunctionUPP = 														NewNavEventUPP((NavEventProcPtr) askSaveDiscardEventFunction); 		HLock((Handle) docStrucHdl);		osError = NavCreateAskDiscardChangesDialog(&dialogOptions,																							 (*docStrucHdl)->askSaveDiscardEventFunctionUPP,																							 windowRef,																							 &(*docStrucHdl)->modalToWindowNavDialogRef);		HUnlock((Handle) docStrucHdl);		if(osError == noErr && (*docStrucHdl)->modalToWindowNavDialogRef != NULL)		{			osError = NavDialogRun((*docStrucHdl)->modalToWindowNavDialogRef);			if(osError != noErr)			{				NavDialogDispose((*docStrucHdl)->modalToWindowNavDialogRef);				(*docStrucHdl)->modalToWindowNavDialogRef = NULL;			}		}		if(dialogOptions.saveFileName != NULL)			CFRelease(dialogOptions.saveFileName);	}	return osError;}// **************************************************************************** doNewDocWindowOSErr  doNewDocWindow(Boolean showWindow,OSType documentType,WindowRef * windowRef){	OSStatus						osError;	WindowAttributes		attributes	= kWindowStandardHandlerAttribute |																		kWindowStandardDocumentAttributes;	Rect								portRect, contentRect = { 0,0,300,500 };	docStructureHandle	docStrucHdl;	EventTypeSpec				windowEvents[] = { { kEventClassWindow,	kEventWindowDrawContent  },																				 { kEventClassWindow, kEventWindowClose        },																				 { kEventClassWindow, kEventWindowClickDragRgn },																				 { kEventClassWindow, kEventWindowPathSelect   } };	if(gCurrentNumberOfWindows == kMaxWindows)		return eMaxWindows;	// иииииииииииииииииииии create window, change attributes, reposition, install event handler	osError = CreateNewWindow(kDocumentWindowClass,attributes,&contentRect,windowRef);	if(osError != noErr)		return osError;	SetWTitle(*windowRef,"\puntitled");	ChangeWindowAttributes(*windowRef,0,kWindowFullZoomAttribute | kWindowResizableAttribute);	RepositionWindow(*windowRef,NULL,kWindowCascadeOnMainScreen);	SetPortWindowPort(*windowRef);		InstallWindowEventHandler(*windowRef,doGetHandlerUPP(),GetEventTypeCount(windowEvents),														windowEvents,0,NULL);	// иииииииииииииииииииииииииииииииииииииииииииииииииииии attach document structure to window	if(!(docStrucHdl = (docStructureHandle) NewHandle(sizeof(docStructure))))	{		DisposeWindow(*windowRef);		return MemError();	}	SetWRefCon(*windowRef,(SInt32) docStrucHdl);	(*docStrucHdl)->editStrucHdl										= NULL;	(*docStrucHdl)->pictureHdl											= NULL;	(*docStrucHdl)->fileRefNum											= 0;	(*docStrucHdl)->aliasHdl												= NULL;																 /////	(*docStrucHdl)->windowTouched										= false;	(*docStrucHdl)->modalToWindowNavDialogRef 			= NULL;	(*docStrucHdl)->askSaveDiscardEventFunctionUPP	= NULL;	(*docStrucHdl)->isAskSaveChangesDialog					= false;	// иииииииииииииииииииииииииииииииииииииииииииии if text document, create TextEdit structure	if(documentType == kFileTypeTEXT)	{		UseThemeFont(kThemeSmallSystemFont,smSystemScript);		GetWindowPortBounds(*windowRef,&portRect);		gDestRect = portRect;		InsetRect(&gDestRect,6,6);		gViewRect = gDestRect;		MoveHHi((Handle) docStrucHdl);		HLock((Handle) docStrucHdl);		if(!((*docStrucHdl)->editStrucHdl = TENew(&gDestRect,&gViewRect)))		{			DisposeWindow(*windowRef);			DisposeHandle((Handle) docStrucHdl);			return MemError();		}		HUnlock((Handle) docStrucHdl);	}	// ииииииииииииииииииииииииииииииииииииииииииии show window and increment open windows count	if(showWindow)		ShowWindow(*windowRef);	gCurrentNumberOfWindows ++;	return noErr;}// *************************************************************************** doGetHandlerUPPEventHandlerUPP  doGetHandlerUPP(void){	static EventHandlerUPP	windowEventHandlerUPP;	if(windowEventHandlerUPP == NULL)		windowEventHandlerUPP = NewEventHandlerUPP((EventHandlerProcPtr) windowEventHandler);	return windowEventHandlerUPP;}// ************************************************************************** doCloseDocWindowOSErr  doCloseDocWindow(WindowRef windowRef){	docStructureHandle	docStrucHdl;	OSErr								osError = noErr;	EventRef						eventRef;	EventTargetRef			eventTargetRef;	docStrucHdl = (docStructureHandle) GetWRefCon(windowRef);	// иииииииииииииииииииииии close file, flush volume, dispose of window and associated memory	if((*docStrucHdl)->fileRefNum != 0)	{		if(!(osError = FSClose((*docStrucHdl)->fileRefNum)))			{			osError = FlushVol(NULL,(*docStrucHdl)->fileFSSpec.vRefNum);			(*docStrucHdl)->fileRefNum = 0;		}	}	if((*docStrucHdl)->editStrucHdl != NULL)		TEDispose((*docStrucHdl)->editStrucHdl);	if((*docStrucHdl)->pictureHdl != NULL)		KillPicture((*docStrucHdl)->pictureHdl);	DisposeHandle((Handle) docStrucHdl);	DisposeWindow(windowRef);	gCurrentNumberOfWindows --;	// иииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииии if quitting application	if(gQuittingApplication)	{		if(FrontWindow() == NULL)			QuitApplicationEventLoop();		else		{			CreateEvent(NULL,kEventClassWindow,kEventWindowClose,0,kEventAttributeNone,									&eventRef);			eventTargetRef = GetWindowEventTarget(FrontWindow());			SendEventToEventTarget(eventRef,eventTargetRef);		}	}	return osError;}// ******************************************************************************** doOpenFileOSErr  doOpenFile(FSSpec fileSpec,OSType documentType){	WindowRef						windowRef;	OSErr								osError = noErr;	SInt16							fileRefNum;	docStructureHandle	docStrucHdl;	// иииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииии create new window	if(osError = doNewDocWindow(false,documentType,&windowRef))		return osError;	SetWTitle(windowRef,fileSpec.name);		// иииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииии open file's data fork	if(osError = FSpOpenDF(&fileSpec,fsRdWrPerm,&fileRefNum))	{		DisposeWindow(windowRef);		gCurrentNumberOfWindows --;		return osError;	}	// иииииииииииииииииии store file reference number and FSSpec in window's document structure	docStrucHdl = (docStructureHandle) GetWRefCon(windowRef);	(*docStrucHdl)->fileRefNum = fileRefNum;	(*docStrucHdl)->fileFSSpec = fileSpec;	// ииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииии read in the file	if(documentType == kFileTypeTEXT)	{		if(osError = doReadTextFile(windowRef))			return osError;	}	else if(documentType == kFileTypePICT)	{		if(osError = doReadPictFile(windowRef))			return osError;	}	// иииииииииииииииииииииииииииииииииииииииииииии set up window's proxy icon, and show window	SetWindowProxyFSSpec(windowRef,&fileSpec);																						 /////	GetWindowProxyAlias(windowRef,&((*docStrucHdl)->aliasHdl));														 /////	SetWindowModified(windowRef,false);																										 /////	ShowWindow(windowRef);	return noErr;}// ************************************************************** doCreateAskSaveChangesDialogOSErr  doCreateAskSaveChangesDialog(WindowRef windowRef,docStructureHandle docStrucHdl,																	  NavAskSaveChangesAction action){	OSErr											osError = noErr;	NavDialogCreationOptions	dialogOptions;	Str255										windowTitle, applicationName;	// иииииииииииииииииииииииииииииииииииииииии create window-modal Save Changes Changes dialog		osError = NavGetDefaultDialogCreationOptions(&dialogOptions);	if(osError == noErr)	{		GetWTitle(windowRef,windowTitle);		dialogOptions.saveFileName = CFStringCreateWithPascalString(NULL,windowTitle,																																CFStringGetSystemEncoding());		GetIndString(applicationName,rMiscStrings,sApplicationName);		dialogOptions.clientName = CFStringCreateWithPascalString(NULL,applicationName,																															CFStringGetSystemEncoding());		dialogOptions.parentWindow = windowRef;		dialogOptions.modality = kWindowModalityWindowModal;		if((*docStrucHdl)->askSaveDiscardEventFunctionUPP != NULL)		{			DisposeNavEventUPP((*docStrucHdl)->askSaveDiscardEventFunctionUPP);			(*docStrucHdl)->askSaveDiscardEventFunctionUPP = NULL;		}		(*docStrucHdl)->askSaveDiscardEventFunctionUPP = 																NewNavEventUPP((NavEventProcPtr) askSaveDiscardEventFunction);		HLock((Handle) docStrucHdl);		osError = NavCreateAskSaveChangesDialog(&dialogOptions,action,																						(*docStrucHdl)->askSaveDiscardEventFunctionUPP,																						windowRef,																						&(*docStrucHdl)->modalToWindowNavDialogRef);		HUnlock((Handle) docStrucHdl);		if(osError == noErr && (*docStrucHdl)->modalToWindowNavDialogRef != NULL)		{			(*docStrucHdl)->isAskSaveChangesDialog = true;			osError = NavDialogRun((*docStrucHdl)->modalToWindowNavDialogRef);			if(osError != noErr)			{				NavDialogDispose((*docStrucHdl)->modalToWindowNavDialogRef);				(*docStrucHdl)->modalToWindowNavDialogRef = NULL;				(*docStrucHdl)->isAskSaveChangesDialog = false;			}			if(!gRunningOnX)			{				if(gCloseDocWindow)				{					osError = doCloseDocWindow(windowRef);					if(osError != noErr)						doErrorAlert(osError);					gCloseDocWindow = false;						}			}		}		if(dialogOptions.saveFileName != NULL)			CFRelease(dialogOptions.saveFileName);		if(dialogOptions.clientName != NULL)			CFRelease(dialogOptions.clientName);	}	return osError;}// ************************************************************************* doSaveUsingFSSpecOSErr  doSaveUsingFSSpec(WindowRef windowRef,NavReplyRecord *navReplyStruc){	OSErr								osError = noErr;	AEKeyword						theKeyword;	DescType						actualType;	FSSpec							fileSpec;	Size								actualSize;	docStructureHandle	docStrucHdl;	OSType 							fileType;	CFStringRef					fileName;	SInt16							fileRefNum;	Rect								portRect;	if((*navReplyStruc).validRecord)	{		// ииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииии get FSSpec		if((osError = AEGetNthPtr(&(*navReplyStruc).selection,1,typeFSS,&theKeyword,																&actualType,&fileSpec,sizeof(fileSpec),&actualSize)) == noErr)		{			docStrucHdl = (docStructureHandle) GetWRefCon(windowRef);			// иииииииииииии get file name, convert to Pascal string, assign to name field of FSSpec			fileName = NavDialogGetSaveFileName((*docStrucHdl)->modalToWindowNavDialogRef);			if(fileName != NULL)				osError = CFStringGetPascalString(fileName,&fileSpec.name[0],sizeof(FSSpec),																					CFStringGetSystemEncoding());			// иииииииииииииииииииииииииииииииииииииииииии if not replacing, first create a new file			if(!((*navReplyStruc).replacing))			{				if((*docStrucHdl)->editStrucHdl != NULL)					fileType = kFileTypeTEXT;				else if((*docStrucHdl)->pictureHdl != NULL)					fileType = kFileTypePICT;				osError = FSpCreate(&fileSpec,kFileCreator,fileType,(*navReplyStruc).keyScript);				if(osError != noErr)				{					NavDisposeReply(&(*navReplyStruc));					return osError;				}			}			// ииииииииииииииииииии assign FSSpec to fileFSSpec field of window's document structure			(*docStrucHdl)->fileFSSpec = fileSpec;			// иииииииииииииииииииииииииииииииииииии if file currently exists for document, close it			if((*docStrucHdl)->fileRefNum != 0)			{				osError = FSClose((*docStrucHdl)->fileRefNum);				(*docStrucHdl)->fileRefNum = 0;			}			// ииииииииииииииииииииииииииииииииииииииииииииииии open file's data fork and write file						if(osError == noErr)				osError = FSpOpenDF(&(*docStrucHdl)->fileFSSpec,fsRdWrPerm,&fileRefNum);			if(osError == noErr)			{				(*docStrucHdl)->fileRefNum = fileRefNum;				SetWTitle(windowRef,fileSpec.name);				// ийийийийийийийийийийийийи и и и и и и и и proxy icon and file synchronisation stuff				SetPortWindowPort(windowRef);																										 /////				SetWindowProxyFSSpec(windowRef,&fileSpec);																			 /////				GetWindowProxyAlias(windowRef,&((*docStrucHdl)->aliasHdl));											 /////				SetWindowModified(windowRef,false);																							 /////				// ийийийийийи ийийийийийийийийийийийи и и и и и ийийийийий write file using safe save								osError = doWriteFile(windowRef);				NavCompleteSave(&(*navReplyStruc),kNavTranslateInPlace);			}		}	}	SetPortWindowPort(windowRef);	GetWindowPortBounds(windowRef,&portRect);	EraseRect(&portRect);	InvalWindowRect(windowRef,&portRect);	return osError;}// ************************************************************************** doSaveUsingFSRefOSErr  doSaveUsingFSRef(WindowRef windowRef,NavReplyRecord *navReplyStruc){	OSErr								osError = noErr;	AEDesc							aeDesc;	Size								dataSize;	FSRef								fsRefParent, fsRefDelete;		UniCharCount				nameLength;	UniChar							*nameBuffer;	FSSpec							fileSpec;	docStructureHandle	docStrucHdl;	FInfo								fileInfo;	SInt16							fileRefNum;	Rect								portRect;	osError = AECoerceDesc(&(*navReplyStruc).selection,typeFSRef,&aeDesc);	if(osError == noErr)	{		// иииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииии get FSRef		dataSize = AEGetDescDataSize(&aeDesc);		if(dataSize > 0)			osError = AEGetDescData(&aeDesc,&fsRefParent,sizeof(FSRef));		if(osError == noErr)		{			// иииииииииииииииииииииииииииии get file name from saveFileName field of NavReplyRecord			nameLength = (UniCharCount) CFStringGetLength((*navReplyStruc).saveFileName);			nameBuffer = (UniChar *) NewPtr(nameLength);			CFStringGetCharacters((*navReplyStruc).saveFileName,CFRangeMake(0,nameLength),														&nameBuffer[0]);			if(nameBuffer != NULL)			{				// ииииииииииииииииииииииииииииииииииииии if replacing, delete the file being replaced				if((*navReplyStruc).replacing)				{					osError = FSMakeFSRefUnicode(&fsRefParent,nameLength,nameBuffer,																			 kTextEncodingUnicodeDefault,&fsRefDelete);					{						if(osError == noErr)							osError = FSDeleteObject(&fsRefDelete);						if(osError == fBsyErr)						{							DisposePtr((Ptr) nameBuffer);							return osError;						}					}				}				// ииииииииииииии create file with Unicode name (but it can be written with an FSSpec)				if(osError == noErr)				{					osError = FSCreateFileUnicode(&fsRefParent,nameLength,nameBuffer,kFSCatInfoNone,																				NULL,NULL,&fileSpec);					if(osError == noErr)					{						docStrucHdl = (docStructureHandle) GetWRefCon(windowRef);						osError = FSpGetFInfo(&fileSpec,&fileInfo);						if((*docStrucHdl)->editStrucHdl != NULL)							fileInfo.fdType = kFileTypeTEXT;						else if((*docStrucHdl)->pictureHdl != NULL)							fileInfo.fdType = kFileTypePICT;						fileInfo.fdCreator = kFileCreator;						if(osError == noErr)							osError = FSpSetFInfo(&fileSpec,&fileInfo);						(*docStrucHdl)->fileFSSpec = fileSpec;						// ииииииииииииииииииииииииииииииииииииииииии open file's data fork and write file						if(osError == noErr)							osError = FSpOpenDF(&fileSpec,fsRdWrPerm,&fileRefNum);						if(osError == noErr)						{							(*docStrucHdl)->fileRefNum = fileRefNum;							SetWTitle(windowRef,fileSpec.name);							// ийийийийийийийийийийийийи и и и и и proxy icon and file synchronisation stuff							SetPortWindowPort(windowRef);																							 /////							SetWindowProxyFSSpec(windowRef,&fileSpec);																 /////							GetWindowProxyAlias(windowRef,&((*docStrucHdl)->aliasHdl));								 /////							SetWindowModified(windowRef,false);																				 /////							// ийийийийийи ийийийийийийийийийийийи и и и и и ийий write file using safe save							osError = doWriteFile(windowRef);							NavCompleteSave(&(*navReplyStruc),kNavTranslateInPlace);						}					}				}			}					DisposePtr((Ptr) nameBuffer);		}		AEDisposeDesc(&aeDesc);	}		SetPortWindowPort(windowRef);	GetWindowPortBounds(windowRef,&portRect);	EraseRect(&portRect);	InvalWindowRect(windowRef,&portRect);	return osError;}// ******************************************************************************* doWriteFileOSErr  doWriteFile(WindowRef windowRef){	docStructureHandle	docStrucHdl;	FSSpec							fileSpecActual, fileSpecTemp;	UInt32							currentTime;	Str255							tempFileName;	SInt16							tempFileVolNum, tempFileRefNum;	SInt32							tempFileDirID;	OSErr								osError = noErr;	docStrucHdl = (docStructureHandle) GetWRefCon(windowRef);	fileSpecActual = (*docStrucHdl)->fileFSSpec;	GetDateTime(&currentTime);	NumToString((SInt32) currentTime,tempFileName);		osError = FindFolder(fileSpecActual.vRefNum,kTemporaryFolderType,kCreateFolder,												&tempFileVolNum,&tempFileDirID);	if(osError == noErr)		osError = FSMakeFSSpec(tempFileVolNum,tempFileDirID,tempFileName,&fileSpecTemp);	if(osError == noErr || osError == fnfErr)		osError = FSpCreate(&fileSpecTemp,'trsh','trsh',smSystemScript);	if(osError == noErr)		osError = FSpOpenDF(&fileSpecTemp,fsRdWrPerm,&tempFileRefNum);	if(osError == noErr)	{		if((*docStrucHdl)->editStrucHdl)			osError = doWriteTextData(windowRef,tempFileRefNum);		else if((*docStrucHdl)->pictureHdl)			osError = doWritePictData(windowRef,tempFileRefNum);	}		if(osError == noErr)		osError = FSClose(tempFileRefNum);	if(osError == noErr)		osError = FSClose((*docStrucHdl)->fileRefNum);	if(osError == noErr)		osError = FSpExchangeFiles(&fileSpecTemp,&fileSpecActual);	if(osError == noErr)		osError = FSpDelete(&fileSpecTemp);	if(osError == noErr)		osError = FSpOpenDF(&fileSpecActual,fsRdWrPerm,&(*docStrucHdl)->fileRefNum);	if(osError == noErr)		osError = doCopyResources(windowRef);	return osError;}// **************************************************************************** doReadTextFileOSErr  doReadTextFile(WindowRef windowRef){	docStructureHandle	docStrucHdl;	SInt16							fileRefNum;	TEHandle						textEditHdl;	SInt32							numberOfBytes;	Handle							textBuffer;	OSErr								osError = noErr;	docStrucHdl = (docStructureHandle) GetWRefCon(windowRef);	fileRefNum = (*docStrucHdl)->fileRefNum;	textEditHdl = (*docStrucHdl)->editStrucHdl;	(*textEditHdl)->txSize = 10;	(*textEditHdl)->lineHeight = 15;	SetFPos(fileRefNum,fsFromStart,0);	GetEOF(fileRefNum,&numberOfBytes);	if(numberOfBytes > 32767)		numberOfBytes = 32767;	if(!(textBuffer = NewHandle((Size) numberOfBytes)))		return MemError();	osError = FSRead(fileRefNum,&numberOfBytes,*textBuffer);	if(osError == noErr || osError == eofErr)	{		HLockHi(textBuffer);		TESetText(*textBuffer,numberOfBytes,(*docStrucHdl)->editStrucHdl);		HUnlock(textBuffer);		DisposeHandle(textBuffer);	}	else		return osError;	return noErr;}// **************************************************************************** doReadPictFileOSErr  doReadPictFile(WindowRef windowRef){	docStructureHandle	docStrucHdl;	SInt16							fileRefNum;	SInt32							numberOfBytes;	OSErr								osError = noErr;	docStrucHdl = (docStructureHandle) GetWRefCon(windowRef);	fileRefNum = (*docStrucHdl)->fileRefNum;	GetEOF(fileRefNum,&numberOfBytes);	SetFPos(fileRefNum,fsFromStart,512);	numberOfBytes -= 512;	if(!((*docStrucHdl)->pictureHdl = (PicHandle) NewHandle(numberOfBytes)))		return MemError();	osError = FSRead(fileRefNum,&numberOfBytes,*(*docStrucHdl)->pictureHdl);	if(osError == noErr || osError == eofErr)		return(noErr);	else		return osError;}// *************************************************************************** doWriteTextDataOSErr  doWriteTextData(WindowRef windowRef,SInt16 tempFileRefNum){	docStructureHandle	docStrucHdl;	TEHandle						textEditHdl;	Handle							editText;	SInt32							numberOfBytes;	SInt16							volRefNum;	OSErr								osError = noErr;	docStrucHdl = (docStructureHandle) GetWRefCon(windowRef);	textEditHdl = (*docStrucHdl)->editStrucHdl;	editText = (*textEditHdl)->hText;	numberOfBytes = (*textEditHdl)->teLength;	osError = SetFPos(tempFileRefNum,fsFromStart,0);	if(osError == noErr)		osError = FSWrite(tempFileRefNum,&numberOfBytes,*editText);	if(osError == noErr)		osError = SetEOF(tempFileRefNum,numberOfBytes);	if(osError == noErr)		osError = GetVRefNum(tempFileRefNum,&volRefNum);	if(osError == noErr)		osError = FlushVol(NULL,volRefNum);	if(osError == noErr)		(*docStrucHdl)->windowTouched = false;	return osError;}// *************************************************************************** doWritePictDataOSErr  doWritePictData(WindowRef windowRef,SInt16 tempFileRefNum){	docStructureHandle	docStrucHdl;	PicHandle						pictureHdl;	SInt32							numberOfBytes, dummyData;	SInt16							volRefNum;	OSErr								osError = noErr;	docStrucHdl = (docStructureHandle) GetWRefCon(windowRef);	pictureHdl = (*docStrucHdl)->pictureHdl;	numberOfBytes = 512;	dummyData = 0;	osError = SetFPos(tempFileRefNum,fsFromStart,0);	if(osError == noErr)		osError = FSWrite(tempFileRefNum,&numberOfBytes,&dummyData);	numberOfBytes = GetHandleSize((Handle) (*docStrucHdl)->pictureHdl);	if(osError == noErr)	{		HLock((Handle) (*docStrucHdl)->pictureHdl);		osError = FSWrite(tempFileRefNum,&numberOfBytes,*(*docStrucHdl)->pictureHdl);		HUnlock((Handle) (*docStrucHdl)->pictureHdl);	}	if(osError == noErr)		osError = SetEOF(tempFileRefNum,512 + numberOfBytes);	if(osError == noErr)		osError = GetVRefNum(tempFileRefNum,&volRefNum);	if(osError == noErr)		osError = FlushVol(NULL,volRefNum);	if(osError == noErr)		(*docStrucHdl)->windowTouched = false;	return osError;}// *************************************************************** getFilePutFileEventFunctionvoid  getFilePutFileEventFunction(NavEventCallbackMessage callBackSelector,																	NavCBRecPtr callBackParms,NavCallBackUserData callBackUD){	OSErr								osError = noErr;	NavReplyRecord			navReplyStruc;	NavUserAction				navUserAction;	SInt32							count, index;	AEKeyword						theKeyword;	DescType						actualType;	FSSpec							fileSpec;	Size								actualSize;	FInfo								fileInfo;	OSType							documentType;	WindowRef						windowRef;	AEDesc							aeDesc;	AEKeyword						keyWord;	DescType						typeCode;	Rect								theRect;	Str255							theString, numberString;	docStructureHandle	docStrucHdl;	switch(callBackSelector)	{		case kNavCBUserAction:			osError = NavDialogGetReply(callBackParms->context,&navReplyStruc);			if(osError == noErr && navReplyStruc.validRecord)			{				navUserAction = NavDialogGetUserAction(callBackParms->context);				switch(navUserAction)				{					// иииииииииииииииииииииииииииииииииииииииииииии click on Open button in Open dialog					case kNavUserActionOpen:						if(gModalToApplicationNavDialogRef != NULL)						{							osError = AECountItems(&(navReplyStruc.selection),&count);							if(osError == noErr)							{								for(index=1;index<=count;index++)								{									osError = AEGetNthPtr(&(navReplyStruc.selection),index,typeFSS,																				&theKeyword,&actualType,&fileSpec,sizeof(fileSpec),																				&actualSize);									if((osError = FSpGetFInfo(&fileSpec,&fileInfo)) == noErr)									{										documentType = fileInfo.fdType;										osError = doOpenFile(fileSpec,documentType);										if(osError != noErr)											doErrorAlert(osError);									}								}							}						}						break;					// ииииииииииииииииииииииииииииииииииии click on Save button in Save Location dialog					case kNavUserActionSaveAs:						windowRef = callBackUD;						osError = AECoerceDesc(&navReplyStruc.selection,typeFSRef,&aeDesc);						if(osError == noErr)						{							osError = doSaveUsingFSRef(windowRef,&navReplyStruc);							if(osError != noErr)								doErrorAlert(osError);							AEDisposeDesc(&aeDesc);						}						else						{							osError = doSaveUsingFSSpec(windowRef,&navReplyStruc);							if(osError != noErr)								doErrorAlert(osError);						}						break;					// ииииииииииииииииииииииииииииииии click on Choose button in Choose a Folder dialog					case kNavUserActionChoose:						if((osError = AEGetNthPtr(&(navReplyStruc.selection),1,typeFSS,&keyWord,&typeCode,																			&fileSpec,sizeof(FSSpec),&actualSize)) == noErr)						{							FSMakeFSSpec(fileSpec.vRefNum,fileSpec.parID,fileSpec.name,&fileSpec);						}						windowRef = callBackUD;						SetPortWindowPort(windowRef);						TextSize(10);						SetRect(&theRect,0,271,600,300);						EraseRect(&theRect);						doCopyPString(fileSpec.name,theString);						doConcatPStrings(theString, "\p   Volume Reference Number: ");						NumToString((SInt32) fileSpec.vRefNum,numberString);						doConcatPStrings(theString,numberString);						doConcatPStrings(theString, "\p   Parent Directory ID: ");						NumToString((SInt32) fileSpec.parID,numberString);						doConcatPStrings(theString,numberString);						MoveTo(10,290);						DrawString(theString);						break;				}				osError = NavDisposeReply(&navReplyStruc);			}			break;		case kNavCBTerminate:			if(gModalToApplicationNavDialogRef != NULL)			{				NavDialogDispose(gModalToApplicationNavDialogRef);				gModalToApplicationNavDialogRef = NULL;			}			else			{				windowRef = callBackUD;				docStrucHdl = (docStructureHandle) GetWRefCon(windowRef);				if((*docStrucHdl)->modalToWindowNavDialogRef != NULL)				{					NavDialogDispose((*docStrucHdl)->modalToWindowNavDialogRef);					(*docStrucHdl)->modalToWindowNavDialogRef = NULL;				}			}			break;	}}// *************************************************************** askSaveDiscardEventFunctionvoid askSaveDiscardEventFunction(NavEventCallbackMessage callBackSelector,																 NavCBRecPtr callBackParms,NavCallBackUserData callBackUD){	WindowRef						windowRef;	docStructureHandle	docStrucHdl;	NavUserAction				navUserAction;	OSErr								osError = noErr;	Rect								portRect;  switch(callBackSelector)	{		case kNavCBUserAction:			windowRef = callBackUD;			docStrucHdl = (docStructureHandle) GetWRefCon(windowRef);			if((*docStrucHdl)->modalToWindowNavDialogRef != NULL)			{				navUserAction = NavDialogGetUserAction(callBackParms->context);				switch(navUserAction)				{					// ииииииииииииииииииииииииииииииииииииии click on Save button in Save Changes alert					case kNavUserActionSaveChanges:						osError = doSaveCommand();						if(osError != noErr)							doErrorAlert(osError);					// ииииииииииииииииииииииииииииииии click on Don't Save button in Save Changes alert					case kNavUserActionDontSaveChanges:						NavDialogDispose((*docStrucHdl)->modalToWindowNavDialogRef);						if(gRunningOnX)						{							osError = doCloseDocWindow(windowRef);							if(osError != noErr)								doErrorAlert(osError);						}						else							gCloseDocWindow = true;						break;					// иииииииииииииииииииииииииииииииииииии click on OK button in Discard Changes alert					case kNavUserActionDiscardChanges:						GetWindowPortBounds(windowRef,&portRect);						SetPortWindowPort(windowRef);						EraseRect(&portRect);						if((*docStrucHdl)->editStrucHdl != NULL && (*docStrucHdl)->fileRefNum != 0)						{							osError = doReadTextFile(windowRef);							if(osError != noErr)								doErrorAlert(osError);						}						else if((*docStrucHdl)->pictureHdl != NULL)						{							KillPicture((*docStrucHdl)->pictureHdl);							(*docStrucHdl)->pictureHdl = NULL;							osError = doReadPictFile(windowRef);							if(osError != noErr)								doErrorAlert(osError);						}						(*docStrucHdl)->windowTouched = false;						SetWindowModified(windowRef,false);																					 /////						InvalWindowRect(windowRef,&portRect);						NavDialogDispose((*docStrucHdl)->modalToWindowNavDialogRef);						(*docStrucHdl)->modalToWindowNavDialogRef = NULL;						break;					// иииииииииииииииии click on Cancel button in Save Changes or Discard Changes alert					case kNavUserActionCancel:						if((*docStrucHdl)->isAskSaveChangesDialog == true)						{							gQuittingApplication = false;							(*docStrucHdl)->isAskSaveChangesDialog = false;						}						NavDialogDispose((*docStrucHdl)->modalToWindowNavDialogRef);						(*docStrucHdl)->modalToWindowNavDialogRef = NULL;						break;				}			}			break;	}}// *************************************************************************** doCopyResourcesOSErr  doCopyResources(WindowRef windowRef){	docStructureHandle	docStrucHdl;	OSType							fileType;	OSErr								osError = noErr;	SInt16							fileRefNum;	Handle							editTextHdl, textResourceHdl;	docStrucHdl = (docStructureHandle) GetWRefCon(windowRef);	if((*docStrucHdl)->editStrucHdl)		fileType = kFileTypeTEXT;	else if((*docStrucHdl)->pictureHdl)		fileType = kFileTypePICT;	FSpCreateResFile(&(*docStrucHdl)->fileFSSpec,kFileCreator,fileType,smSystemScript);	osError = ResError();	if(osError == noErr)		fileRefNum = FSpOpenResFile(&(*docStrucHdl)->fileFSSpec,fsRdWrPerm);	if(fileRefNum > 0)	{		osError = doCopyAResource('STR ',-16396,gAppResFileRefNum,fileRefNum);		if(fileType == kFileTypePICT)		{			doCopyAResource('pnot',128,gAppResFileRefNum,fileRefNum);			doCopyAResource('PICT',128,gAppResFileRefNum,fileRefNum);		}		if(!gRunningOnX && fileType == kFileTypeTEXT)		{			doCopyAResource('pnot',129,gAppResFileRefNum,fileRefNum);						editTextHdl = (*(*docStrucHdl)->editStrucHdl)->hText;			textResourceHdl = NewHandleClear(1024);			BlockMoveData(*editTextHdl,*textResourceHdl,1024);						UseResFile(fileRefNum);			AddResource(textResourceHdl,'TEXT',129,"\p");			if(ResError() == noErr)				UpdateResFile(fileRefNum);			ReleaseResource(textResourceHdl);		}	}	else		osError = ResError();	if(osError == noErr)		CloseResFile(fileRefNum); 	osError = ResError();	return osError;}// *************************************************************************** doCopyAResourceOSErr  doCopyAResource(ResType resourceType,SInt16 resourceID,SInt16 sourceFileRefNum,											SInt16 destFileRefNum){	Handle	sourceResourceHdl;	Str255	sourceResourceName;	ResType	ignoredType;	SInt16	ignoredID;	UseResFile(sourceFileRefNum);	sourceResourceHdl = GetResource(resourceType,resourceID);	if(sourceResourceHdl != NULL)	{		GetResInfo(sourceResourceHdl,&ignoredID,&ignoredType,sourceResourceName);		DetachResource(sourceResourceHdl);		UseResFile(destFileRefNum);		AddResource(sourceResourceHdl,resourceType,resourceID,sourceResourceName);		if(ResError() == noErr)			UpdateResFile(destFileRefNum);	}	ReleaseResource(sourceResourceHdl);	return ResError();}// *******************************************************************************************