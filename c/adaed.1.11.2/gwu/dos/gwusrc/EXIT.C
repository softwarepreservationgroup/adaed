/*
    GWAda Development Environment for 386/486 PCs   
    Copyright (C) 1993, Arthur Vargas Lopes  & Michael Bliss Feldman
                        vlopes@vortex.ufrgs.br mfeldman@seas.gwu.edu
 
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; version 2 of the License.    

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

/* exit.c */

#include "externs.h"


void AVL_EXIT()
{
	char msg[80];
	int i;
	AVL_EDIT_WINDOW_PTR w;
	for(i = 0; i < avl_nwindows; ++i)  {
		w = &avl_windows[avl_window = i];
		sprintf(msg," Save %s (Y/N) ? ", w -> file_name);
		if (w -> changed)
			if (do_not_realy_exit && i == 0)  
				AVL_SAVE();
			else if (AVL_QUESTION(msg) == 'Y')
				AVL_SAVE();
		}
	_setvideomode( _DEFAULTMODE );
	if (!do_not_realy_exit)  {
		exit(0);
		AVL_RESTORE_I23();
		}
}
