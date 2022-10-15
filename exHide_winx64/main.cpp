#include<iostream>
#include"cryptApi.h"
#include"app.h"
using namespace std;


int main(int argc, char** argv)
{
	gcrypt_init();

	if (argc == 1)
	{
		gui_default();
		return 0;
	}
	if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))
	{
		gui_help();
		return 0;
	}
	else if (!strcmp(argv[1], "-e") || !strcmp(argv[1], "--encrypt"))
	{
		if (argc == 2)
		{
			gui_wrong();
			return 0;
		}
		encrypt(argv[2]);
	}
	else if (!strcmp(argv[1], "-d") || !strcmp(argv[1], "--decrypt"))
	{
		if (argc == 2)
		{
			gui_wrong();
			return 0;
		}
		decrypt(argv[2]);
	}
	
	
	return 0;
}