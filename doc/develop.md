# 开发文档

## 1 libgcrypt

这是一个由GunPG组织开发的一个开源底层加密算法库，其特点在于完全线程安全。gcrypt可以作为VeraCrypt的前身TureCrypt实现的依赖库。这里采用了libgcrypt的静态库与动态库实现了windows平台上的编译。

[Documentation (The Libgcrypt Reference Manual) (gnupg.org)](https://www.gnupg.org/documentation/manuals/gcrypt/index.html#SEC_Contents)

### 1.1 使用libgcrypt

- 所有接口在头文件`gcrypt.h`中定义

  ```cpp
  #include<gcrypt.h>
  ```

  

- 在使用gcrypt库前，必须对其进行初始化。通过调用`gcry_check_version`实现

- 在使用密码算法时，必须分配一个相应的句柄，可使用打开函数完成
  ```cpp
  gcry_error_t gcry_cipher_open(gcry_cipher_hd_t* hd,int algo,int mode,unsigned int flags)
  ```

## 2 需求分析

目标为实现VeraCrypt类似的功能，对文件（多个）进行加密。在本次Demo的开发中，出于时间以及开发精力的多方面考虑，我们不实现VeraCrypt中的加密卷的功能。在本次Demo中，我们实现了对单个文件的加密，并将加密的内容隐藏到某个文件的ADS中。

在加密部分上，我们与VeraCrypt这样的比较完善的工程差距较大，考虑到后续开发的可能，可在确定方向后继续完善。

所以，本Demo实现的功能有

- 使用命令行对文件进行加密
- 可以选择对称加密的算法
- 对文件进行解密

