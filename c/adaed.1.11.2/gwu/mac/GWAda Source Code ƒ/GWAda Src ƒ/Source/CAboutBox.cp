/****************************************************************************** CAboutBox.c				CAboutBox Dialog Director Class		Copyright (C) 1985-1992  New York University	Copyright (C) 1994 George Washington University	 	This file is part of the GWAdaEd system, an extension of the Ada/Ed-C	system.  See the Ada/Ed README file for warranty (none) and distribution	info and also the GNU General Public License for more details.	Generated by Classy� (Object Factory) 9:22 PM Thu, Aug 5, 1993	This file is only generated once. You can modify it by filling	in the placeholder functions and adding any new functions you wish.	If you change the name of the dialog class, a fresh version of this	file will be generated. If you have made any changes to the file	with the old name, you will have to copy those changes to the new	file by hand. ******************************************************************************/#include "CAboutBox.h"#include "AboutBoxItems.h"#include <CStaticText.h>#include <CPicture.h>#include <CButtonProc.h>#define kFirstPict 128#define kLastPict  139#define kFirstCredit	128#define kAboutStr	1#define kCreditStr	2#define kMoreStr	3static	GWorldPtr	pOldPort;static	GDHandle	pOldDevice;void	UseOffWorld(GWorldPtr offWorlder);void	DoneWithOffWorld(GWorldPtr offWorlder);void	UseOffWorld(GWorldPtr offWorlder){	GetGWorld(&pOldPort, &pOldDevice);	LockPixels(GetGWorldPixMap(offWorlder));	SetGWorld(offWorlder, NULL);}void	DoneWithOffWorld(GWorldPtr offWorlder){	UnlockPixels(GetGWorldPixMap(offWorlder));	SetGWorld(pOldPort, pOldDevice);}/**** C O N S T R U C T I O N / D E S T R U C T I O N   M E T H O D S ****//****************************************************************************** ICAboutBox	Initialize the dialog	IMPORTANT NOTE: When an object is initialized as part of a	'CVue' resource, this I-function is NEVER CALLED. Instead,	the object is initialized in its GetFrom method(s). To add your	own initialization, override GetFrom. ******************************************************************************/void CAboutBox::ICAboutBox(CDirectorOwner *aSupervisor){	fCurrPicture = kFirstPict;	fFirstTime = true;	fHaveGWorlds = false;	fInCredits = kAboutStr;	fNumCredits = Count1Resources('TEXT');	fCurrCredits = kFirstCredit;	// Initialize data members that must be set up before	// BeginData is called here	x_CAboutBox::Ix_CAboutBox(aSupervisor);	// CreateOffWorld();}void	CAboutBox::Dispose(void){	/* Get rid of GWorld */	DestroyOffWorld();	inherited::Dispose();}void	CAboutBox::CreateOffWorld(void){	OSErr		err;	long		response;	err = Gestalt(gestaltQuickdrawVersion, &response);	if ((err == noErr) && (response >= gestalt32BitQD)) {			GWorldPtr	braveNewGWorld;		GrafPtr		oldPort;		GDHandle	oldDevice;		LongRect	theFrame;		Rect		globalRect;		fAboutBox_Pict3->GetFrame(&theFrame);		globalRect.left = theFrame.left;		globalRect.top = theFrame.top;		globalRect.right = theFrame.right;		globalRect.bottom = theFrame.bottom;		err = NewGWorld(&braveNewGWorld, 0, &globalRect, NULL, NULL, 0);		GetGWorld((CGrafPtr *) &oldPort, &oldDevice);		UseOffWorld(braveNewGWorld);		EraseRect(&braveNewGWorld->portRect);		DoneWithOffWorld(braveNewGWorld);		SetGWorld((CGrafPtr) oldPort, oldDevice);		fOffscreen = (GrafPtr) braveNewGWorld;		fHaveGWorlds = true;	}	else {		fHaveGWorlds = false;		fOffscreen = NULL;	}}void	CAboutBox::DestroyOffWorld(void){	if (fHaveGWorlds) {		DisposeGWorld((GWorldPtr) fOffscreen);		fOffscreen = NULL;		fHaveGWorlds = false;	}}void	CAboutBox::PictureToOffWorld(short pictId){	if (fHaveGWorlds) {		PicHandle	thePic;		/* Get picture 'pictId', draw in offworld, and get rid of it */		thePic = GetPicture(pictId);		if (thePic) {			GWorldPtr	braveNewGWorld;			braveNewGWorld = (GWorldPtr)fOffscreen;			UseOffWorld(braveNewGWorld);			DrawPicture(thePic,  &braveNewGWorld->portRect);			DoneWithOffWorld(braveNewGWorld);			ReleaseResource((Handle) thePic);		}	}	fAboutBox_Pict3->UsePICT(pictId);	// change pict but don't refresh}void	CAboutBox::OffWorldToDialogue(void){	if (fHaveGWorlds) {		LongRect	theFrame;		Rect		pictRect;		GrafPtr		windowPort;		GWorldPtr	braveNewGWorld;		braveNewGWorld = (GWorldPtr)fOffscreen;		fAboutBox_Pict3->GetFrame(&theFrame);		fAboutBox_Pict3->FrameToWindR(&theFrame, &pictRect);		windowPort = itsWindow->GetMacPort();		UseOffWorld(braveNewGWorld);		itsWindow->Prepare();		SetGDevice(GetMainDevice());		CopyBits(&((GrafPtr)braveNewGWorld)->portBits, &windowPort->portBits,				 &braveNewGWorld->portRect, &pictRect, srcCopy, NULL);		DoneWithOffWorld(braveNewGWorld);	}	else		fAboutBox_Pict3->Refresh();}/****************************************************************************** Dawdle		Kill some time while the user isn't doing anything. Subclasses		should override this method if they want to perform some		repetitive active, such as twiddling their thumbs or flashing		a text insertion cursor. If the Bureaucrat must regularly receive		Dawdle() messages, the maxSleep parameter should be set to the		maximum time, in ticks, between successive Dawdle() messages. ******************************************************************************/void	CAboutBox::Dawdle(long	*maxSleep){	*maxSleep = 8;		/* 8 ticks, close to 7 times per second */	if (fFirstTime) {		fFirstTime = false;		fCurrPicture++;		PictureToOffWorld(fCurrPicture);	}	else if (fCurrPicture == kLastPict) {		*maxSleep = 0;	}	else {		OffWorldToDialogue();		fCurrPicture++;		PictureToOffWorld(fCurrPicture);	}}	/****************************************************************************** DoCmdCredits		Respond to cmdCredits command. ******************************************************************************/void CAboutBox::DoCmdCredits(){	Handle	text;	Str255	label;	/* Change the text in the fAboutBox_Stat2 StaticText item */	if (fCurrCredits == kFirstCredit) {		if (fNumCredits > 1)			fInCredits = kMoreStr;		else			fInCredits = kAboutStr;		fCurrCredits++;	}	else if (fCurrCredits == kFirstCredit + fNumCredits - 1) {		fInCredits = kCreditStr;		fCurrCredits = kFirstCredit;	}	/* Button reads: Credits, change it to More */	else {		if (fCurrCredits == kFirstCredit + fNumCredits - 2)			fInCredits = kAboutStr;		else			fInCredits = kMoreStr;		fCurrCredits++;	}	text = Get1Resource('TEXT', fCurrCredits);	GetIndString(label, 20050, fInCredits);	fAboutBox_Stat2->SetTextHandle(text);	ReleaseResource(text);	fAboutBox_Stat2->Refresh();	fAboutBox_Butn4->SetTitle(label);	fAboutBox_Butn4->Refresh();}