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

/*  Info.c  */

#include <externs.h>


unsigned AVL_NBYTES()
{
	AVL_EDIT_WINDOW_PTR w;
	AVL_LINE_PTR temp;
	unsigned n = 0;
	w = &avl_windows[avl_window];
	for (temp = w -> head -> next; temp != w -> head; temp = temp -> next)
		n += (strlen(temp -> line) + 1);
	if ((w -> head -> previous -> line_no == 2) &&
	    (strlen(w -> head -> next -> line) == 0)) --n;
	return n;
}

void AVL_ABOUT_GWADA() 
{  
	struct diskfree_t *dks;
	short co;
	int n, k;
	char line[180];
	char l2[100];
	AVL_WIN_PTR w;
	sprintf(line," Info - GWAda V %s - By A. V. Lopes & M. B. Feldman ",GWAda_Version);
	n = strlen(line) + 2;
	k = (80 - n) / 2;
	w = AVL_MAKE_WINDOW(line,5,k,14+5,k+n+2,avl_wnd_bk_color,avl_wnd_color);
	_settextposition(2,2);
	sprintf(line,"Current drive and directory: %s", avl_curr_directory);
	_outtext(line);
	_settextposition(3,2);
	sprintf(line,"Source directory           : %s", avl_dir_sources);
	_outtext(line);
	_settextposition(4,2);
	sprintf(line,"Library directory          : %s", avl_dir_library);
	_outtext(line);

	_settextposition(6,2);
	if (_dos_getdiskfree(0,dks) != 0)  
		sprintf(line,"Disk space available       : unknown");
	else
		sprintf(line,"Disk space available       : %ldKb",
			(((dks -> bytes_per_sector * 1L) *  
			   dks -> sectors_per_cluster) * dks -> avail_clusters) / 1024L);
	_outtext(line);
	_settextposition(7,2);
	sprintf(line,"Extended memory available  : %ldKb", _memavl() / 1024L);
	_outtext(line);
	_settextposition(8,2);
	sprintf(line,"Active text file name      : %s", avl_windows[avl_window].file_name);
	_outtext(line);
	_settextposition(9,2);
	sprintf(line,"     No. of text lines     : %d", 
		avl_windows[avl_window].head -> previous -> line_no - 1);
	_outtext(line);
	_settextposition(10,2);
	sprintf(line,"     No. of text characters: %u", AVL_NBYTES());
	_outtext(line);
	_settextposition(12,2);
	_settextcolor(9);
	_outtext("Press any key to return to top menu...");
	if ((k = getch()) == 0) k = getch();
	AVL_DEL_WINDOW(w);
}
