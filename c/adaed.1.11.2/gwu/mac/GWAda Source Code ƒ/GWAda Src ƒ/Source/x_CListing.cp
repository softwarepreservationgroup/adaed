/****************************************************************************** x_CListing.c				CListing Document Class		Copyright (C) 1985-1992  New York University	Copyright (C) 1994 George Washington University	 	This file is part of the GWAdaEd system, an extension of the Ada/Ed-C	system.  See the Ada/Ed README file for warranty (none) and distribution	info and also the GNU General Public License for more details.	Generated by Classy� (Object Factory) 10:17 PM Mon, Sep 6, 1993	This file is rewritten each time you generate code. You should not	make changes to this file; changes should go in the My.c	file, instead.	If you want to change how Classy generates this file, you can	change the template for this file. It is "_Doc.c" in the Classy	Templates folder. ******************************************************************************/#include "x_CListing.h"#include "CListing.h"#include "ListingItems.h"#include "ViewUtilities.h"#include "CAdaApp.h"#include <CApplication.h>#include <CBartender.h>#include <Commands.h>#include <Constants.h>#include <CDecorator.h>#include <CDesktop.h>#include <CFile.h>#include <TBUtilities.h>#include <CWindow.h>extern	CApplication *gApplication;		/* The application					*/extern CDecorator	*gDecorator;		/* Decorator for arranging windows	*/extern CDesktop		*gDesktop;			/* The visible Desktop				*/extern CBartender	*gBartender;		/* Manages all menus				*/#include "CEditText.h"#include "CStaticText.h"	// Define symbols for commands handled by this class	// Prevents a recompile every time any command changed./**** C O N S T R U C T I O N / D E S T R U C T I O N   M E T H O D S ****//****************************************************************************** Ix_CListing	Initialize the document ******************************************************************************/void x_CListing::Ix_CListing(){	IDocument(gApplication, TRUE);		// Initialize data members below.}/****************************************************************************** MakeNewWindow	Create a new, empty window.  Subclass may override to populate	the new window. ******************************************************************************/void x_CListing::MakeNewWindow(void){	itsWindow = CLGetNamedWindow("\pListing", this);		itsMainPane = (CPane*) GetItemPointer(itsWindow, 0);		// Initialize pointers to the subpanes in the window  fListing_Pano1 = (CEditText*) itsWindow->FindViewByID(kListing_Pano1ID);  ASSERT(member(fListing_Pano1, CEditText));  fListing_Stat2 = (CStaticText*) itsWindow->FindViewByID(kListing_Stat2ID);  ASSERT(member(fListing_Stat2, CStaticText));}/****************************************************************************** FailOpen {OVERRIDE}		Fail if file already open in this application.				This function calls the application's FileAlreadyOpen function		and fails quietly if the file is open.		Note that open may also fail if the file is open in		another application. This will cause a failure in open,		but you may wish to override this function to detect this		case and provide a more meaningful error message than -49. ******************************************************************************/void	x_CListing::FailOpen(	CFile	*aFile){										/* Only the application knows		*/	if (((CAdaApp*)gApplication)->FileAlreadyOpen(aFile))		Failure(kSilentErr, 0);}/****************************************************************************** PositionWindow	The default method in CSaver calls the the decorator, which	staggers and resizes the window. Since the window has already	been positioned when it is initialized from the view resource,	we don't want to do this twice. ******************************************************************************/void	x_CListing::PositionWindow(){}