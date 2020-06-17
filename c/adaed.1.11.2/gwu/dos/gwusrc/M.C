#include <malloc.h>
#include <fcntl.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <io.h>
#include <dos.h>
#include <bios.h>
#include <errno.h>
#include <stdio.h>
#include <stddef.h>
#include <conio.h>
#include <direct.h>
#include <process.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	short x;
	short y;
	short status;
	} AVL_MOUSE, *AVL_MOUSE_PTR;

/************************************************************************/
/***   Inicializa o Mouse no Video                                    ***/
/*                                                                      */
void AVL_MOUSE_INIT()
{
	union REGS inreg, outreg;
	inreg.x.ax = 0;
	int86(0x33, &inreg, &outreg);
	inreg.x.ax = 1;
	int86(0x33, &inreg, &outreg);
}

void AVL_MOUSE_ON()
{
	union REGS inreg, outreg;
	inreg.x.ax = 1;
	int86(0x33, &inreg, &outreg);
}

void AVL_MOUSE_OFF()
{
	union REGS inreg, outreg;
	inreg.x.ax = 2;
	int86(0x33, &inreg, &outreg);
}

void AVL_MOUSE_SET(int x, int y)
{
	union REGS inreg, outreg;
	inreg.x.ax = 4;
	inreg.x.cx = x;
	inreg.x.dx = y;
	int86(0x33, &inreg, &outreg);
}

/************************************************************************/
/***   Fornece o Status do Mouse - Posicao, Botao Precionado.  	      ***/
/**    Botoes: 0 - esquerda                                            **/
/*             1 - direita          Obs.: Definir estrutura de volta    */
/*             2 - meio                   fora do programa.  xmouse     */
/*                                                           ymouse     */
/*    Retorna:  1 - se botao pressionado                                */
/*              0 - caso contrario ou se nro. do botao for invalido     */
/*                                                                      */
AVL_MOUSE AVL_MOUSE_STATUS()
{
	union REGS inreg, outreg;
	static AVL_MOUSE m;
	short i;

	inreg.x.ax = 3;
	inreg.x.bx = 0;

	int86(0x33, &inreg, &outreg);

	m.x = outreg.x.dx;
	m.y = outreg.x.cx;
	m.x = m.x / 8 + 1;
	m.y = m.y / 8 + 1;
	m.status = outreg.x.bx;

	return(m);
}

void AVL_WVIDEO(unsigned char x, unsigned char a, short *c);
#define avl_video_base_address  0xB8000
#define AVL_MAP(r,c)  ((short *) (avl_video_base_address + (r - 1) * 160 + (c - 1) * 2))
#define AVL_PUT(ch,r,c,bk,cor) *AVL_MAP(r,c) = ((char) ch) | ((short) (((bk << 4) | cor) << 8))
#define AVL_ATT(r,c) ((char) (*(AVL_MAP(r,c)) >> 8))
#define AVL_CH(r,c)  ((char) *AVL_MAP(r,c))


static AVL_MOUSE m1;

AVL_MOUSE AVL_MOUSE_SELECT(int x1, int y1, int x2, int y2, int *row)
{
	static AVL_MOUSE m;
	short i;
	short c;
	char ch;
	char s[20];
	m = AVL_MOUSE_STATUS();
	if (m1.x == 0 && m1.y == 0)
		m1 = m;
	sprintf(s,"%2d %2d", m.x, m.y);
	for(i = 0; i < strlen(s); ++i)
		AVL_WVIDEO(s[i],(unsigned char) (0 << 4 | 3), AVL_MAP(1,73+i));
	if (m.x >= x1 && m.x <= x2 && m.y >= y1 && m.y <= y2) {
		for(i=y1; i <= y2; ++i)  {
			c = *AVL_MAP(m.x,i);
			ch = c;
			AVL_WVIDEO(c,(unsigned char) (0 << 4 | 4), AVL_MAP(m.x,i));
			}
		if (m1.x != m.x)  {
			for(i=y1; i <= y2; ++i)  {
				c = *AVL_MAP(m1.x,i);
				ch = c;
				AVL_WVIDEO(c,(unsigned char) (0 << 4 | 15), AVL_MAP(m1.x,i));
				}
			m1 = m;
			}
		}
	if (m.status == 1)  {
		AVL_MOUSE_OFF();
		exit(1);
		}
	return m;
}

	main()
{
	int i;
	AVL_MOUSE m;
	m1.x = m1.y = 0;

	AVL_MOUSE_INIT();
	AVL_MOUSE_ON();

	while ( 1 )  {
/*		m = AVL_MOUSE_STATUS();
		if (kbhit())  {
			printf("AVL_MOUSE_STATUS=%d x=%2d y=%2d\n"
				, m.status, m.x, m.y);
			i = getch();
			if (i == 0) i = getch();
			}
*/
		m = AVL_MOUSE_SELECT(2,2,15,20);
/*		m = AVL_MOUSE_STATUS();
		if (m.status == 1) 
			break; 
*/
		}
	AVL_MOUSE_OFF();
}
