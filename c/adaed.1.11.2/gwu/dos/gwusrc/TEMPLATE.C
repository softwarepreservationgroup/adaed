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

/* template.c */

#include <externs.h>

void AVL_MOUNT_TEMPLATE(char *local)
{
	AVL_EDIT_WINDOW_PTR w;
	AVL_LINE_PTR temp, head = NULL;
	AVL_WIN_PTR m1;
	char fname[100];
	int n1, n2, x;
	char msg2[80];
	char *pl;
	pl = getenv("ADAED");
	if (pl == NULL)
		pl=getenv("adaed");
	if (pl != NULL)  
		sprintf(fname,"%s%c%s",pl,'\\',local);
	else  {
		AVL_ERROR("Set the ADAED environment variable first (see readme.gw).");
		return;
		}		

	w = &avl_windows[avl_window];
	n1 = w -> txt_col;
	avl_start_pos = AVL_COL() - 1;
	if (avl_start_pos < 0) avl_start_pos = 0;
	w -> txt_col = 0;
	temp = AVL_MAKE_LINE("### DUMMY ###",0);
    AVL_LINE_INSERT(temp,&head);
    x = AVL_LOAD(fname, &head);
	avl_start_pos = 0;

	if (!x) {
		sprintf(msg2,"Can't open \'%s\'", fname);
		AVL_ERROR(msg2);
		w -> txt_col = n1;
		}
	else {
		AVL_CLEAR_BLOCK();
		avl_block_first_line = head -> next;
		avl_block_last_line = head -> previous;
		avl_block_first_col = 0;
		avl_block_last_col = strlen(head -> previous -> line) - 1;
		if (avl_block_last_col < 0) avl_block_last_col = 0;
		AVL_EDIT_COPY(1);
		AVL_CLEAR_BLOCK();
		AVL_FIND(1,"$");
		AVL_EDIT_DEL_RIGHT(1);
		}
}

void AVL_TEMPLATE_ADA_PGM()
{
	AVL_MOUNT_TEMPLATE("adapgm.tpl");
	strcpy(avl_message,"Enter the program's name");
}
	
void AVL_TEMPLATE_ADA_VECTOR()
{
	AVL_MOUNT_TEMPLATE("adavecto.tpl");
	strcpy(avl_message,"Enter the vector's name (type)");
}
	
void AVL_TEMPLATE_ADA_ACCESS()
{
	AVL_MOUNT_TEMPLATE("adaacces.tpl");
	strcpy(avl_message,"Enter the pointer's name (type)");
}
	
void AVL_TEMPLATE_ADA_RECORD()
{
	AVL_MOUNT_TEMPLATE("adarecor.tpl");
	strcpy(avl_message,"Enter the record's name (type)");
}
	
void AVL_TEMPLATE_ADA_MATRIX()
{
	AVL_MOUNT_TEMPLATE("adamatri.tpl");
	strcpy(avl_message,"Enter the matrix's name (type)");
}
	
void AVL_TEMPLATE_ADA_TASK()
{
	AVL_MOUNT_TEMPLATE("adatask.tpl");
	strcpy(avl_message,"Enter the task's name");
}
	
void AVL_TEMPLATE_ADA_PACKAGE()
{
	AVL_MOUNT_TEMPLATE("adapacka.tpl");
	strcpy(avl_message,"Enter the package's name");
}
	
void AVL_TEMPLATE_ADA_GENERIC()
{
	AVL_MOUNT_TEMPLATE("adagener.tpl");
	strcpy(avl_message,"Enter the stack's name (type)");
}
	
void AVL_TEMPLATE_ADA_SERVER()
{
	AVL_MOUNT_TEMPLATE("adaserve.tpl");
	strcpy(avl_message,"Enter the entry's name (type)");
}
	
void AVL_TEMPLATE_ADA_LOOP()
{
	AVL_MOUNT_TEMPLATE("adaloop.tpl");
	strcpy(avl_message,"Enter the loop's range");
}
	
void AVL_TEMPLATE_ADA_WHILE()
{
	AVL_MOUNT_TEMPLATE("adawhile.tpl");
	strcpy(avl_message,"Enter the while condition");
}
	
void AVL_TEMPLATE_ADA_IF()
{
	AVL_MOUNT_TEMPLATE("adaif.tpl");
	strcpy(avl_message,"Enter the condition");
}
	
void AVL_TEMPLATE_ADA_CASE()
{
	AVL_MOUNT_TEMPLATE("adacase.tpl");
	strcpy(avl_message,"Enter the case's object");
}
	
void AVL_TEMPLATE_ADA_FUNCTION()
{
	AVL_MOUNT_TEMPLATE("adafunct.tpl");
	strcpy(avl_message,"Enter the function's name");
}
	
void AVL_TEMPLATE_ADA_PROCEDURE()
{
	AVL_MOUNT_TEMPLATE("adaproce.tpl");
	strcpy(avl_message,"Enter the procedure's name");
}
