/****************************************************************************** x_CAboutBox.h			Header File For CAboutBox Lower-Layer Dialog Class		Copyright (C) 1985-1992  New York University	Copyright (C) 1994 George Washington University	 	This file is part of the GWAdaEd system, an extension of the Ada/Ed-C	system.  See the Ada/Ed README file for warranty (none) and distribution	info and also the GNU General Public License for more details.	Generated by Classy� (Object Factory)	This file is rewritten each time you generate code. You should not	make changes to this file; changes should go in the My.h	file, instead.	If you want to change how Classy generates this file, you can	change the template for this file. It is "_Dialog.h" in the Classy	Templates folder. ******************************************************************************/#pragma once#include <CDialogDirector.h>CLASS CButtonProc;CLASS CStaticText;CLASS CPicture;	// Data struct for initializing dialog items	// and receiving changed valuestypedef struct{	// This dialog has no editable panes!	// The Boolean prevents a compile error.		Boolean	dummy;} CAboutBoxData;	// We define a separate struct for UpdateData() which	// eliminates duplicate data typestypedef struct{	Str255		stringvalue;			// CDialogText	long		longvalue;				// CIntegerText	short		value;					// All other controls and buttons	Point		selection;				// CArrayPane} CAboutBoxUpdate;CLASS CDirectorOwner;CLASS CPanorama;CLASS CPane;class x_CAboutBox : public CDialogDirector{public:										// Pointers to panes in window	CButtonProc	*fAboutBox_Butn1;	CStaticText	*fAboutBox_Stat2;	CPicture	*fAboutBox_Pict3;	CButtonProc	*fAboutBox_Butn4;	void 	Ix_CAboutBox(CDirectorOwner *aSupervisor);	virtual	long	DoModalDialog(long defaultCmd);	virtual	Boolean	Close(Boolean quitting);	virtual	void	ProviderChanged(CCollaborator *aProvider, long reason, void* info);	virtual	void	DoCommand(long theCommand);	virtual	void	UpdateMenus(void);protected:	Boolean			ignore;	CAboutBoxData	saveData;	virtual void 	MakeNewWindow(void);	virtual void 	BeginData(CAboutBoxData *initial);	virtual void 	UpdateData(CAboutBoxUpdate *update, long itemNo);	virtual void 	EndData(CAboutBoxData *final);	virtual void	DoCmdCredits(void);	CPane	*FindPane(long ID);private:	virtual	void 	DoBeginData(void);	virtual	void 	DoEndData(long theCommand);};#define	CVueCAboutBox	128