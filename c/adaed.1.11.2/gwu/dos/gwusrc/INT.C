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

#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <stk.h>
#include <i32.h>

#pragma interrupt (AVL_INT23)
void AVL_INT23();               /* User Int 23h protected-mode interrupt handler */

void (*AVL_PREV_INT23)();             /* Previous Int 23h protected-mode handler */


void AVL_INSTALL_I23()               /* Control-C Protected-Mode Handler Example */
{
   AVL_PREV_INT23 = _dos_getvect ( 0x23u  );    /* Save previous handler pointer */
   _dos_setvect ( 0x23u, AVL_INT23 );            /* Install user Int 23h handler */
}

void AVL_RESTORE_I23()
{
   _dos_setvect ( 0x23u, AVL_PREV_INT23 );           /* Restore previous handler */
}


void AVL_INT23 ( void )         /* User Int 23h Protected-Mode Interrupt Handler */
{
   extern int avl_ctrl_c;
   _XSTACK *ebp;                            /* Real-mode handler stack frame */

   ebp = (_XSTACK *) _get_stk_frame();           /* Get stack frame address */

/*   avl_ctrl_c = 1;   */

         /* Perform interrupt handling services, ie, ignore Control-C */

   ebp->opts |= _STK_NOINT;                      /* Bypass real-mode handler */
}


