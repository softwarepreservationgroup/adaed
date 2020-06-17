/****************************************************************************** x_CCompilerOptions.c				CCompilerOptions Dialog Director Class		Copyright (C) 1985-1992  New York University	Copyright (C) 1994 George Washington University	 	This file is part of the GWAdaEd system, an extension of the Ada/Ed-C	system.  See the Ada/Ed README file for warranty (none) and distribution	info and also the GNU General Public License for more details.	Generated by Classy� (Object Factory)	This file is rewritten each time you generate code. You should not	make changes to this file; changes should go in the CCompilerOptions.c	file, instead.	If you want to change how Classy generates this file, you can	change the template for this file. It is "_Dialog.c" in the Classy	Templates folder. ******************************************************************************/#include "x_CCompilerOptions.h"#include "CompilerOptionsItems.h"#include "ViewUtilities.h"#include <CIntegerText.h>#include <Commands.h>#include <CControl.h>#include <CPopup.h>#include <CPopupPane.h>#include <CArrayPane.h>#include <CIconButton.h>#include <CPictureButton.h>#include <CSwissArmyButton.h>#include "CButtonProc.h"#include "CStaticText.h"#include "CRadioControlProc.h"#include <CApplication.h>#include <CBartender.h>#include <Commands.h>#include <Constants.h>#include <CDecorator.h>#include <CDesktop.h>#include <CDirectorOwner.h>#include <CFile.h>#include <CList.h>#include <CPanorama.h>#include <TBUtilities.h>#include <CWindow.h>extern	CApplication *gApplication;		/* The application					*/extern CDecorator	*gDecorator;		/* Decorator for arranging windows	*/extern CDesktop		*gDesktop;			/* The visible Desktop				*/extern CBartender	*gBartender;		/* Manages all menus				*/	// Define symbols for commands handled by this class	// Prevents a recompile every time any command changed./**** C O N S T R U C T I O N / D E S T R U C T I O N   M E T H O D S ****//****************************************************************************** Ix_CCompilerOptions	Initialize the dialog ******************************************************************************/void x_CCompilerOptions::Ix_CCompilerOptions(CDirectorOwner *aSupervisor){	IDialogDirector(aSupervisor);		// There are several circumstances where we don't want		// ProviderChanged to be called. During initialization,		// during calls to UpdateData, etc. The ignore flag		// heads these off.	ignore = TRUE;						/* Don't call UpdateData now		*/	MakeNewWindow();					/* Create the dialog's window		*/	DoBeginData();						/* Gather initial values			*/	ignore = FALSE;}/****************************************************************************** MakeNewWindow	Create a window by reading a view resource ******************************************************************************/void x_CCompilerOptions::MakeNewWindow(void){	itsWindow = CLGetNamedWindow("\pCompilerOptions", this);		// Initialize pointers to the subpanes in the window  fCompilerOptions_Butn3 = (CButtonProc*) FindPane(kCompilerOptions_Butn3ID);  ASSERT(member(fCompilerOptions_Butn3, CButtonProc));  fCompilerOptions_Butn4 = (CButtonProc*) FindPane(kCompilerOptions_Butn4ID);  ASSERT(member(fCompilerOptions_Butn4, CButtonProc));  fCompilerOptions_Stat6 = (CStaticText*) FindPane(kCompilerOptions_Stat6ID);  ASSERT(member(fCompilerOptions_Stat6, CStaticText));  fCompilerOptions_Always = (CRadioControlProc*) FindPane(kCompilerOptions_AlwaysID);  ASSERT(member(fCompilerOptions_Always, CRadioControlProc));  fCompilerOptions_Errors = (CRadioControlProc*) FindPane(kCompilerOptions_ErrorsID);  ASSERT(member(fCompilerOptions_Errors, CRadioControlProc));}/****************************************************************************** FindPane	Locate a subpane of this window by ID. Note that Classy-	generated IDs are unique within a project, so this function	will find panes within subviews, even if dynamically loaded. ******************************************************************************/CPane *x_CCompilerOptions::FindPane(long ID){	return (CPane*) itsWindow->FindViewByID(ID);}/****************************************************************************** DoBeginData  	Collect initial values from subclass and initialize panes******************************************************************************/void x_CCompilerOptions::DoBeginData(){	CCompilerOptionsData		data = {0};		/* The initial value record			*/	BeginData(&data);					/* Ask subclass for initial values	*/		// Initialize the panes based on the values supplied.		// The ASSERT statements ensure that the generated		// code is in synch with the view resource.	fCompilerOptions_Always->SetValue(data.fCompilerOptions_Always);	fCompilerOptions_Errors->SetValue(data.fCompilerOptions_Errors);		// Save the initial values in case user cancels	saveData = data;}/****************************************************************************** DoEndData  	Collect final values from panes and tell subclass******************************************************************************/void x_CCompilerOptions::DoEndData(long theCommand){	CCompilerOptionsData		data;			/* The initial value record			*/	RgnHandle			rgn;			/* Selection region					*/										/* If user canceled the dialog,		*/	if (theCommand == cmdCancel)		/*	 return the initial values		*/	{		data = saveData;		EndData(&data);		return;	}										/* Collect values from panes		*/    data.fCompilerOptions_Always = fCompilerOptions_Always->GetValue();    data.fCompilerOptions_Errors = fCompilerOptions_Errors->GetValue();	EndData(&data);}/****************************************************************************** BeginData  	Collect initial values from subclass and initialize panes******************************************************************************/void x_CCompilerOptions::BeginData(CCompilerOptionsData *initial){}/****************************************************************************** UpdateData  	Tell subclass when panes change******************************************************************************/void x_CCompilerOptions::UpdateData(CCompilerOptionsUpdate *update, long itemNo){}/****************************************************************************** EndData  	Tell subclass the final values. For a canceled modal dialog, 	these are the same as the initial values.******************************************************************************/void x_CCompilerOptions::EndData(CCompilerOptionsData *final){}/****************************************************************************** DoModalDialog	{OVERRIDE}  	Override to call EndData******************************************************************************/long x_CCompilerOptions::DoModalDialog(long defaultCmd){	long	result = CDialogDirector::DoModalDialog(defaultCmd);		DoEndData(result);	return result;}/****************************************************************************** Close {OVERRIDE}  	Override to call EndData******************************************************************************/Boolean x_CCompilerOptions::Close(Boolean quitting){		if (itsWindow && !itsWindow->active)		itsWindow->Select();	if (EndDialog(cmdOK, TRUE))	{																				/* For a modal dialog, exit through	*/		if (itsWindow->IsModal())		/*	 DoModalDialog					*/		{			dismissCmd = cmdClose;			return TRUE;		}		else		{										/* Collect final values and call	*/			DoEndData(cmdOK);			/*	 EndData						*/										/* Do the Close						*/			return CDirector::Close(quitting);		}	}	else		return FALSE;}/****************************************************************************** ProviderChanged	{OVERRIDE} 	 	User did something. Note that the TCL 	does not currently report changes to plain CEditText items. ******************************************************************************/void x_CCompilerOptions::ProviderChanged(CCollaborator *aProvider, long reason, void* info){	CCompilerOptionsUpdate	data;			/* The update value record			*/	Str255	str;	Boolean	saveIgnore = ignore;		if (ignore)							/* Don't be a chatterbox			*/		return;	ignore = TRUE;		TRY	{ 		if (FALSE) {}		else if (reason == controlValueChanged				&& aProvider == fCompilerOptions_Always)		{			data.value = *(short*) info;			UpdateData(&data, kCompilerOptions_AlwaysID);		}		else if (reason == controlValueChanged				&& aProvider == fCompilerOptions_Errors)		{			data.value = *(short*) info;			UpdateData(&data, kCompilerOptions_ErrorsID);		}		else			CDialogDirector::ProviderChanged(aProvider, reason, info);	}	CATCH		ignore = saveIgnore;	ENDTRY		ignore = saveIgnore;				/* ProviderChanged() can't Close()!	*/}