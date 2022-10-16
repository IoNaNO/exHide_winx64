#pragma once
#include<iostream>
using namespace std;

#define APP_VERSION "0.0.1"

void gui_default()
{
	fprintf(stderr,
		"usage: excrypt [--version] [--help]\n"
		"               [--encrypt <filename> <filename>]\n"
		"               [--decrypt <filename>]\n");
}

void gui_version()
{
	cout << "exHide version: " << APP_VERSION << endl;
}

void gui_help()
{
	gui_default();
	fprintf(stderr,
		"Valid commands are:\n"
		" -h, --help\n"
		"\t Print help message and exit.\n"
		" -v, --version\n"
		"\t Print version message and exit.\n"
		" -e <targetfile name> <hidefile name>, --encrypt=<targetfile name> <hidefile name>\n"
		"\t Encrypt targetfile and hide it into hidefile.You need to type the password.\n"
		" -d <hidefile name>, --decrypt=<hidefile name>\n"
		"\t Decrypt targetfile from hidefile.You need to type the right password.\n");
}

void gui_wrong()
{
	cerr << "No such command!" << endl;
	gui_default();
}