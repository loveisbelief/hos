#include "math.h"
#include <graphics.h>
#include <stdlib.h>
#include <time.h>
void main(void)
{
 int gdriver=DETECT,gmode;
 int ran_number;
 float a,b,c,d,e,f;
 float x,y,x_pre,y_pre;
 float disp_x,disp_y;
 initgraph(&gdriver,&gmode,"\\tc");
/* setfillstyle(SOLID_FILL,RED);*/
 randomize();
 setbkcolor(BLUE);
 setcolor(14);
 x=y=x_pre=y_pre=0;
 ran_number=90;
 while(kbhit()==0)
 {
  ran_number=random(100)+1;
  if (ran_number==1)
    {
     a=0;
     b=0;
     c=0;
     d=0.15;
     e=0;
     f=0;
    }
  else if (ran_number>1&&ran_number<=86)
   {
    a=0.87;
    b=0.014;
    c=-0.014;
    d=0.87;
    e=0;
    f=1.6;
   }
 else  if (ran_number>86&&ran_number<=93)
   {
    a=0.26;
    b=0.472;
    c=0.772;
    d=0.34;
    e=0;
    f=1.6;
   }
        else
   {
    a=0.28;
    b=0.867;
    c=-0.478;
    d=0.4;
    e=0;
    f=0.44;
   }
  x=a*x_pre*cos(b)-d*sin(c)*y_pre+e;
  y=c*x_pre*sin(b)+d*cos(c)*y_pre+f;
  x_pre=x;
  y_pre=y;
  disp_x=(x+5)*639/12;
  disp_y=350-y*28;
  putpixel((int)disp_x,(int)disp_y,GREEN);
 }
 getch();
 getch();
 closegraph();
 }
