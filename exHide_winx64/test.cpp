#include<iostream>
#include"gcrypt.h"

using namespace std;



int main(int argc,char** argv)
{
	string teststr;
	cin >> teststr;
	int hash_len = gcry_md_get_algo_dlen(GCRY_MD_SHA1);
	cout << "compile succeed!" <<hash_len<< endl;
	return 0;
}