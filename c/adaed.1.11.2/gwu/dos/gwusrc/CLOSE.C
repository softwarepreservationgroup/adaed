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

/* close.c */

#include "externs.h"




void AVL_CLOSE()
{
	int i;
	char msg[81];
	if (avl_nwindows == 1)  
		AVL_EXIT();
	if (avl_windows[avl_window].changed)  {
		sprintf(msg," Save %s (Y/N) ? ",avl_windows[avl_window].file_name);
		if (AVL_QUESTION(msg) == 'Y')
			AVL_SAVE();
		}
	AVL_FREE_ALL();
	for(i = avl_window; i < (avl_nwindows - 1); ++i)
		avl_windows[i] = avl_windows[i + 1];
	--avl_nwindows;
	if (avl_window != 0) --avl_window;
	AVL_RESTORE_ENV();
}
                
