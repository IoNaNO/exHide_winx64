#define _CRT_SECURE_NO_WARNINGS
#include"cryptApi.h"
#include<conio.h>
#include<fstream>
#include <string.h>
#include <Windows.h>
using namespace std;

int add_hidefile(const char* filename)
{
    //在该文件下新建一个名为HIDE的流
    char stream_name[MAX_PATH + 36];
    strcpy(stream_name, filename);
    strcat(stream_name, ":HIDE:$DATA");
    int size = MultiByteToWideChar(CP_ACP, 0, stream_name, -1, NULL, 0);
    WCHAR w_stream_name[MAX_PATH + 36];
    MultiByteToWideChar(CP_ACP, 0, stream_name, -1, w_stream_name, size);

    HANDLE hStream = CreateFile(w_stream_name, // Filename
        GENERIC_WRITE,           // Desired access
        FILE_SHARE_WRITE,        // Share flags
        NULL,                    // Security Attributes
        OPEN_ALWAYS,             // Creation Disposition
        0,                       // Flags and Attributes
        NULL);                  // OVERLAPPED pointer
    if (hStream == INVALID_HANDLE_VALUE)
        return 0;
    CloseHandle(hStream);
    hStream = INVALID_HANDLE_VALUE;
    return 1;
}

//之后可以删了
int edit_hidefile(char* filename, char* src)
{
    char stream_name[MAX_PATH + 36];
    strcpy(stream_name, filename);
    strcat(stream_name, ":hide:$DATA");

    //const char* s = "this is hide test content!";//这里是测试内容
    fstream f, s;
    f.open(stream_name, ios::out);
    s.open(src, ios::in);
    if (!f.is_open() || !s.is_open())
        return 0;
    f << s.rdbuf();
    f.close();
    return 1;
}

//之后可以删了
int read_hidefile(char* filename)
{
    char stream_name[MAX_PATH + 36];
    strcpy(stream_name, filename);
    strcat(stream_name, ":hide:$DATA");

    fstream infile, outfile;
    infile.open(stream_name, ios::in);
    outfile.open("outfile.txt", ios::out);//这个文件名可以改
    if (!infile.is_open() || !outfile.is_open())
        return 0;
    outfile << infile.rdbuf();
    infile.close();
    outfile.close();
    return 1;
}

void xerr(const char* msg)
{
    fprintf(stderr, "%s\n", msg);
    exit(1);
}

void gcrypt_init()
{
    gcry_error_t err = 0;
    /* Version check should be the very first call because it
     makes sure that important subsystems are initialized.
     #define NEED_LIBGCRYPT_VERSION to the minimum required version. */
    if (!gcry_check_version(GCRYPT_VERSION))
    {
        fprintf(stderr, "libgcrypt is too old (need %s, have %s)\n",
            GCRYPT_VERSION, gcry_check_version(NULL));
        exit(2);
    }

    /* We don't want to see any warnings, e.g. because we have not yet
       parsed program options which might be used to suppress such
       warnings. */
    err=gcry_control(GCRYCTL_SUSPEND_SECMEM_WARN);

    /* ... If required, other initialization goes here.  Note that the
       process might still be running with increased privileges and that
       the secure memory has not been initialized.  */

       /* Allocate a pool of 16k secure memory.  This makes the secure memory
          available and also drops privileges where needed.  Note that by
          using functions like gcry_xmalloc_secure and gcry_mpi_snew Libgcrypt
          may expand the secure memory pool with memory which lacks the
          property of not being swapped out to disk.   */
    err|=gcry_control(GCRYCTL_INIT_SECMEM, 16384, 0);

    /* It is now okay to let Libgcrypt complain when there was/is
       a problem with the secure memory. */
    err|=gcry_control(GCRYCTL_RESUME_SECMEM_WARN);

    /* ... If required, other initialization goes here.  */

    /* Tell Libgcrypt that initialization has completed. */
    err|=gcry_control(GCRYCTL_INITIALIZATION_FINISHED, 0);

    if (err) {
        xerr("gcrypt:failed initialization");
    }

}

std::string getpasswd(const std::string prompt)
{
    std::string pwd;
    std::cout << prompt;
    char ch = 0;
    while (ch = _getch())
    {
        if (ch == '\n' || ch == '\r')
        {
            break;
        }
        else if (ch == 8)
        {
            pwd.erase(pwd.end() - 1);
            std::cout << "\b \b";
        }
        else
        {
            _putch('*');
            pwd += ch;
        }
    }
    return pwd;
}

void cipher_error_handler(int cipher_err){
	fprintf(stderr,"An error occured! id=%d",cipher_err);
	exit(3);
}

std::string get_key_from_password()
{
    std::string key;
    char* buf = (char*)malloc(SIZE_OF_RETKEY + 1);
    if (!buf) {
        std::cerr << "Mem wrong!" << std::endl;
        exit(2);
    }
    memset(buf, 0, SIZE_OF_RETKEY+1);
    std::string pwd;
    while (true) {
        pwd = getpasswd("Input password: ");
        std::string pwd2 = getpasswd("\nConfirm password: ");
        if (pwd == pwd2)
            break;
        else
            std::cerr << "\nPassword does not match!" << std::endl;
    }
    key.resize(LEN_OF_RETKEY);
    gpg_error_t err = gcry_kdf_derive(pwd.c_str(),
        pwd.size(), GCRY_KDF_PBKDF2, GCRY_MD_SHA512,
        SALT, sizeof(SALT), ITER,
        LEN_OF_RETKEY, buf);
    if (err) {
        std::cerr<<"a error occured in get_key_from_password()\n";
        fprintf(stderr, "error id = %d\n", err);
        exit(2);
    }
    key += buf;
    return key;
}

size_t get_file_size(const char* filename)
{
    struct stat file_st;
    stat(filename, &file_st);
    return file_st.st_size;
}

void encrypt(const char* filename)
{
    //gcry_cipher_hd_t cipher_hd;
    //gcry_error_t err=0;
    //size_t file_size = get_file_size(filename);
    //std::ifstream input(filename);
    //if (!input.is_open())
    //{
    //    std::cerr << "No such file!" << std::endl;
    //    exit(1);
    //}
    //std::string file_content((std::istreambuf_iterator<char>(input)),
    //    std::istreambuf_iterator<char>());
    //input.close();
    //auto Algo_type = GCRY_CIPHER_AES256;
    //// get buffer
    //size_t key_size = gcry_cipher_get_algo_keylen(Algo_type);
    //size_t block_size = gcry_cipher_get_algo_blklen(Algo_type);
    //size_t block_required = file_size / block_size;
    //if (file_size % block_size != 0) {
    //    block_required++;
    //}
    //char* cipher_buffer = (char*)malloc(block_size * block_required);
    //if (!cipher_buffer) {
    //    std::cerr << "Mem wrong!" << std::endl;
    //    exit(1);
    //}
    //memset(cipher_buffer, 0, block_size * block_required);

    //// get key
    //std::string key = get_key_from_password();
    //// open cipher
    //err = gcry_cipher_open(&cipher_hd, Algo_type, GCRY_CIPHER_MODE_CBC, GCRY_CIPHER_CBC_CTS);
    //if (err) {
    //    cipher_error_handler(err);
    //}
    //// set key
    //err = gcry_cipher_setkey(cipher_hd,key.c_str(),key_size);
    //if (err) {
    //    cipher_error_handler(err);
    //}
    //// set iv
    //err = gcry_cipher_setiv(cipher_hd, MAGIC_STRING,block_size);
    //if (err) {
    //    cipher_error_handler(err);
    //}
    //_memccpy(cipher_buffer, file_content.c_str(), file_content.size(), file_content.size());
    //err = gcry_cipher_encrypt(cipher_hd, cipher_buffer, block_required * block_size, NULL, 0);
    //if (err) {
    //    cipher_error_handler(err);
    //}
    //// test

    //std::string ofilename;
    //ofilename = filename;
    //ofilename+=".cpt";
    //std::ofstream output(ofilename,std::ios::binary);
    //output << cipher_buffer;
    //gcry_cipher_close(cipher_hd);
    //output.close();
    auto CIPHER_ALGO = GCRY_CIPHER_AES256;
    FILE* fin = fopen(filename, "rb");
    FILE* fout;

    gcry_cipher_hd_t cipher_hd;
    gcry_error_t cipher_err;

    int file_size = get_file_size(filename);
    char* input_buf = (char*)malloc(file_size);
    memset(input_buf, 0, file_size);
    // plain text buffer

    size_t key_size = gcry_cipher_get_algo_keylen(CIPHER_ALGO);
    size_t block_size = gcry_cipher_get_algo_blklen(CIPHER_ALGO);
    size_t block_required = file_size / block_size;
    if (file_size % block_size != 0) {
        block_required++;
    }
    char* cipher_buffer = (char*)malloc(block_size * block_required);
    memset(cipher_buffer, 0, block_size * block_required);

    char* iv = (char*)malloc(block_size);
    memset(iv, 0, block_size);
    memcpy(iv, MAGIC_STRING, sizeof(MAGIC_STRING));

    std::string key = get_key_from_password();
    //open cipher
    cipher_err = gcry_cipher_open(&cipher_hd, CIPHER_ALGO,
        GCRY_CIPHER_MODE_CBC, GCRY_CIPHER_CBC_CTS);
    if (cipher_err) {
        cipher_error_handler(cipher_err);
    }

    //set key
    cipher_err = gcry_cipher_setkey(cipher_hd, key.c_str(), key_size);
    if (cipher_err) {
        cipher_error_handler(cipher_err);
    }

    //set iv
    cipher_err = gcry_cipher_setiv(cipher_hd, iv, block_size);
    if (cipher_err) {
        cipher_error_handler(cipher_err);
    }

    //这里改成向ADS写入
    int flag=add_hidefile(filename);//创建隐藏文件
    if (flag) {
        //ADS创建成功
        char stream_name[MAX_PATH + 36];
        strcpy(stream_name, filename);
        strcat(stream_name, ":hide:$DATA");

        fout = fopen(stream_name, "wb");
        //这里少一个判断是否打开成功
        //encrypt
        fread(input_buf, 1, file_size, fin);
        memcpy(cipher_buffer, input_buf, block_required * block_size);
        cipher_err = gcry_cipher_encrypt(cipher_hd, cipher_buffer,
            block_required * block_size, NULL, 0);
        if (cipher_err) {
            cipher_error_handler(cipher_err);
        }
        fwrite(cipher_buffer, 1, block_required * block_size, fout);
        gcry_cipher_close(cipher_hd);
        fclose(fout);
    }
    fclose(fin);
}

void decrypt(const char* filename)
{
    //这里把读文件改成读ADS
    char stream_name[MAX_PATH + 36];
    strcpy(stream_name, filename);
    strcat(stream_name, ":hide:$DATA");//ADS名
    gcry_cipher_hd_t cipher_hd;
    gcry_error_t err = 0;
    size_t file_size = get_file_size(stream_name);
    std::ifstream input(stream_name,std::ios::binary);
    if (!input.is_open())
    {
        std::cerr << "No such file!" << std::endl;
        exit(1);
    }
    std::string file_content((std::istreambuf_iterator<char>(input)),
        std::istreambuf_iterator<char>());
    input.close();
    auto Algo_type = GCRY_CIPHER_AES256;
    // get buffer
    size_t key_size = gcry_cipher_get_algo_keylen(Algo_type);
    size_t block_size = gcry_cipher_get_algo_blklen(Algo_type);
    /*size_t block_required = file_size / block_size;
    if (file_size % block_size != 0) {
        block_required++;
    }*/
    char* cipher_buffer = (char*)malloc(file_size);
    if (!cipher_buffer) {
        std::cerr << "Mem wrong!" << std::endl;
        exit(1);
    }
    //memset(cipher_buffer, 0, block_size * block_required);

    // get key
    std::string key = get_key_from_password();
    // open cipher
    err = gcry_cipher_open(&cipher_hd, Algo_type, GCRY_CIPHER_MODE_CBC, GCRY_CIPHER_CBC_CTS);
    if (err) {
        cipher_error_handler(err);
    }
    // set key
    err = gcry_cipher_setkey(cipher_hd, key.c_str(), key_size);
    if (err) {
        cipher_error_handler(err);
    }
    // set iv
    err = gcry_cipher_setiv(cipher_hd, MAGIC_STRING, block_size);
    if (err) {
        cipher_error_handler(err);
    }
    // decrypt
    _memccpy(cipher_buffer, file_content.c_str(), file_content.size(), file_content.size());
    err = gcry_cipher_decrypt(cipher_hd, cipher_buffer, file_size, NULL, 0);
    if (err) {
        cipher_error_handler(err);
    }

    while (!cipher_buffer[file_size - 1]) {
        file_size--;
    }

    // test

    std::string ofilename;
    ofilename = filename;
    ofilename += ".dec";
    //std::ofstream output(ofilename,std::ios::binary);
    //output << cipher_buffer;
    fstream outfile;
    outfile.open(ofilename, ios::out);
    if (!outfile.is_open()) {
        std::cerr << "outfile not exists" << std::endl;
        exit(1);
    }
    outfile << cipher_buffer;
    gcry_cipher_close(cipher_hd);
    outfile.close();
}