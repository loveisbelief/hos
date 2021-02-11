#include <graphics.h>
#include <math.h>
int main()
{
int gdriver, gmode;
float md=0.01;
float m,x;
int i,j;
gdriver=VGA;
gmode=VGAHI;
initgraph(&gdriver, &gmode, "c:\\caic\\bgi");
/*bar3d(100, 100, 300, 250, 50, 1);*/ /*画一长方体*/
/*float mstart=0;
float mend=2;
float n=200;
float md=(mend-mstart)/n;*/
for(m=0;m<=200;m=m+md)
{
	x=0.5;
	for(i=1;i<=200;i++)
		x=1-m*x*x;
	for(j=1;j<=300;j++)
	{
		x=1-m*x*x;
		putpixel(floor(320*m),200-floor(100*(1+x)),4);
	}
}
getch();
closegraph();
return 0;
}
