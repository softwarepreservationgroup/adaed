/**** * CTextPane.h * *	Text edit pane for the Source Code Editor and the Source Listing *	Copyright (C) 1985-1992  New York University	Copyright (C) 1994 George Washington University	 	This file is part of the GWAdaEd system, an extension of the Ada/Ed-C	system.  See the Ada/Ed README file for warranty (none) and distribution	info and also the GNU General Public License for more details. * ****/#pragma once/* #define useCPEdit  controls which class to use as the base class * for this CTextPane class. */#ifdef useCPEdit#define useCPEdit#include "CPEditText.h"struct CTextPane : CPEditText {#else#include <CEditText.h>struct CTextPane : CEditText {#endif								/** Instance Methods **/									/** Contruction/Destruction **/	void	ITextPane(CView *anEnclosure, CBureaucrat *aSupervisor,				short			aWidth,				short			aHeight,				short			aHEncl,				short			aVEncl,				SizingOption	aHSizing,				SizingOption	aVSizing);			void	Activate( void);	virtual	void	SetTEWordBreak(void);			long	GetCursorLine(void);	virtual	void	SelectLine(long line);	virtual	void	SelectFT(long line1, long col1,					  long line2, long col2);			short	GetFontSize(void);			short	GetFontNum(void);};