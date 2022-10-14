#pragma once
#include<iostream>
using namespace std;

#define APP_VERSION "0.0.1"

void gui_default()
{
	cout << "exHide version: " << APP_VERSION << endl;
}

void gui_help()
{
	cout << "This is help" << endl;
}

void gui_wrong()
{
	cout << "This is wrong" << endl;
}