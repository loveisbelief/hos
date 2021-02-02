//于渊《自己动手写操作系统》
void myprint(char*msg,int len);
int choose(int a,int b)
{
    if(a>=b)
    {
        myprint("the 1st one\n",13);
    }
    else
    {
        myprint("the 2nd one\n",13);
    }
}