/* BKEYBRD.C: This program prints a message on the screen until the
 * right SHIFT key is pressed.
 */

#include <bios.h>
#include <conio.h>
#include <stdio.h>

void main()
{
   unsigned x;
   int i;
   printf( "Use the right SHIFT key to stop this message\n" );
   while( !((x = _bios_keybrd( _NKEYBRD_SHIFTSTATUS )) & 0001))  {
   		if (!(x & 0001)) printf("Left shift\n"); 
   		if (!(x & 0002)) printf("^ key\n"); 
   		if (!(x & 0003)) printf("Alt \n"); 
   		if (!(x & 0007)) printf("In insert mode\n"); 
   		if (!(x & 0x08)) printf("Left ^\n"); 
   		if (!(x & 0x09)) printf("Left Alt\n"); 
   		if (!(x & 0x0A)) printf("Right ^\n"); 
   		if (!(x & 0x0B)) printf("Right Alt\n"); 
   		i = getch(); if (i == 0) i = getch();
   		}
}
