#pragma once
#include"gcrypt.h"
#include<iostream>

#define LEN_OF_RETKEY 32
#define SIZE_OF_RETKEY LEN_OF_RETKEY
#define ITER 4096
#define SALT "thisIsSaLT"
#define MAGIC_STRING "ThisIsMSforDEMO"


void gcrypt_init();

std::string getpasswd(std::string);
