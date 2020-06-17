/**** * CListingPane.h * *	A text pane for the source listing.  It doesn't allow editing, *  nor selecting.  It highlights the current error... * ****//* * Copyright (C) 1985-1992  New York University * Copyright (C) 1994 George Washington University *  * This file is part of the GWAdaEd system, an extension of the Ada/Ed-C * system.  See the Ada/Ed README file for warranty (none) and distribution * info and also the GNU General Public License for more details. */#define _H_CListingPane#include "CTextPane.h"struct CListingPane : public CTextPane {	short	itsLine1;	short	itsLine2;	short	fVOrigin;									/** Contruction/Destruction **/	void	IListingPane(CView *anEnclosure, CBureaucrat *aSupervisor,				short			aWidth,				short			aHeight,				short			aHEncl,				short			aVEncl,				SizingOption	aHSizing,				SizingOption	aVSizing);			void	HilightLines(short line1, short line2);			void	PrepareToPrint( void);	virtual	void	SetTEWordBreak(void);};