/****************************************************************************** x_CBindingSection.c				CBindingSection ArrayPane Class		Copyright (C) 1985-1992  New York University	Copyright (C) 1994 George Washington University	 	This file is part of the GWAdaEd system, an extension of the Ada/Ed-C	system.  See the Ada/Ed README file for warranty (none) and distribution	info and also the GNU General Public License for more details.	Generated by Classy� (Object Factory)	This file is rewritten each time you generate code. You should not	make changes to this file; changes should go in the CBindingSection.c	file, instead.	If you want to change how Classy generates this file, you can	change the template for this file. It is "_Table.c" in the Classy	Templates folder. ******************************************************************************/#include "x_CBindingSection.h"#include "ViewUtilities.h"/**** C O N S T R U C T I O N / D E S T R U C T I O N   M E T H O D S ****//****************************************************************************** Ix_CBindingSection	Initialize the object ******************************************************************************/void	x_CBindingSection::Ix_CBindingSection(CView *anEnclosure, CBureaucrat *aSupervisor,						short aWidth, short aHeight,						short aHEncl, short aVEncl,						SizingOption aHSizing, SizingOption aVSizing){	CArrayPane::IArrayPane(anEnclosure, aSupervisor,					aWidth, aHeight, aHEncl, aVEncl, aHSizing, aVSizing);		// All data members are initialized to zero}/****************************************************************************** PutTo        Put the contents of this object to the stream ******************************************************************************/void	x_CBindingSection::PutTo(    CStream     *aStream){		// Put data members for this class	CArrayPane::PutTo(aStream);	/* Let superclass save				*/}/****************************************************************************** GetFrom        Get the contents of this object from the stream and        initialize the object ******************************************************************************/void	x_CBindingSection::GetFrom(    CStream     *aStream){		// Get data members for this class										/* Let superclass restore			*/	CArrayPane::GetFrom(aStream);}