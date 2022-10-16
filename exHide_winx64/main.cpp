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
	if (!strcmp(argv[1], "-v") || !strcmp(argv[1], "--version"))
	{
		gui_version();
		return 0;
	}
	if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))
	{
		gui_help();
		return 0;
	}
	else if (!strcmp(argv[1], "-e") || !strcmp(argv[1], "--encrypt"))
	{
		if (argc != 4)
		{
			gui_wrong();
			return 0;
		}
		encrypt(argv[2],argv[3]);
		cout << "\nEncrypt successfully!" << endl;
	}
	else if (!strcmp(argv[1], "-d") || !strcmp(argv[1], "--decrypt"))
	{
		if (argc != 3)
		{
			gui_wrong();
			return 0;
		}
		decrypt(argv[2]);
		cout << "\nDecrypt successfully!" << endl;
	}
	else
	{
		gui_wrong();
		return 0;
	}
	
	
	return 0;
}