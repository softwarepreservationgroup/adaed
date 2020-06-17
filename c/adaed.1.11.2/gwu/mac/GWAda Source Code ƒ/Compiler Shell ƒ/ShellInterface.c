/***** * ShellInterface.c * *	Routines for the window in the compiler-parts * *****//* * Copyright (C) 1985-1992  New York University * Copyright (C) 1994 George Washington University *  * This file is part of the GWAdaEd system, an extension of the Ada/Ed-C * system.  See the Ada/Ed README file for warranty (none) and distribution * info and also the GNU General Public License for more details. */#include "ShellInterface.h"#include "ShellGlobals.h"#include "DrawTextColumn.h"#include "AdaFileTypes.h"#include "SpinCursor.h"#include "VersStr.h"#include "PStrUtilities.h"#include <string.h>#include <stdio.h>extern	FILE 	*errfile;void DrawProcess(short lh, Str255 label, Str255 path);enum	{	appleID = 1,	fileID	};enum	{	quitItem = 1	};#define colTwo 82#define border 5#define progressBarH (lineHeight-8)#define kWindowWidInches 6.0// Controls what lines will be drawn in the progress dlog// (negative means don't draw).static	short lnProcName = 1;static	short lnVersion = 2;static	short lnHeap = 3;static	short lnOptions = 4;static	short lnEnvName = -1;#define kTotalLines 5static	short lnNumLines;		// number of lines to usestatic	short lnProgress;		// this is always the last onestatic	short lineHeight;		// ascent + descent + leadingstatic	short lnOffsetTop;		// offset from baseline to top of linestatic	short lnOffsetBottom;	// offset from baseline to bottom of line/**** * SetUpMenus() * *	Set up the menus. Normally, we�d use a resource file, but *	for this example we�ll supply �hardwired� strings. * ****/void SetUpMenus(void){	InsertMenu(gAppleMenu = NewMenu(appleID, "\p\024"), 0);	InsertMenu(gFileMenu = NewMenu(fileID, "\pFile"), 0);	DrawMenuBar();	AddResMenu(gAppleMenu, 'DRVR');	AppendMenu(gFileMenu, "\pQuit/Q");}/**** *  AdjustMenus() ****/	static void enable(MenuHandle menu, short item, short ok)	{		if (ok)			EnableItem(menu, item);		else			DisableItem(menu, item);	}void AdjustMenus(void){	enable(gFileMenu, quitItem, 1);}/***** * HandleMenu(mSelect) * *	Handle the menu selection. mSelect is what MenuSelect() and *	MenuKey() return: the high word is the menu ID, the low word *	is the menu item * *****/void HandleMenu (long mSelect){	int			menuID = HiWord(mSelect);	int			menuItem = LoWord(mSelect);	Str255		name;	GrafPtr		savePort;	WindowPeek	frontWindow;		switch (menuID)	  {	  case	appleID:		GetPort(&savePort);		GetItem(gAppleMenu, menuItem, name);		OpenDeskAcc(name);		SetPort(savePort);		break;		  case	fileID:		switch (menuItem)		  {		  case	quitItem:			gShellQuitting = true;			break;		  }		break;			  }}/* end HandleMenu *//***** * The window routines *****//**** * SetUpWindow() * *	Create the progress window, and open it. * ****/void SetUpWindow(Boolean useProgress){Rect	windowBounds;short	v;short	wid;short	hppi, vppi;Str255  shellName;FontInfo	finfo;short	lineDots;	if (useProgress) {		lnNumLines = kTotalLines;		lnProgress = kTotalLines;	}	else {		lnNumLines = kTotalLines - 1;		lnProgress = -1;	}	ScreenRes(&hppi, &vppi);	wid = (screenBits.bounds.right - screenBits.bounds.left);	if (wid * hppi > kWindowWidInches)	// keep a limit for window width		wid = kWindowWidInches * hppi;	lineHeight = 14;		// guessing	SetRect(&windowBounds, 20, 40, wid, 40 + (lineHeight * (lnNumLines+1)));	gShellDragRect = screenBits.bounds;	GetPartName(shellName);	gShellWindow = NewWindow(0L, &windowBounds, shellName, false, // invisible at first		 noGrowDocProc, (WindowPtr) -1L, false, 0);	SetPort(gShellWindow);		// set font to use�	TextFont(geneva);	TextSize(9);	GetFontInfo(&finfo);		// and calculate line info	lineDots = finfo.ascent + finfo.descent + finfo.leading;	lnOffsetTop = finfo.ascent;	lnOffsetBottom = finfo.descent + finfo.leading;	SizeWindow(gShellWindow, wid, (lineHeight * (lnNumLines+1)), false);	ShowWindow(gShellWindow);	if (lnProgress > 0) {		v = (lineHeight * lnProgress) - progressBarH;		SetRect(&gProgressBar, colTwo+4, v, colTwo + 104, v + progressBarH);		gProgressArea = gProgressBar;		InsetRect(&gProgressArea, -2, -2);	}}/***** * DrawWContents() * *	Draws the window content * *****/  	static void CalcLineBox(short lh, Rect *r) 	{ 		r->left = border; 		r->right = gShellWindow->portRect.right; 		r->top = lh - lnOffsetTop; 		r->bottom = lh + lnOffsetBottom; 	}  	static void EraseLineBox(short lh) 	{ 		Rect r; 		 		CalcLineBox(lh, &r); 		EraseRect(&r); 	} 	 	static void DrawMemory(void) 	{		Str255 num1; 		Rect r; 		Point pt;		short columnWidth; 		if (lnHeap > 0) { 			static long freeMem = -1; 			 			if (FreeMem() != freeMem) { 				freeMem = FreeMem();				NumToString(freeMem, num1);		 		CalcLineBox(lnHeap * lineHeight, &r);		 		r.left = colTwo;		 		EraseRect(&r);				MoveTo(colTwo, lnHeap * lineHeight);				DrawString("\p: ");					GetPen(&pt);				columnWidth = gShellWindow->portRect.right - pt.h - border;				DrawStringColumn(num1, columnWidth);			}		}	} 	static void DrawProcess(short lh, Str255 label, Str255 path) 	{ 		Point pt;		short columnWidth;		if (lh < 0)		// negative means don't draw this one			return; 		MoveTo(border, lh);		TextFace(bold);			DrawString(label);		TextFace(normal);		GetPen(&pt);		MoveTo(colTwo, pt.v);		DrawString("\p: ");		GetPen(&pt);		columnWidth = gShellWindow->portRect.right - pt.h - border;		DrawStringColumn(path, columnWidth);	}void DrawWContents(short active){short i;Point pt;Str255 version;Str255 partName, partPath;	SetPort(gShellWindow);	GetVersStr(version);	GetPartName(partName);	GetCompilerPath(partPath); 	DrawProcess(lnProcName * lineHeight, "\pProcess", partName); 	DrawProcess(lnVersion * lineHeight, "\pVersion", (unsigned char *)version); 	//DrawProcess(lnEnvName * lineHeight, "\pCalled by", partName); 	DrawProcess(lnOptions * lineHeight, "\pOptions", (unsigned char *)gCmdLineOptions);	DrawProcess(lnHeap * lineHeight, "\pFree Heap", "\p");	// if processing then draw the grow bar stuff	if (lnProgress > 0) {		if (gProcessing) {			MoveTo(border, lnProgress * lineHeight);			TextFace(bold);	DrawString("\pProgress");	TextFace(normal);			FrameRect(&gProgressArea);		}	}}	/* end DrawWContents *//**** * ProgressBar() * *		This routine updates the progress bar. * *****/void ProgressBar(short progress){	// If we are still doing some processing, then show some	// kind of progress indicator.	if ((lnProgress > 0) && (gProcessing)) {		Rect area;		// update indicator here		SetPort(gShellWindow);		area = gProgressBar;		if (progress < 100)			area.right = area.left + (area.right - area.left) * (progress/100.0);		FillRect(&area, gray);	}	// if we want to show memory information, then draw free mem and	// free stack space	DrawMemory();}/**** *	InternalError() * * Display alert message, write error results * to results file, and call the longjmp.  This routine never * returns! * ****/void InternalError(ErrMsgs msgs, int errCode, char *file, int line){	Str255 str[4];	short i;	// Setup parameters	for (i = 0; i < 4; i++) {		if (msgs[i].id > 0)			GetIndString(str[i], 20000, msgs[i].id);		else if (msgs[i].msg == NULL)			str[i][0] = 0;			// NULL Pascal string		else {			short len = strlen(msgs[i].msg);			BlockMove(msgs[i].msg, &str[i][1], len);			str[i][0] = len;		}				}	// write error message to file	if (errfile) {		Str255 partName;		Str255 version;		GetVersStr(version);		GetPartName(partName);		fprintf(errfile, "In %#s version %#s\n", partName, version);		fprintf(errfile, "Chaos: in %s, line %d\n", file, line);		fprintf(errfile, "Error: %#s%#s%#s%#s\n\n", str[0], str[1], str[2], str[3]);		fflush(errfile);		fclose(errfile);	}	// Show error message.	ParamText(str[0], str[1], str[2], str[3]);	StopSpinning();	StopAlert(20000, NULL);	// jump back to the main program passing whatever return	// code we have here!	longjmp(gJumpEnv, errCode+10);		// add 10, see note in ShellMain.c}/**** *	ShowMessage * *	Display a message using an alert box * ****/void ShowMessage(char *msg){	// write error message to file	if (errfile) {		fprintf(errfile, "%s\n", msg);		fflush(errfile);	}	// Show error message.	ParamText(CtoPstr(msg), "\p", "\p", "\p");	StopSpinning();	StopAlert(20000, NULL);	PtoCstr((unsigned char *)msg);}/**** *	AbortMessage * *	Display a message using ShowMessage and then quit. *	This routine doesn't return to its caller. * ****/void AbortMessage(char *msg, int errCode){	// write error message to file	if (errfile) {		fprintf(errfile, "Aborting: \n");		fflush(errfile);		fclose(errfile);	}	ShowMessage(msg);	// jump back to the main program passing whatever return	// code we have here!	longjmp(gJumpEnv, errCode+10);		// add 10, see note in ShellMain.c}