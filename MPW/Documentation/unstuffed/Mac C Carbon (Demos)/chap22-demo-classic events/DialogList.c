// *******************************************************************************************// DialogLists.c// *******************************************************************************************// ииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииии includes#include "Lists.h"// ***************************************************************************** doListsDialogvoid  doListsDialog(void){	DialogPtr				dialogPtr;	GrafPtr					oldPort;	ModalFilterUPP	eventFilterUPP;	ControlRef			dateFormatControlRef, watermarkControlRef, controlRef;	ListHandle			dateFormatListHdl, watermarkListHdl;	SInt16					itemHit;	Cell						theCell;	SInt16					dataLen, offset;	Str255					dateFormatString, watermarkString;	Boolean					wasDoubleClick = false;	// ииииииииииииииииииииииииии explicitly deactivate front window if it exists, create dialog	if(FrontWindow())		doActivateWindow(FrontWindow(),false);	if(!(dialogPtr = GetNewDialog(rListsDialog,NULL,(WindowRef) -1)))		ExitToShell();	GetPort(&oldPort);	SetPortDialogPort(dialogPtr);	// иииииииииииииииииииииииииииииииииииииииииииииииииииии set default and cancel push buttons	SetDialogDefaultItem(dialogPtr,kStdOkItemIndex);	SetDialogCancelItem(dialogPtr,kStdCancelItemIndex);	// ииииииииииииииииииииииииииии create universal procedure pointer for event filter function	eventFilterUPP = NewModalFilterUPP((ModalFilterProcPtr) eventFilter);	// иииииииииии add rows to lists, store data in their cells, modify cell selection algorithm	GetDialogItemAsControl(dialogPtr,iDateFormatList,&dateFormatControlRef);	GetControlData(dateFormatControlRef,kControlEntireControl,kControlListBoxListHandleTag,								 sizeof(dateFormatListHdl),&dateFormatListHdl,NULL); 	doAddRowsAndDataToTextList(dateFormatListHdl,rDateFormatStrings,17);	SetListSelectionFlags(dateFormatListHdl,lOnlyOne);	SetPt(&theCell,0,0);	LSetSelect(true,theCell,dateFormatListHdl);	GetDialogItemAsControl(dialogPtr,iWatermarkList,&watermarkControlRef);	GetControlData(watermarkControlRef,kControlEntireControl,kControlListBoxListHandleTag,								 sizeof(watermarkListHdl),&watermarkListHdl,NULL);	doAddRowsAndDataToTextList(watermarkListHdl,rWatermarkStrings,12);	SetListSelectionFlags(watermarkListHdl,lOnlyOne);	SetPt(&theCell,0,0);	LSetSelect(true,theCell,watermarkListHdl);	// ииииииииииииииииииииииииииииииииииииииииииииииииииииии show dialog and set keyboard focus	ShowWindow(GetDialogWindow(dialogPtr));	SetKeyboardFocus(GetDialogWindow(dialogPtr),watermarkControlRef,1);	SetKeyboardFocus(GetDialogWindow(dialogPtr),dateFormatControlRef,1);	// ииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииии enter ModalDialog loop	do	{		ModalDialog(eventFilterUPP,&itemHit);		if(itemHit == iDateFormatList)		{			SetPt(&theCell,0,0);			LGetSelect(true,&theCell,dateFormatListHdl);			LGetCellDataLocation(&offset,&dataLen,theCell,dateFormatListHdl);			LGetCell(dateFormatString + 1,&dataLen,theCell,dateFormatListHdl);			dateFormatString[0] = (SInt8) dataLen;			GetDialogItemAsControl(dialogPtr,iDateFormatStaticText,&controlRef);			SetControlData(controlRef,kControlEntireControl,kControlStaticTextTextTag,										 dateFormatString[0],&dateFormatString[1]);			Draw1Control(controlRef);			GetControlData(dateFormatControlRef,kControlEntireControl,										 kControlListBoxDoubleClickTag,sizeof(wasDoubleClick),&wasDoubleClick,										 NULL);		}		else if(itemHit == iWatermarkList)		{			SetPt(&theCell,0,0);			LGetSelect(true,&theCell,watermarkListHdl);			LGetCellDataLocation(&offset,&dataLen,theCell,watermarkListHdl);			LGetCell(watermarkString + 1,&dataLen,theCell,watermarkListHdl);			watermarkString[0] = (SInt8) dataLen;			GetDialogItemAsControl(dialogPtr,iWatermarkStaticText,&controlRef);			SetControlData(controlRef,kControlEntireControl,kControlStaticTextTextTag,										 watermarkString[0],&watermarkString[1]);			Draw1Control(controlRef);			GetControlData(watermarkControlRef,kControlEntireControl,										 kControlListBoxDoubleClickTag,sizeof(wasDoubleClick),&wasDoubleClick,										 NULL);		}	} while(itemHit != kStdOkItemIndex && wasDoubleClick == false);	// ииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииииии clean up	DisposeDialog(dialogPtr);	DisposeModalFilterUPP(eventFilterUPP);	SetPort(oldPort);}// ******************************************************************************* eventFilterBoolean  eventFilter(DialogPtr dialogPtr,EventRecord *eventStrucPtr,SInt16 *itemHit){	Boolean			handledEvent;	GrafPtr			oldPort;	WindowRef		windowRef;	SInt8				charCode;	ControlRef	controlRef, focusControlRef;	ListHandle	watermarkListHdl;	handledEvent = false;	windowRef = GetDialogWindow(dialogPtr);		if((eventStrucPtr->what == updateEvt) && ((WindowRef) eventStrucPtr->message != windowRef))	{		doUpdate(eventStrucPtr);	}	else	{		GetPort(&oldPort);		SetPortDialogPort(dialogPtr);		if(eventStrucPtr->what == keyDown)		{			charCode = eventStrucPtr->message & charCodeMask;						if(charCode != kUpArrow && charCode != kDownArrow && charCode != kTab)			{				GetDialogItemAsControl(dialogPtr,iWatermarkList,&controlRef);				GetControlData(controlRef,kControlEntireControl,kControlListBoxListHandleTag,									 sizeof(watermarkListHdl),&watermarkListHdl,NULL);				GetKeyboardFocus(GetDialogWindow(dialogPtr),&focusControlRef);				if(controlRef == focusControlRef)				{					doTypeSelectSearch(watermarkListHdl,eventStrucPtr);					Draw1Control(controlRef);				}								handledEvent = true;			}		}		else		{			handledEvent = StdFilterProc(dialogPtr,eventStrucPtr,itemHit);		}		SetPort(oldPort);	}	return handledEvent;}// *******************************************************************************************