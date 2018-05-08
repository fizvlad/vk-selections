# Подключение curl к Code::Blocks

1. Скачать **исходники curl**
2. Скачать **OpenSSL**
3. *autoconf*
4. *configure --with-winssl=PATH_TO_SSL*
5. *mingw32-make* (Из MSYS)
6. Добавить в Linker settings: **libcurl.a, libcurl.dll.a**
7. Добавить в Search directories:  
Compiler: .../curl/include  
Linker: .../curl/lib/.libs
