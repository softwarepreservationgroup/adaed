/****************************************************************************** x_CConsole.h			Header File For CConsole Lower-Layer Director Class		Copyright (C) 1985-1992  New York University	Copyright (C) 1994 George Washington University	 	This file is part of the GWAdaEd system, an extension of the Ada/Ed-C	system.  See the Ada/Ed README file for warranty (none) and distribution	info and also the GNU General Public License for more details.	Generated by Classy� (Object Factory)	This file is rewritten each time you generate code. You should not	make changes to this file; changes should go in the My.h	file, instead.	If you want to change how Classy generates this file, you can	change the template for this file. It is "_Director.h" in the Classy	Templates folder. ******************************************************************************/#pragma once#include <CDirector.h>CLASS CDirectorOwner;CLASS CPanorama;CLASS CPane;CLASS CEditText;class x_CConsole : public CDirector{public:										// Pointers to panes in window	CEditText	*fConsole_Pano1;	void 	Ix_CConsole(CDirectorOwner *aSupervisor);protected:	Boolean	ignore;	virtual void 	MakeNewWindow(void);	virtual	CPane	*FindPane(long ID);};