/******************************************************************************	CListingDocument.c		Methods for the listing files.	Copyright (C) 1985-1992  New York University	Copyright (C) 1994 George Washington University	 	This file is part of the GWAdaEd system, an extension of the Ada/Ed-C	system.  See the Ada/Ed README file for warranty (none) and distribution	info and also the GNU General Public License for more details. ******************************************************************************/#include <Global.h>#include <Commands.h>#include <assert.h>#include <CApplication.h>#include <CBartender.h>#include <CDesktop.h>#include <CDecorator.h>#include <CDataFile.h>#include <CStaticText.h>#include <CPaneBorder.h>#include "CListingDocument.h"#include "CListingPane.h"#include "CSourceDocument.h"#include "CMenuWindow.h"#include "CAdaApp.h"#include "AppCommands.h"#ifdef useCPEdit#include "CPEditScrollPane.h"#endifextern	CApplication *gApplication;	/* The application */extern	CBartender	*gBartender;	/* The menu handling object */extern	CDesktop	*gDesktop;		/* The enclosure for all windows */extern	CDecorator	*gDecorator;	/* Window dressing object	*//**** Font Defaults ****/#define kDefaultFont	monaco#define kDefaultSize	9/*** * IListingDirector * ***/ void CListingDocument::IListingDocument(CSourceDocument *source,		long numErrors, Msg *ptr){	IDocument(gApplication, true);	fSource = source;	itsNumErrors = numErrors;	itsCurrMsg = -1;	itsErrors = ptr;}/**** *	Dispose * ****/void	CListingDocument::Dispose(void){	if (itsErrors) {		free_msg(itsNumErrors, itsErrors);		itsNumErrors = -1;		itsErrors = NULL;	}	if (fSource) {		fSource->ListingClosed();		fSource = NULL;	}	inherited::Dispose();}/*** * OpenFile * ***/void CListingDocument::OpenFile(SFReply *macSFReply){	CDataFile	*theFile;	Handle		theData;	Str63		theName;	OSErr		theError;	theFile = new(CDataFile);	theFile->IDataFile();	theFile->SFSpecify(macSFReply);		theFile->Open(fsRdWrPerm);#ifndef useCPEdit	if (theFile->GetLength() > 32000L) {		ParamText( "\pCan't open a file bigger than 32K.", "\p", "\p", "\p");		PositionDialog('ALRT', 128);		InitCursor();		Alert(128, NULL);		theFile->Dispose();		Dispose();		return;	}#endif	    theData = theFile->ReadAll();     /* ReadAll() creates the handle */	BuildWindow(theData);	DisposHandle(theData);	theFile->GetName(theName);	itsFile = theFile;	itsWindow->SetTitle(theName);	itsWindow->Select();	/* Don't forget to make the window active */}/**** *	BuildWindow * ****/void	CListingDocument::BuildWindow (Handle theData){	Str255			messageStr;	itsWindow = new(CMenuWindow);	itsWindow->IWindow(WINDculture, FALSE, gDesktop, this);	BuildWindowContents();	if (theData) {		long i, size;		long start, end;		Boolean foundNL;		/* CAREFUL: This knows the secret of the last line of the		 * listing file.  The last line always reads:		 *		"  error message\n"		 * To find number of errors, just parse backwards and get		 * the last line		 */		start = -1;		end = GetHandleSize(theData);		foundNL = false;		for (size = end-1; size > 0; size--) {			if (((*theData)[size] == '\r') ||				((*theData)[size] == '\n')) {				if (!foundNL)					end = size;				else {					start = size;					break;				}				foundNL = true;			}		}		start++;		// skip the new lines		end--;		// Skip over initial blanks and tabs		for (i = 1; start <= end; start++)			if (((*theData)[start] != ' ') &&				((*theData)[start] != '\t'))				break;		// Copy what's left in the string		for (i = 1; start <= end; start++)			messageStr[i++] = (*theData)[start];		messageStr[0] = i-1;		fTextEdit->SetTextHandle(theData);		fMessage->SetTextString(messageStr);	}	fTextEdit->SetSelection(0, 0, false);	fTextEdit->ScrollToSelection();	gDecorator->PlaceNewWindow(itsWindow);}/*** *	BuildWindowContents * *	Separated from BuildWindow so that subclass can create *	different configurations of the contents of the window * ***/void	CListingDocument::BuildWindowContents(void){#ifdef useCPEdit	CPEditScrollPane	*thePScrollPane;#else	CScrollPane		*thePScrollPane;#endif	CTextPane		*theMainPane;	LongPt			pt;	CPaneBorder		*theBorder;	Rect			 margin;#define kWindowWidth 240#define kWindowHeight 200#define kLeftEdge		4#define kMessageLeft kLeftEdge#define kMessageHeight 16#define kMessageTop		kLeftEdge#define kSourceWidth	(kWindowWidth-kLeftEdge)#define kSourceHeight	(kWindowHeight-kSourceTop)#define kSourceLeft		kLeftEdge#define kSourceTop		(2*kMessageTop+kMessageHeight)	/*------------------------------------*/	/* Message Text Field */	fMessage = new CStaticText;	fMessage->IStaticText(itsWindow, this, 		kWindowWidth - kMessageLeft, kMessageHeight,		kMessageLeft, kMessageTop,		sizELASTIC, sizFIXEDSTICKY, -1);	fMessage->SetFontNumber(kDefaultFont);	//fMessage->SetFontStyle(bold);	fMessage->SetFontSize(kDefaultSize);	/*------------------------------------*/	/* Source Display */#ifdef useCPEdit	thePScrollPane = new(CPEditScrollPane);#else	thePScrollPane = new(CScrollPane);#endif	thePScrollPane->IScrollPane(itsWindow, this,		kSourceWidth+1, kSourceHeight+1,		kSourceLeft,  kSourceTop,		sizELASTIC, sizELASTIC, TRUE, TRUE, TRUE);	theBorder = new CPaneBorder;	theBorder->IPaneBorder(kBorderFrame);	SetRect(&margin, 0, 0, 0, 0);	margin.right = -1;	margin.top = 1;	margin.bottom = -1;	theBorder->SetMargin(&margin);	thePScrollPane->SetBorder(theBorder);	fTextEdit = new(CListingPane);	fTextEdit->IListingPane(thePScrollPane, this,		kSourceWidth-SBARSIZE-2, kSourceHeight-SBARSIZE-2, 1, 1,		sizELASTIC, sizELASTIC);	// set the font/size into the text edit field	fTextEdit->SetFontNumber(kDefaultFont);	fTextEdit->SetFontSize(kDefaultSize);	// Set non-editable, non-selectable	fTextEdit->Specify(kNotEditable, kSelectable, kNotStylable);	itsMainPane = fTextEdit;	itsGopher = fTextEdit;	thePScrollPane->InstallPanorama(fTextEdit);	SetLongPt(&pt, 0, 0);	fTextEdit->ScrollTo(&pt, false);}/**** *	CanCmdNext * *	Can we do a Next command? * ****/Boolean		CListingDocument::CanCmdNext(void){	return (fSource && itsErrors && itsNumErrors > 0 &&			itsCurrMsg+1 < itsNumErrors);}/**** *	CanCmdPrev * *	Can we do a Previous command? * ****/Boolean		CListingDocument::CanCmdPrev(void){	return (fSource && itsErrors && itsNumErrors > 0 &&		itsCurrMsg > 0);}/**** *	UpdateMenus * *	Set menu state based on current context of this document * ****/void	CListingDocument::UpdateMenus(void){	inherited::UpdateMenus();	gBartender->DisableCmd(cmdGotoLineInSource);	if (fSource) {		long selStart;		long selEnd;		fTextEdit->GetSelection(&selStart, &selEnd);		if (selStart < selEnd)			gBartender->EnableCmd(cmdGotoLineInSource);	}				if (CanCmdNext())		gBartender->EnableCmd(cmdNextError);	else		gBartender->DisableCmd(cmdNextError);	if (CanCmdPrev())		gBartender->EnableCmd(cmdPreviousError);	else		gBartender->DisableCmd(cmdPreviousError);	gBartender->EnableCmd(cmdPageSetup);	gBartender->EnableCmd(cmdPrint);}/*** * DoCommand * *	Handle Previous/Next error commands * ***/ void	CListingDocument::DoCommand(long theCommand){	switch (theCommand) {		case cmdGotoLineInSource: {			long selStart;			long selEnd;			assert(fSource);			fTextEdit->GetSelection(&selStart, &selEnd);			/* Parse the selection to find out line number */			if (selStart < selEnd) {				Str255 number;				Boolean foundOne = false;				Handle text;				text = fTextEdit->GetTextHandle();				selStart--;				if (((*text)[selStart] == '\r') ||					((*text)[selStart] == '\n')) {					int i;					for (i = 1; selStart < selEnd; selStart++) {						if (((*text)[selStart] >= '0') &&							((*text)[selStart] <= '9')) {							number[i++] = (*text)[selStart];							foundOne = true;						}						else if (foundOne) {							long  value;							number[0] = i-1;							StringToNum(number, &value);							fSource->SelectLine(value);							fSource->ScrollToSelection();							fSource->Select();							break;						}					}				}			}			break;		}		case cmdPreviousError: {			Msg cm;		// current message			if (itsErrors) {				itsCurrMsg--;				if (itsCurrMsg < 0)					itsCurrMsg = 0;				cm = itsErrors[itsCurrMsg];				if (fSource) {					fSource->SelectFT(cm->msg_line1, cm->msg_col1,								   cm->msg_line2, cm->msg_col2);					fSource->ScrollToSelection();						// Select the window associated with this					fSource->Select();				}				fTextEdit->HilightLines(cm->msg_lineno1,					cm->msg_lineno2);			}			break;		}		case cmdNextError: {			Msg cm;			long line;						if (itsErrors) {				itsCurrMsg++;				if (itsCurrMsg >= itsNumErrors)					itsCurrMsg = itsNumErrors - 1;				cm = itsErrors[itsCurrMsg];					if (fSource) {					fSource->SelectFT(cm->msg_line1, cm->msg_col1,								   cm->msg_line2, cm->msg_col2);					fSource->ScrollToSelection();						// Select the window associated with this					fSource->Select();				}				fTextEdit->HilightLines(cm->msg_lineno1,						cm->msg_lineno2);			}			break;		}		default:			inherited::DoCommand(theCommand);			break;	}}/**** *	GetNumErrors * *	Return instance variable 'itsNumErrors' * ****/long	CListingDocument::GetNumErrors(void){	return itsNumErrors;}/**** *	GetNextError * *	An attempt to make the listing work independently of the *	structure built by the msg_parser.  This code is NOT used *	and has NOT been tested. * ****/void	CListingDocument::GetNextError(long *fline, long *fcol,						 long *tline, long *tcol){	long selStart;	long selEnd;	Handle text;	long iOffset, offset, findLen, rplcLen;	Ptr findStr, rplcStr;	fTextEdit->GetSelection(&selStart, &selEnd);	if (selStart == fTextEdit->GetLength())		selStart = selEnd = 0;	findStr = "***";	findLen = 3;	rplcStr = NULL;	rplcLen = 0;	text = fTextEdit->GetTextHandle();	iOffset = Munger(text, selEnd, findStr, findLen,						rplcStr, rplcLen);}/**** *	GetPrevError * *	See comments above * ****/void	CListingDocument::GetPrevError(long *fline, long *fcol,						 long *tline, long *tcol){}