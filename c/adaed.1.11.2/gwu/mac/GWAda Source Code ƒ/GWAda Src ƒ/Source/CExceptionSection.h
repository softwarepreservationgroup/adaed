/****************************************************************************** CExceptionSection.h				Header for CExceptionSection ArrayPane Class		Copyright (C) 1985-1992  New York University	Copyright (C) 1994 George Washington University	 	This file is part of the GWAdaEd system, an extension of the Ada/Ed-C	system.  See the Ada/Ed README file for warranty (none) and distribution	info and also the GNU General Public License for more details.	Generated by Classy� (Object Factory) 7:12 AM Tue, Sep 28, 1993	This file is only generated once. You can modify it by filling	in the placeholder functions and adding any new functions you wish.	If you change the name of the document class, a fresh version of this	file will be generated. If you have made any changes to the file	with the old name, you will have to copy those changes to the new	file by hand. ******************************************************************************/#pragma once#include "x_CExceptionSection.h"#include "MacAdaLib.h"class CExceptionSection : public x_CExceptionSection{public:			LSlot	slot;									/** Initialize **/	void		ICExceptionSection( CView *anEnclosure, CBureaucrat *aSupervisor,							short aWidth, short aHeight,							short aHEncl, short aVEncl,							SizingOption aHSizing, SizingOption aVSizing);	void	SetSlot(LSlot e);	void	GetCellText( Cell aCell, short availableWidth, StringPtr itsText);};