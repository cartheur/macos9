// *******************************************************************************************// CarbonPrinting.h                                                        CLASSIC EVENT MODEL// *******************************************************************************************// // This program://// ¥	Demonstrates printing using the Carbon Printing Manager session functions. //// ¥	Opens two windows. The first window is a document window in which is displayed the//		document to be printed.  The second window displays some printing-related information//		obtained from PMPageFormat and PMPrintSettings objects.//// ¥	Customises the Pring dialog by adding a pop-up menu button, three radio buttons, a //		checkbox, and a group box. //// ¥	Allows the user to print a document containing a picture and text, with the text//		being printed in the font and font size, and with the fractional widths setting, //		specified using the items added to the Print dialog.//// The customising of the Print dialog uses the AppendDITL method.  Accordingly, on Mac OS X,// the dialogs are application-modal and are not displayed as window-modal sheet dialogs.//// The program utilises the following resources://// ¥	A 'plst' resource.//// ¥	'MBAR' resource and associated 'MENU' resources (preload, non-purgeable).//// ¥	Two 'WIND' resources (purgeable).//// ¥	A 'TEXT' resource (purgeable) used for printing.//// ¥	A 'PICT' resource (non-purgeable) used for printing.//// ¥	'CNTL' resources (purgeable) for controls added to the Print dialog box.//// ¥	A 'DITL' resource (purgeable) specifying the items to be appended to the Print dialog//		box.//// ¥	A 'MENU' resource (preload, non-purgeable) for the pop-up menu button.//// ¥	A 'SIZE' resource with the acceptSuspendResumeEvents, canBackground, //		doesActivateOnFGSwitch, and isHighLevelEventAware flags set.//// *******************************************************************************************// ÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉ includes#include <Carbon.h>// ÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉ defines#define rMenubar								128#define mAppleApplication				128#define  iAbout									1#define mFile										129#define  iPageSetup							9#define  iPrint									10#define  iQuit									12#define mFont										131#define rDocWindow							128#define rInfoWindow							129#define rText										128#define rPicture								128#define rPrintDialogAppendDITL	128#define  iPopupButton						1#define  iRadioButton10pt				2#define  iRadioButton12pt				3#define  iRadioButton14pt				4#define  iCheckboxFracWidths		5#define kMargin									90#define MAX_UINT32							0xFFFFFFFF#define MIN(a,b) 								((a) < (b) ? (a) : (b))// ÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉ typedefstypedef struct{	PMPrintSession	printSession;	PMPageFormat		pageFormat;	PMPrintSettings	printSettings;	TEHandle				editTextStrucHdl;	PicHandle				pictureHdl;} docStructure, *docStructurePtr, **docStructureHdl;// ÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉÉ function prototypesvoid			main													(void);void			doPreliminaries								(void);OSErr			quitAppEventHandler						(AppleEvent *,AppleEvent *,SInt32);void			doGetDocument									(void);void			doEvents											(EventRecord *);void			doUpdate											(EventRecord *);void			doUpdateDocumentWindow				(WindowRef);void			doMenuChoice									(SInt32);OSStatus	doCreateOrValidatePageFormat	(WindowRef);OSStatus	doPageSetUpDialog							(WindowRef);OSStatus	doPrintSettingsDialog					(WindowRef);OSStatus	doPrinting										(WindowRef);SInt16		doCalcNumberOfPages						(WindowRef,Rect);void			doDrawPage										(WindowRef,Rect,SInt16,SInt16);void			doDrawPrintInfo								(void);void			doDrawRectStrings							(Str255,SInt16,SInt16,Str255,SInt16,SInt16,Str255);void			doErrorAlert									(OSStatus);void			doConcatPStrings							(Str255,Str255);void			initialisationFunction				(PMPrintSettings, PMDialog *);void			itemEvaluationFunction				(DialogPtr,SInt16);Boolean		eventFilter										(DialogPtr,EventRecord *,SInt16 *);// *******************************************************************************************