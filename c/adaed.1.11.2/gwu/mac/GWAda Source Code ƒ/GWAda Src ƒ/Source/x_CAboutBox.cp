/****************************************************************************** x_CAboutBox.c				CAboutBox Dialog Director Class		Copyright (C) 1985-1992  New York University	Copyright (C) 1994 George Washington University	 	This file is part of the GWAdaEd system, an extension of the Ada/Ed-C	system.  See the Ada/Ed README file for warranty (none) and distribution	info and also the GNU General Public License for more details.	Generated by Classy� (Object Factory)	This file is rewritten each time you generate code. You should not	make changes to this file; changes should go in the CAboutBox.c	file, instead.	If you want to change how Classy generates this file, you can	change the template for this file. It is "_Dialog.c" in the Classy	Templates folder. ******************************************************************************/#include "x_CAboutBox.h"#include "AboutBoxItems.h"#include "ViewUtilities.h"#include <CIntegerText.h>#include <Commands.h>#include <CControl.h>#include <CPopup.h>#include <CPopupPane.h>#include <CArrayPane.h>#include <CIconButton.h>#include <CPictureButton.h>#include <CSwissArmyButton.h>#include "CButtonProc.h"#include "CStaticText.h"#include "CPicture.h"#include <CApplication.h>#include <CBartender.h>#include <Commands.h>#include <Constants.h>#include <CDecorator.h>#include <CDesktop.h>#include <CDirectorOwner.h>#include <CFile.h>#include <CList.h>#include <CPanorama.h>#include <TBUtilities.h>#include <CWindow.h>extern	CApplication *gApplication;		/* The application					*/extern CDecorator	*gDecorator;		/* Decorator for arranging windows	*/extern CDesktop		*gDesktop;			/* The visible Desktop				*/extern CBartender	*gBartender;		/* Manages all menus				*/	// Define symbols for commands handled by this class	// Prevents a recompile every time any command changed.#define cmdCredits 559/**** C O N S T R U C T I O N / D E S T R U C T I O N   M E T H O D S ****//****************************************************************************** Ix_CAboutBox	Initialize the dialog ******************************************************************************/void x_CAboutBox::Ix_CAboutBox(CDirectorOwner *aSupervisor){	IDialogDirector(aSupervisor);		// There are several circumstances where we don't want		// ProviderChanged to be called. During initialization,		// during calls to UpdateData, etc. The ignore flag		// heads these off.	ignore = TRUE;						/* Don't call UpdateData now		*/	MakeNewWindow();					/* Create the dialog's window		*/	DoBeginData();						/* Gather initial values			*/	ignore = FALSE;}/****************************************************************************** MakeNewWindow	Create a window by reading a view resource ******************************************************************************/void x_CAboutBox::MakeNewWindow(void){	itsWindow = CLGetNamedWindow("\pAboutBox", this);		// Initialize pointers to the subpanes in the window  fAboutBox_Butn1 = (CButtonProc*) FindPane(kAboutBox_Butn1ID);  ASSERT(member(fAboutBox_Butn1, CButtonProc));  fAboutBox_Stat2 = (CStaticText*) FindPane(kAboutBox_Stat2ID);  ASSERT(member(fAboutBox_Stat2, CStaticText));  fAboutBox_Pict3 = (CPicture*) FindPane(kAboutBox_Pict3ID);  ASSERT(member(fAboutBox_Pict3, CPicture));  fAboutBox_Butn4 = (CButtonProc*) FindPane(kAboutBox_Butn4ID);  ASSERT(member(fAboutBox_Butn4, CButtonProc));}/****************************************************************************** FindPane	Locate a subpane of this window by ID. Note that Classy-	generated IDs are unique within a project, so this function	will find panes within subviews, even if dynamically loaded. ******************************************************************************/CPane *x_CAboutBox::FindPane(long ID){	return (CPane*) itsWindow->FindViewByID(ID);}/****************************************************************************** DoBeginData  	Collect initial values from subclass and initialize panes******************************************************************************/void x_CAboutBox::DoBeginData(){	CAboutBoxData		data = {0};		/* The initial value record			*/	BeginData(&data);					/* Ask subclass for initial values	*/		// Initialize the panes based on the values supplied.		// The ASSERT statements ensure that the generated		// code is in synch with the view resource.		// Save the initial values in case user cancels	saveData = data;}/****************************************************************************** DoEndData  	Collect final values from panes and tell subclass******************************************************************************/void x_CAboutBox::DoEndData(long theCommand){	CAboutBoxData		data;			/* The initial value record			*/	RgnHandle			rgn;			/* Selection region					*/										/* If user canceled the dialog,		*/	if (theCommand == cmdCancel)		/*	 return the initial values		*/	{		data = saveData;		EndData(&data);		return;	}										/* Collect values from panes		*/	EndData(&data);}/****************************************************************************** BeginData  	Collect initial values from subclass and initialize panes******************************************************************************/void x_CAboutBox::BeginData(CAboutBoxData *initial){}/****************************************************************************** UpdateData  	Tell subclass when panes change******************************************************************************/void x_CAboutBox::UpdateData(CAboutBoxUpdate *update, long itemNo){}/****************************************************************************** EndData  	Tell subclass the final values. For a canceled modal dialog, 	these are the same as the initial values.******************************************************************************/void x_CAboutBox::EndData(CAboutBoxData *final){}/****************************************************************************** DoModalDialog	{OVERRIDE}  	Override to call EndData******************************************************************************/long x_CAboutBox::DoModalDialog(long defaultCmd){	long	result = CDialogDirector::DoModalDialog(defaultCmd);		DoEndData(result);	return result;}/****************************************************************************** Close {OVERRIDE}  	Override to call EndData******************************************************************************/Boolean x_CAboutBox::Close(Boolean quitting){		if (itsWindow && !itsWindow->active)		itsWindow->Select();	if (EndDialog(cmdOK, TRUE))	{																				/* For a modal dialog, exit through	*/		if (itsWindow->IsModal())		/*	 DoModalDialog					*/		{			dismissCmd = cmdClose;			return TRUE;		}		else		{										/* Collect final values and call	*/			DoEndData(cmdOK);			/*	 EndData						*/										/* Do the Close						*/			return CDirector::Close(quitting);		}	}	else		return FALSE;}/****************************************************************************** ProviderChanged	{OVERRIDE} 	 	User did something. Note that the TCL 	does not currently report changes to plain CEditText items. ******************************************************************************/void x_CAboutBox::ProviderChanged(CCollaborator *aProvider, long reason, void* info){	CAboutBoxUpdate	data;			/* The update value record			*/	Str255	str;	Boolean	saveIgnore = ignore;		if (ignore)							/* Don't be a chatterbox			*/		return;	ignore = TRUE;		TRY	{ 		if (FALSE) {}		else			CDialogDirector::ProviderChanged(aProvider, reason, info);	}	CATCH		ignore = saveIgnore;	ENDTRY		ignore = saveIgnore;				/* ProviderChanged() can't Close()!	*/}/****************************************************************************** DoCommand {OVERRIDE}		Dispatch Classy-specified actions. ******************************************************************************/void x_CAboutBox::DoCommand(long theCommand){	switch (theCommand)	{		case cmdCredits:			DoCmdCredits();			break;		default:			CDialogDirector::DoCommand(theCommand);	}}/****************************************************************************** UpdateMenus {OVERRIDE}		Enable menus which generate commands handled by this class. ******************************************************************************/void x_CAboutBox::UpdateMenus(){	CDialogDirector::UpdateMenus();	gBartender->EnableCmd(cmdCredits);}/****************************************************************************** DoCmdCredits		Respond to cmdCredits command. ******************************************************************************/void x_CAboutBox::DoCmdCredits(){  		// Subclass must override this function to  		// handle the command}