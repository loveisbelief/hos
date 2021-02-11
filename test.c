#include<stdio.h>
//using namespace std;
int main()
{
	int num=1;
	float* pn=&num;
	(*pn)+=100;
	printf("%d",num);
	//cout<<"Hello, world!"<<endl;
	return 0;
}