#include<iostream>
#include"cryptApi.h"
#include"app.h"
using namespace std;



int main(int argc,char** argv)
{
	if (argc == 1)
	{
		gui_default();
	}
	
	gcrypt_init();
	
	return 0;
}