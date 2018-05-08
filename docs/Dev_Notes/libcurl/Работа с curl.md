# Работа с библиотекой curl для C++

## Общая схема работы
**Подключение**  
`#include <curl/curl.h>`

**Инициализация**  
``` C++
    CURL *curl; // Объект curl  
    curl = curl_easy_init(); // Инициализация
```  
*easy* в данном случае обозначает, что используется однопоточная версия. Также есть версия *multi* для асинхронной работы.

**Настройка**  
Настройка происходит с помощью вызова функций вида  
`CURLcode curl_easy_setopt(CURL *handle, CURLoption option, parameter);`  
*Подробнее позже.*

**Запрос**  
`CURLcode curl_easy_perform(CURL * easy_handle);`  
Производит запрос по ранее заданным настройкам.  
Можно использовать один и тот же handle для нескольких запросов подряд *(но не одновременно)*. Иногда это даже полезно и приводит к ускорению работы.

**Закрытие соединения**  
`void curl_easy_cleanup(CURL * handle);`  
**Обязательно** должен быть вызван после выполнения всех действий. Закрывает все соединения.

--------------------

## Опции запроса

### Поведение

*[CURLOPT_VERBOSE](https://curl.haxx.se/libcurl/c/CURLOPT_VERBOSE.html)* - Display verbose information

*[CURLOPT_HEADER](https://curl.haxx.se/libcurl/c/CURLOPT_HEADER.html)* - Include the header in the body output

*[CURLOPT_NOPROGRESS](https://curl.haxx.se/libcurl/c/CURLOPT_NOPROGRESS.html)* - Shut off the progress meter

*[CURLOPT_NOSIGNAL](https://curl.haxx.se/libcurl/c/CURLOPT_NOSIGNAL.html)* - Do not install signal handlers

*[CURLOPT_WILDCARDMATCH](https://curl.haxx.se/libcurl/c/CURLOPT_WILDCARDMATCH.html)* - Transfer multiple files according to a file name pattern

### Callback

*[CURLOPT_WRITEFUNCTION](https://curl.haxx.se/libcurl/c/CURLOPT_WRITEFUNCTION.html)* - Callback for writing data

*[CURLOPT_WRITEDATA](https://curl.haxx.se/libcurl/c/CURLOPT_WRITEDATA.html)* - Data pointer to pass to the write callback

*[CURLOPT_READFUNCTION](https://curl.haxx.se/libcurl/c/CURLOPT_READFUNCTION.html)* - Callback for reading data

*[CURLOPT_READDATA](https://curl.haxx.se/libcurl/c/CURLOPT_READDATA.html)* - Data pointer to pass to the read callback

*[CURLOPT_IOCTLFUNCTION](https://curl.haxx.se/libcurl/c/CURLOPT_IOCTLFUNCTION.html)* - Callback for I/O operations

*[CURLOPT_IOCTLDATA](https://curl.haxx.se/libcurl/c/CURLOPT_IOCTLDATA.html)* - Data pointer to pass to the I/O callback

*[CURLOPT_SEEKFUNCTION](https://curl.haxx.se/libcurl/c/CURLOPT_SEEKFUNCTION.html)* - Callback for seek operations

*[CURLOPT_SEEKDATA](https://curl.haxx.se/libcurl/c/CURLOPT_SEEKDATA.html)* - Data pointer to pass to the seek callback

*[CURLOPT_SOCKOPTFUNCTION](https://curl.haxx.se/libcurl/c/CURLOPT_SOCKOPTFUNCTION.html)* - Callback for sockopt operations

*[CURLOPT_SOCKOPTDATA](https://curl.haxx.se/libcurl/c/CURLOPT_SOCKOPTDATA.html)* - Data pointer to pass to the sockopt callback

*[CURLOPT_OPENSOCKETFUNCTION](https://curl.haxx.se/libcurl/c/CURLOPT_OPENSOCKETFUNCTION.html)* - Callback for socket creation

*[CURLOPT_OPENSOCKETDATA](https://curl.haxx.se/libcurl/c/CURLOPT_OPENSOCKETDATA.html)* - Data pointer to pass to the open socket callback

*[CURLOPT_CLOSESOCKETFUNCTION](https://curl.haxx.se/libcurl/c/CURLOPT_CLOSESOCKETFUNCTION.html)* - Callback for closing socket

*[CURLOPT_CLOSESOCKETDATA](https://curl.haxx.se/libcurl/c/CURLOPT_CLOSESOCKETDATA.html)* - Data pointer to pass to the close socket callback

*[CURLOPT_PROGRESSFUNCTION](https://curl.haxx.se/libcurl/c/CURLOPT_PROGRESSFUNCTION.html)* - OBSOLETE callback for progress meter

*[CURLOPT_PROGRESSDATA](https://curl.haxx.se/libcurl/c/CURLOPT_PROGRESSDATA.html)* - Data pointer to pass to the progress meter callback

*[CURLOPT_XFERINFOFUNCTION](https://curl.haxx.se/libcurl/c/CURLOPT_XFERINFOFUNCTION.html)* - Callback for progress meter

*[CURLOPT_XFERINFODATA](https://curl.haxx.se/libcurl/c/CURLOPT_XFERINFODATA.html)* - Data pointer to pass to the progress meter callback

*[CURLOPT_HEADERFUNCTION](https://curl.haxx.se/libcurl/c/CURLOPT_HEADERFUNCTION.html)* - Callback for writing received headers

*[CURLOPT_HEADERDATA](https://curl.haxx.se/libcurl/c/CURLOPT_HEADERDATA.html)* - Data pointer to pass to the header callback

*[CURLOPT_DEBUGFUNCTION](https://curl.haxx.se/libcurl/c/CURLOPT_DEBUGFUNCTION.html)* - Callback for debug information

*[CURLOPT_DEBUGDATA](https://curl.haxx.se/libcurl/c/CURLOPT_DEBUGDATA.html)* - Data pointer to pass to the debug callback

*[CURLOPT_SSL_CTX_FUNCTION](https://curl.haxx.se/libcurl/c/CURLOPT_SSL_CTX_FUNCTION.html)* - Callback for SSL context logic

*[CURLOPT_SSL_CTX_DATA](https://curl.haxx.se/libcurl/c/CURLOPT_SSL_CTX_DATA.html)* - Data pointer to pass to the SSL context callback

*[CURLOPT_CONV_TO_NETWORK_FUNCTION](https://curl.haxx.se/libcurl/c/CURLOPT_CONV_TO_NETWORK_FUNCTION.html)* - Callback for code base conversion

*[CURLOPT_CONV_FROM_NETWORK_FUNCTION](https://curl.haxx.se/libcurl/c/CURLOPT_CONV_FROM_NETWORK_FUNCTION.html)* - Callback for code base conversion

*[CURLOPT_CONV_FROM_UTF8_FUNCTION](https://curl.haxx.se/libcurl/c/CURLOPT_CONV_FROM_UTF8_FUNCTION.html)* - Callback for code base conversion

*[CURLOPT_INTERLEAVEFUNCTION](https://curl.haxx.se/libcurl/c/CURLOPT_INTERLEAVEFUNCTION.html)* - Callback for RTSP interleaved data

*[CURLOPT_INTERLEAVEDATA](https://curl.haxx.se/libcurl/c/CURLOPT_INTERLEAVEDATA.html)* - Data pointer to pass to the RTSP interleave callback

*[CURLOPT_CHUNK_BGN_FUNCTION](https://curl.haxx.se/libcurl/c/CURLOPT_CHUNK_BGN_FUNCTION.html)* - Callback for wildcard download start of chunk

*[CURLOPT_CHUNK_END_FUNCTION](https://curl.haxx.se/libcurl/c/CURLOPT_CHUNK_END_FUNCTION.html)* - Callback for wildcard download end of chunk

*[CURLOPT_CHUNK_DATA](https://curl.haxx.se/libcurl/c/CURLOPT_CHUNK_DATA.html)* - Data pointer to pass to the chunk callbacks

*[CURLOPT_FNMATCH_FUNCTION](https://curl.haxx.se/libcurl/c/CURLOPT_FNMATCH_FUNCTION.html)* - Callback for wildcard matching

*[CURLOPT_FNMATCH_DATA](https://curl.haxx.se/libcurl/c/CURLOPT_FNMATCH_DATA.html)* - Data pointer to pass to the wildcard matching callback

*[CURLOPT_SUPPRESS_CONNECT_HEADERS](https://curl.haxx.se/libcurl/c/CURLOPT_SUPPRESS_CONNECT_HEADERS.html)* - Suppress proxy CONNECT response headers from user callbacks

### Ошибки

*[CURLOPT_ERRORBUFFER](https://curl.haxx.se/libcurl/c/CURLOPT_ERRORBUFFER.html)* - Error message buffer

*[CURLOPT_STDERR](https://curl.haxx.se/libcurl/c/CURLOPT_STDERR.html)* - stderr replacement stream

*[CURLOPT_FAILONERROR](https://curl.haxx.se/libcurl/c/CURLOPT_FAILONERROR.html)* - Fail on HTTP 4xx errors

*[CURLOPT_KEEP_SENDING_ON_ERROR](https://curl.haxx.se/libcurl/c/CURLOPT_KEEP_SENDING_ON_ERROR.html)* - Keep sending on HTTP &gt;= 300 errors

### Сетевые настройки

*[CURLOPT_URL](https://curl.haxx.se/libcurl/c/CURLOPT_URL.html)* - URL to work on

*[CURLOPT_PATH_AS_IS](https://curl.haxx.se/libcurl/c/CURLOPT_PATH_AS_IS.html)* - Disable squashing /../ and /./ sequences in the path

*[CURLOPT_PROTOCOLS](https://curl.haxx.se/libcurl/c/CURLOPT_PROTOCOLS.html)* - Allowed protocols

*[CURLOPT_REDIR_PROTOCOLS](https://curl.haxx.se/libcurl/c/CURLOPT_REDIR_PROTOCOLS.html)* - Protocols to allow redirects to

*[CURLOPT_DEFAULT_PROTOCOL](https://curl.haxx.se/libcurl/c/CURLOPT_DEFAULT_PROTOCOL.html)* - Default protocol

*[CURLOPT_PROXY](https://curl.haxx.se/libcurl/c/CURLOPT_PROXY.html)* - Proxy to use

*[CURLOPT_PRE_PROXY](https://curl.haxx.se/libcurl/c/CURLOPT_PRE_PROXY.html)* - Socks proxy to use

*[CURLOPT_PROXYPORT](https://curl.haxx.se/libcurl/c/CURLOPT_PROXYPORT.html)* - Proxy port to use

*[CURLOPT_PROXYTYPE](https://curl.haxx.se/libcurl/c/CURLOPT_PROXYTYPE.html)* - Proxy type

*[CURLOPT_NOPROXY](https://curl.haxx.se/libcurl/c/CURLOPT_NOPROXY.html)* - Filter out hosts from proxy use

*[CURLOPT_HTTPPROXYTUNNEL](https://curl.haxx.se/libcurl/c/CURLOPT_HTTPPROXYTUNNEL.html)* - Tunnel through the HTTP proxy

*[CURLOPT_CONNECT_TO](https://curl.haxx.se/libcurl/c/CURLOPT_CONNECT_TO.html)* - Connect to a specific host and port

*[CURLOPT_SOCKS5_AUTH](https://curl.haxx.se/libcurl/c/CURLOPT_SOCKS5_AUTH.html)* - Socks5 authentication methods

*[CURLOPT_SOCKS5_GSSAPI_SERVICE](https://curl.haxx.se/libcurl/c/CURLOPT_SOCKS5_GSSAPI_SERVICE.html)* - Socks5 GSSAPI service name

*[CURLOPT_SOCKS5_GSSAPI_NEC](https://curl.haxx.se/libcurl/c/CURLOPT_SOCKS5_GSSAPI_NEC.html)* - Socks5 GSSAPI NEC mode

*[CURLOPT_PROXY_SERVICE_NAME](https://curl.haxx.se/libcurl/c/CURLOPT_PROXY_SERVICE_NAME.html)* - Proxy authentication service name

*[CURLOPT_SERVICE_NAME](https://curl.haxx.se/libcurl/c/CURLOPT_SERVICE_NAME.html)* - Authentication service name

*[CURLOPT_INTERFACE](https://curl.haxx.se/libcurl/c/CURLOPT_INTERFACE.html)* - Bind connection locally to this

*[CURLOPT_LOCALPORT](https://curl.haxx.se/libcurl/c/CURLOPT_LOCALPORT.html)* - Bind connection locally to this port

*[CURLOPT_LOCALPORTRANGE](https://curl.haxx.se/libcurl/c/CURLOPT_LOCALPORTRANGE.html)* - Bind connection locally to port range

*[CURLOPT_DNS_CACHE_TIMEOUT](https://curl.haxx.se/libcurl/c/CURLOPT_DNS_CACHE_TIMEOUT.html)* - Timeout for DNS cache

*[CURLOPT_DNS_USE_GLOBAL_CACHE](https://curl.haxx.se/libcurl/c/CURLOPT_DNS_USE_GLOBAL_CACHE.html)* - OBSOLETE Enable global DNS cache

*[CURLOPT_BUFFERSIZE](https://curl.haxx.se/libcurl/c/CURLOPT_BUFFERSIZE.html)* - Ask for alternate buffer size

*[CURLOPT_PORT](https://curl.haxx.se/libcurl/c/CURLOPT_PORT.html)* - Port number to connect to

*[CURLOPT_TCP_FASTOPEN](https://curl.haxx.se/libcurl/c/CURLOPT_TCP_FASTOPEN.html)* - Enable TFO, TCP Fast Open

*[CURLOPT_TCP_NODELAY](https://curl.haxx.se/libcurl/c/CURLOPT_TCP_NODELAY.html)* - Disable the Nagle algorithm

*[CURLOPT_ADDRESS_SCOPE](https://curl.haxx.se/libcurl/c/CURLOPT_ADDRESS_SCOPE.html)* - IPv6 scope for local addresses

*[CURLOPT_TCP_KEEPALIVE](https://curl.haxx.se/libcurl/c/CURLOPT_TCP_KEEPALIVE.html)* - Enable TCP keep-alive

*[CURLOPT_TCP_KEEPIDLE](https://curl.haxx.se/libcurl/c/CURLOPT_TCP_KEEPIDLE.html)* - Idle time before sending keep-alive

*[CURLOPT_TCP_KEEPINTVL](https://curl.haxx.se/libcurl/c/CURLOPT_TCP_KEEPINTVL.html)* - Interval between keep-alive probes

*[CURLOPT_UNIX_SOCKET_PATH](https://curl.haxx.se/libcurl/c/CURLOPT_UNIX_SOCKET_PATH.html)* - Path to a Unix domain socket

*[CURLOPT_ABSTRACT_UNIX_SOCKET](https://curl.haxx.se/libcurl/c/CURLOPT_ABSTRACT_UNIX_SOCKET.html)* - Path to an abstract Unix domain socket

### Аутентификация

*[CURLOPT_NETRC](https://curl.haxx.se/libcurl/c/CURLOPT_NETRC.html)* - Enable .netrc parsing

*[CURLOPT_NETRC_FILE](https://curl.haxx.se/libcurl/c/CURLOPT_NETRC_FILE.html)* - .netrc file name

*[CURLOPT_USERPWD](https://curl.haxx.se/libcurl/c/CURLOPT_USERPWD.html)* - User name and password

*[CURLOPT_PROXYUSERPWD](https://curl.haxx.se/libcurl/c/CURLOPT_PROXYUSERPWD.html)* - Proxy user name and password

*[CURLOPT_USERNAME](https://curl.haxx.se/libcurl/c/CURLOPT_USERNAME.html)* - User name

*[CURLOPT_PASSWORD](https://curl.haxx.se/libcurl/c/CURLOPT_PASSWORD.html)* - Password

*[CURLOPT_LOGIN_OPTIONS](https://curl.haxx.se/libcurl/c/CURLOPT_LOGIN_OPTIONS.html)* - Login options

*[CURLOPT_PROXYUSERNAME](https://curl.haxx.se/libcurl/c/CURLOPT_PROXYUSERNAME.html)* - Proxy user name

*[CURLOPT_PROXYPASSWORD](https://curl.haxx.se/libcurl/c/CURLOPT_PROXYPASSWORD.html)* - Proxy password

*[CURLOPT_HTTPAUTH](https://curl.haxx.se/libcurl/c/CURLOPT_HTTPAUTH.html) - HTTP* server authentication methods

*[CURLOPT_TLSAUTH_USERNAME](https://curl.haxx.se/libcurl/c/CURLOPT_TLSAUTH_USERNAME.html)* - TLS authentication user name

*[CURLOPT_PROXY_TLSAUTH_USERNAME](https://curl.haxx.se/libcurl/c/CURLOPT_PROXY_TLSAUTH_USERNAME.html)* - Proxy TLS authentication user name

*[CURLOPT_TLSAUTH_PASSWORD](https://curl.haxx.se/libcurl/c/CURLOPT_TLSAUTH_PASSWORD.html)* - TLS authentication password

*[CURLOPT_PROXY_TLSAUTH_PASSWORD](https://curl.haxx.se/libcurl/c/CURLOPT_PROXY_TLSAUTH_PASSWORD.html)* - Proxy TLS authentication password

*[CURLOPT_TLSAUTH_TYPE](https://curl.haxx.se/libcurl/c/CURLOPT_TLSAUTH_TYPE.html)* - TLS authentication methods

*[CURLOPT_PROXY_TLSAUTH_TYPE](https://curl.haxx.se/libcurl/c/CURLOPT_PROXY_TLSAUTH_TYPE.html)* - Proxy TLS authentication methods

*[CURLOPT_PROXYAUTH](https://curl.haxx.se/libcurl/c/CURLOPT_PROXYAUTH.html)* - HTTP proxy authentication methods

*[CURLOPT_SASL_IR](https://curl.haxx.se/libcurl/c/CURLOPT_SASL_IR.html)* - Enable SASL initial response

*[CURLOPT_XOAUTH2_BEARER](https://curl.haxx.se/libcurl/c/CURLOPT_XOAUTH2_BEARER.html)* - OAuth2 bearer token

### HTTP

*[CURLOPT_AUTOREFERER](https://curl.haxx.se/libcurl/c/CURLOPT_AUTOREFERER.html)* - Automatically set Referer: header

*[CURLOPT_ACCEPT_ENCODING](https://curl.haxx.se/libcurl/c/CURLOPT_ACCEPT_ENCODING.html)* - Accept-Encoding and automatic decompressing data

*[CURLOPT_TRANSFER_ENCODING](https://curl.haxx.se/libcurl/c/CURLOPT_TRANSFER_ENCODING.html)* - Request Transfer-Encoding

*[CURLOPT_FOLLOWLOCATION](https://curl.haxx.se/libcurl/c/CURLOPT_FOLLOWLOCATION.html)* - Follow HTTP redirects

*[CURLOPT_UNRESTRICTED_AUTH](https://curl.haxx.se/libcurl/c/CURLOPT_UNRESTRICTED_AUTH.html)* - Do not restrict authentication to original host

*[CURLOPT_MAXREDIRS](https://curl.haxx.se/libcurl/c/CURLOPT_MAXREDIRS.html)* - Maximum number of redirects to follow

*[CURLOPT_POSTREDIR](https://curl.haxx.se/libcurl/c/CURLOPT_POSTREDIR.html)* - How to act on redirects after POST

*[CURLOPT_PUT](https://curl.haxx.se/libcurl/c/CURLOPT_PUT.html)* - Issue a HTTP PUT request

*[CURLOPT_POST](https://curl.haxx.se/libcurl/c/CURLOPT_POST.html)* - Issue a HTTP POST request

*[CURLOPT_POSTFIELDS](https://curl.haxx.se/libcurl/c/CURLOPT_POSTFIELDS.html)* - Send a POST with this data

*[CURLOPT_POSTFIELDSIZE](https://curl.haxx.se/libcurl/c/CURLOPT_POSTFIELDSIZE.html)* - The POST data is this big

*[CURLOPT_POSTFIELDSIZE_LARGE](https://curl.haxx.se/libcurl/c/CURLOPT_POSTFIELDSIZE_LARGE.html)* - The POST data is this big

*[CURLOPT_COPYPOSTFIELDS](https://curl.haxx.se/libcurl/c/CURLOPT_COPYPOSTFIELDS.html)* - Send a POST with this data - and copy it

*[CURLOPT_HTTPPOST](https://curl.haxx.se/libcurl/c/CURLOPT_HTTPPOST.html)* - Multipart formpost HTTP POST

*[CURLOPT_REFERER](https://curl.haxx.se/libcurl/c/CURLOPT_REFERER.html)* - Referer: header

*[CURLOPT_USERAGENT](https://curl.haxx.se/libcurl/c/CURLOPT_USERAGENT.html)* - User-Agent: header

*[CURLOPT_HTTPHEADER](https://curl.haxx.se/libcurl/c/CURLOPT_HTTPHEADER.html)* - Custom HTTP headers

*[CURLOPT_HEADEROPT](https://curl.haxx.se/libcurl/c/CURLOPT_HEADEROPT.html)* - Control custom headers

*[CURLOPT_PROXYHEADER](https://curl.haxx.se/libcurl/c/CURLOPT_PROXYHEADER.html)* - Custom HTTP headers sent to proxy

*[CURLOPT_HTTP200ALIASES](https://curl.haxx.se/libcurl/c/CURLOPT_HTTP200ALIASES.html)* - Alternative versions of 200 OK

*[CURLOPT_COOKIE](https://curl.haxx.se/libcurl/c/CURLOPT_COOKIE.html)* - Cookie(s) to send

*[CURLOPT_COOKIEFILE](https://curl.haxx.se/libcurl/c/CURLOPT_COOKIEFILE.html)* - File to read cookies from

*[CURLOPT_COOKIEJAR](https://curl.haxx.se/libcurl/c/CURLOPT_COOKIEJAR.html)* - File to write cookies to

*[CURLOPT_COOKIESESSION](https://curl.haxx.se/libcurl/c/CURLOPT_COOKIESESSION.html)* - Start a new cookie session

*[CURLOPT_COOKIELIST](https://curl.haxx.se/libcurl/c/CURLOPT_COOKIELIST.html)* - Add or control cookies

*[CURLOPT_HTTPGET](https://curl.haxx.se/libcurl/c/CURLOPT_HTTPGET.html)* - Do a HTTP GET request

*[CURLOPT_REQUEST_TARGET](https://curl.haxx.se/libcurl/c/CURLOPT_REQUEST_TARGET.html)* - Set the request target

*[CURLOPT_HTTP_VERSION](https://curl.haxx.se/libcurl/c/CURLOPT_HTTP_VERSION.html)* - HTTP version to use

*[CURLOPT_IGNORE_CONTENT_LENGTH](https://curl.haxx.se/libcurl/c/CURLOPT_IGNORE_CONTENT_LENGTH.html)* - Ignore Content-Length

*[CURLOPT_HTTP_CONTENT_DECODING](https://curl.haxx.se/libcurl/c/CURLOPT_HTTP_CONTENT_DECODING.html)* - Disable Content decoding

*[CURLOPT_HTTP_TRANSFER_DECODING](https://curl.haxx.se/libcurl/c/CURLOPT_HTTP_TRANSFER_DECODING.html)* - Disable Transfer decoding

*[CURLOPT_EXPECT_100_TIMEOUT_MS](https://curl.haxx.se/libcurl/c/CURLOPT_EXPECT_100_TIMEOUT_MS.html)* - 100-continue timeout

*[CURLOPT_PIPEWAIT](https://curl.haxx.se/libcurl/c/CURLOPT_PIPEWAIT.html)* - Wait on connection to pipeline on it

*[CURLOPT_STREAM_DEPENDS](https://curl.haxx.se/libcurl/c/CURLOPT_STREAM_DEPENDS.html)* - This HTTP/2 stream depends on another

*[CURLOPT_STREAM_DEPENDS_E](https://curl.haxx.se/libcurl/c/CURLOPT_STREAM_DEPENDS_E.html)* - This HTTP/2 stream depends on another exclusively

*[CURLOPT_STREAM_WEIGHT](https://curl.haxx.se/libcurl/c/CURLOPT_STREAM_WEIGHT.html)* - Set this HTTP/2 stream's weight

### SMTP

*[CURLOPT_MAIL_FROM](https://curl.haxx.se/libcurl/c/CURLOPT_MAIL_FROM.html)* - Address of the sender

*[CURLOPT_MAIL_RCPT](https://curl.haxx.se/libcurl/c/CURLOPT_MAIL_RCPT.html)* - Address of the recipients

*[CURLOPT_MAIL_AUTH](https://curl.haxx.se/libcurl/c/CURLOPT_MAIL_AUTH.html)* - Authentication address

### TFTP

*[CURLOPT_TFTP_BLKSIZE](https://curl.haxx.se/libcurl/c/CURLOPT_TFTP_BLKSIZE.html)* - TFTP block size

*[CURLOPT_TFTP_NO_OPTIONS](https://curl.haxx.se/libcurl/c/CURLOPT_TFTP_NO_OPTIONS.html)* - Do not send TFTP options requests

### FTP

*[CURLOPT_FTPPORT](https://curl.haxx.se/libcurl/c/CURLOPT_FTPPORT.html)* - Use active FTP

*[CURLOPT_QUOTE](https://curl.haxx.se/libcurl/c/CURLOPT_QUOTE.html)* - Commands to run before transfer

*[CURLOPT_POSTQUOTE](https://curl.haxx.se/libcurl/c/CURLOPT_POSTQUOTE.html)* - Commands to run after transfer

*[CURLOPT_PREQUOTE](https://curl.haxx.se/libcurl/c/CURLOPT_PREQUOTE.html)* - Commands to run just before transfer

*[CURLOPT_APPEND](https://curl.haxx.se/libcurl/c/CURLOPT_APPEND.html)* - Append to remote file

*[CURLOPT_FTP_USE_EPRT](https://curl.haxx.se/libcurl/c/CURLOPT_FTP_USE_EPRT.html)* - Use EPTR

*[CURLOPT_FTP_USE_EPSV](https://curl.haxx.se/libcurl/c/CURLOPT_FTP_USE_EPSV.html)* - Use EPSV

*[CURLOPT_FTP_USE_PRET](https://curl.haxx.se/libcurl/c/CURLOPT_FTP_USE_PRET.html)* - Use PRET

*[CURLOPT_FTP_CREATE_MISSING_DIRS](https://curl.haxx.se/libcurl/c/CURLOPT_FTP_CREATE_MISSING_DIRS.html)* - Create missing directories on the remote server

*[CURLOPT_FTP_RESPONSE_TIMEOUT](https://curl.haxx.se/libcurl/c/CURLOPT_FTP_RESPONSE_TIMEOUT.html)* - Timeout for FTP responses

*[CURLOPT_FTP_ALTERNATIVE_TO_USER](https://curl.haxx.se/libcurl/c/CURLOPT_FTP_ALTERNATIVE_TO_USER.html)* - Alternative to USER

*[CURLOPT_FTP_SKIP_PASV_IP](https://curl.haxx.se/libcurl/c/CURLOPT_FTP_SKIP_PASV_IP.html)* - Ignore the IP address in the PASV response

*[CURLOPT_FTPSSLAUTH](https://curl.haxx.se/libcurl/c/CURLOPT_FTPSSLAUTH.html)* - Control how to do TLS

*[CURLOPT_FTP_SSL_CCC](https://curl.haxx.se/libcurl/c/CURLOPT_FTP_SSL_CCC.html)* - Back to non-TLS again after authentication

*[CURLOPT_FTP_ACCOUNT](https://curl.haxx.se/libcurl/c/CURLOPT_FTP_ACCOUNT.html)* - Send ACCT command

*[CURLOPT_FTP_FILEMETHOD](https://curl.haxx.se/libcurl/c/CURLOPT_FTP_FILEMETHOD.html)* - Specify how to reach files

### RTSP

*[CURLOPT_RTSP_REQUEST](https://curl.haxx.se/libcurl/c/CURLOPT_RTSP_REQUEST.html)* - RTSP request

*[CURLOPT_RTSP_SESSION_ID](https://curl.haxx.se/libcurl/c/CURLOPT_RTSP_SESSION_ID.html)* - RTSP session-id

*[CURLOPT_RTSP_STREAM_URI](https://curl.haxx.se/libcurl/c/CURLOPT_RTSP_STREAM_URI.html)* - RTSP stream URI

*[CURLOPT_RTSP_TRANSPORT](https://curl.haxx.se/libcurl/c/CURLOPT_RTSP_TRANSPORT.html)* - RTSP Transport: header

*[CURLOPT_RTSP_CLIENT_CSEQ](https://curl.haxx.se/libcurl/c/CURLOPT_RTSP_CLIENT_CSEQ.html)* - Client CSEQ number

*[CURLOPT_RTSP_SERVER_CSEQ](https://curl.haxx.se/libcurl/c/CURLOPT_RTSP_SERVER_CSEQ.html)* - CSEQ number for RTSP Server-&gt;Client request

### Протокол

*[CURLOPT_TRANSFERTEXT](https://curl.haxx.se/libcurl/c/CURLOPT_TRANSFERTEXT.html)* - Use text transfer

*[CURLOPT_PROXY_TRANSFER_MODE](https://curl.haxx.se/libcurl/c/CURLOPT_PROXY_TRANSFER_MODE.html)* - Add transfer mode to URL over proxy

*[CURLOPT_CRLF](https://curl.haxx.se/libcurl/c/CURLOPT_CRLF.html)* - Convert newlines

*[CURLOPT_RANGE](https://curl.haxx.se/libcurl/c/CURLOPT_RANGE.html)* - Range requests

*[CURLOPT_RESUME_FROM](https://curl.haxx.se/libcurl/c/CURLOPT_RESUME_FROM.html)* - Resume a transfer

*[CURLOPT_RESUME_FROM_LARGE](https://curl.haxx.se/libcurl/c/CURLOPT_RESUME_FROM_LARGE.html)* - Resume a transfer

*[CURLOPT_CUSTOMREQUEST](https://curl.haxx.se/libcurl/c/CURLOPT_CUSTOMREQUEST.html)* - Custom request/method

*[CURLOPT_FILETIME](https://curl.haxx.se/libcurl/c/CURLOPT_FILETIME.html)* - Request file modification date and time

*[CURLOPT_DIRLISTONLY](https://curl.haxx.se/libcurl/c/CURLOPT_DIRLISTONLY.html)* - List only

*[CURLOPT_NOBODY](https://curl.haxx.se/libcurl/c/CURLOPT_NOBODY.html)* - Do not get the body contents

*[CURLOPT_INFILESIZE](https://curl.haxx.se/libcurl/c/CURLOPT_INFILESIZE.html)* - Size of file to send

*[CURLOPT_INFILESIZE_LARGE](https://curl.haxx.se/libcurl/c/CURLOPT_INFILESIZE_LARGE.html)* - Size of file to send

*[CURLOPT_UPLOAD](https://curl.haxx.se/libcurl/c/CURLOPT_UPLOAD.html)* - Upload data

*[CURLOPT_MIMEPOST](https://curl.haxx.se/libcurl/c/CURLOPT_MIMEPOST.html)* - Post/send MIME data

*[CURLOPT_MAXFILESIZE](https://curl.haxx.se/libcurl/c/CURLOPT_MAXFILESIZE.html)* - Maximum file size to get

*[CURLOPT_MAXFILESIZE_LARGE](https://curl.haxx.se/libcurl/c/CURLOPT_MAXFILESIZE_LARGE.html)* - Maximum file size to get

*[CURLOPT_TIMECONDITION](https://curl.haxx.se/libcurl/c/CURLOPT_TIMECONDITION.html)* - Make a time conditional request

*[CURLOPT_TIMEVALUE](https://curl.haxx.se/libcurl/c/CURLOPT_TIMEVALUE.html)* - Time value for the time conditional request

### Настройки соединения

*[CURLOPT_TIMEOUT](https://curl.haxx.se/libcurl/c/CURLOPT_TIMEOUT.html)* - Timeout for the entire request

*[CURLOPT_TIMEOUT_MS](https://curl.haxx.se/libcurl/c/CURLOPT_TIMEOUT_MS.html)* - Millisecond timeout for the entire request

*[CURLOPT_LOW_SPEED_LIMIT](https://curl.haxx.se/libcurl/c/CURLOPT_LOW_SPEED_LIMIT.html)* - Low speed limit to abort transfer

*[CURLOPT_LOW_SPEED_TIME](https://curl.haxx.se/libcurl/c/CURLOPT_LOW_SPEED_TIME.html)* - Time to be below the speed to trigger low speed abort

*[CURLOPT_MAX_SEND_SPEED_LARGE](https://curl.haxx.se/libcurl/c/CURLOPT_MAX_SEND_SPEED_LARGE.html)* - Cap the upload speed to this

*[CURLOPT_MAX_RECV_SPEED_LARGE](https://curl.haxx.se/libcurl/c/CURLOPT_MAX_RECV_SPEED_LARGE.html)* - Cap the download speed to this

*[CURLOPT_MAXCONNECTS](https://curl.haxx.se/libcurl/c/CURLOPT_MAXCONNECTS.html)* - Maximum number of connections in the connection pool

*[CURLOPT_FRESH_CONNECT](https://curl.haxx.se/libcurl/c/CURLOPT_FRESH_CONNECT.html)* - Use a new connection

*[CURLOPT_FORBID_REUSE](https://curl.haxx.se/libcurl/c/CURLOPT_FORBID_REUSE.html)* - Prevent subsequent connections from re-using this

*[CURLOPT_CONNECTTIMEOUT](https://curl.haxx.se/libcurl/c/CURLOPT_CONNECTTIMEOUT.html)* - Timeout for the connection phase

*[CURLOPT_CONNECTTIMEOUT_MS](https://curl.haxx.se/libcurl/c/CURLOPT_CONNECTTIMEOUT_MS.html)* - Millisecond timeout for the connection phase

*[CURLOPT_IPRESOLVE](https://curl.haxx.se/libcurl/c/CURLOPT_IPRESOLVE.html)* - IP version to resolve to

*[CURLOPT_CONNECT_ONLY](https://curl.haxx.se/libcurl/c/CURLOPT_CONNECT_ONLY.html)* - Only connect, nothing else

*[CURLOPT_USE_SSL](https://curl.haxx.se/libcurl/c/CURLOPT_USE_SSL.html)* - Use TLS/SSL

*[CURLOPT_RESOLVE](https://curl.haxx.se/libcurl/c/CURLOPT_RESOLVE.html)* - Provide fixed/fake name resolves

*[CURLOPT_DNS_INTERFACE](https://curl.haxx.se/libcurl/c/CURLOPT_DNS_INTERFACE.html)* - Bind name resolves to this interface

*[CURLOPT_DNS_LOCAL_IP4](https://curl.haxx.se/libcurl/c/CURLOPT_DNS_LOCAL_IP4.html)* - Bind name resolves to this IP4 address

*[CURLOPT_DNS_LOCAL_IP6](https://curl.haxx.se/libcurl/c/CURLOPT_DNS_LOCAL_IP6.html)* - Bind name resolves to this IP6 address

*[CURLOPT_DNS_SERVERS](https://curl.haxx.se/libcurl/c/CURLOPT_DNS_SERVERS.html)* - Preferred DNS servers

*[CURLOPT_ACCEPTTIMEOUT_MS](https://curl.haxx.se/libcurl/c/CURLOPT_ACCEPTTIMEOUT_MS.html)* - Timeout for waiting for the server's connect back to be accepted

### SSL и безопасность

*[CURLOPT_SSLCERT](https://curl.haxx.se/libcurl/c/CURLOPT_SSLCERT.html)* - Client cert

*[CURLOPT_PROXY_SSLCERT](https://curl.haxx.se/libcurl/c/CURLOPT_PROXY_SSLCERT.html)* - Proxy client cert

*[CURLOPT_SSLCERTTYPE](https://curl.haxx.se/libcurl/c/CURLOPT_SSLCERTTYPE.html)* - Client cert type

*[CURLOPT_PROXY_SSLCERTTYPE](https://curl.haxx.se/libcurl/c/CURLOPT_PROXY_SSLCERTTYPE.html)* - Proxy client cert type

*[CURLOPT_SSLKEY](https://curl.haxx.se/libcurl/c/CURLOPT_SSLKEY.html)* - Client key

*[CURLOPT_PROXY_SSLKEY](https://curl.haxx.se/libcurl/c/CURLOPT_PROXY_SSLKEY.html)* - Proxy client key

*[CURLOPT_SSLKEYTYPE](https://curl.haxx.se/libcurl/c/CURLOPT_SSLKEYTYPE.html)* - Client key type

*[CURLOPT_PROXY_SSLKEYTYPE](https://curl.haxx.se/libcurl/c/CURLOPT_PROXY_SSLKEYTYPE.html)* - Proxy client key type

*[CURLOPT_KEYPASSWD](https://curl.haxx.se/libcurl/c/CURLOPT_KEYPASSWD.html)* - Client key password

*[CURLOPT_PROXY_KEYPASSWD](https://curl.haxx.se/libcurl/c/CURLOPT_PROXY_KEYPASSWD.html)* - Proxy client key password

*[CURLOPT_SSL_ENABLE_ALPN](https://curl.haxx.se/libcurl/c/CURLOPT_SSL_ENABLE_ALPN.html)* - Enable use of ALPN

*[CURLOPT_SSL_ENABLE_NPN](https://curl.haxx.se/libcurl/c/CURLOPT_SSL_ENABLE_NPN.html)* - Enable use of NPN

*[CURLOPT_SSLENGINE](https://curl.haxx.se/libcurl/c/CURLOPT_SSLENGINE.html)* - Use identifier with SSL engine

*[CURLOPT_SSLENGINE_DEFAULT](https://curl.haxx.se/libcurl/c/CURLOPT_SSLENGINE_DEFAULT.html)* - Default SSL engine

*[CURLOPT_SSL_FALSESTART](https://curl.haxx.se/libcurl/c/CURLOPT_SSL_FALSESTART.html)* - Enable TLS False Start

*[CURLOPT_SSLVERSION](https://curl.haxx.se/libcurl/c/CURLOPT_SSLVERSION.html)* - SSL version to use

*[CURLOPT_PROXY_SSLVERSION](https://curl.haxx.se/libcurl/c/CURLOPT_PROXY_SSLVERSION.html)* - Proxy SSL version to use

*[CURLOPT_SSL_VERIFYHOST](https://curl.haxx.se/libcurl/c/CURLOPT_SSL_VERIFYHOST.html)* - Verify the host name in the SSL certificate

*[CURLOPT_PROXY_SSL_VERIFYHOST](https://curl.haxx.se/libcurl/c/CURLOPT_PROXY_SSL_VERIFYHOST.html)* - Verify the host name in the proxy SSL certificate

*[CURLOPT_SSL_VERIFYPEER](https://curl.haxx.se/libcurl/c/CURLOPT_SSL_VERIFYPEER.html)* - Verify the SSL certificate

*[CURLOPT_PROXY_SSL_VERIFYPEER](https://curl.haxx.se/libcurl/c/CURLOPT_PROXY_SSL_VERIFYPEER.html)* - Verify the proxy SSL certificate

*[CURLOPT_SSL_VERIFYSTATUS](https://curl.haxx.se/libcurl/c/CURLOPT_SSL_VERIFYSTATUS.html)* - Verify the SSL certificate's status

*[CURLOPT_CAINFO](https://curl.haxx.se/libcurl/c/CURLOPT_CAINFO.html)* - CA cert bundle

*[CURLOPT_PROXY_CAINFO](https://curl.haxx.se/libcurl/c/CURLOPT_PROXY_CAINFO.html)* - Proxy CA cert bundle

*[CURLOPT_ISSUERCERT](https://curl.haxx.se/libcurl/c/CURLOPT_ISSUERCERT.html)* - Issuer certificate

*[CURLOPT_CAPATH](https://curl.haxx.se/libcurl/c/CURLOPT_CAPATH.html)* - Path to CA cert bundle

*[CURLOPT_PROXY_CAPATH](https://curl.haxx.se/libcurl/c/CURLOPT_PROXY_CAPATH.html)* - Path to proxy CA cert bundle

*[CURLOPT_CRLFILE](https://curl.haxx.se/libcurl/c/CURLOPT_CRLFILE.html)* - Certificate Revocation List

*[CURLOPT_PROXY_CRLFILE](https://curl.haxx.se/libcurl/c/CURLOPT_PROXY_CRLFILE.html)* - Proxy Certificate Revocation List

*[CURLOPT_CERTINFO](https://curl.haxx.se/libcurl/c/CURLOPT_CERTINFO.html)* - Extract certificate info

*[CURLOPT_PINNEDPUBLICKEY](https://curl.haxx.se/libcurl/c/CURLOPT_PINNEDPUBLICKEY.html)* - Set pinned SSL public key

*[CURLOPT_PROXY_PINNEDPUBLICKEY](https://curl.haxx.se/libcurl/c/CURLOPT_PROXY_PINNEDPUBLICKEY.html)* - Set the proxy's pinned SSL public key

*[CURLOPT_RANDOM_FILE](https://curl.haxx.se/libcurl/c/CURLOPT_RANDOM_FILE.html)* - Provide source for entropy random data

*[CURLOPT_EGDSOCKET](https://curl.haxx.se/libcurl/c/CURLOPT_EGDSOCKET.html)* - Identify EGD socket for entropy

*[CURLOPT_SSL_CIPHER_LIST](https://curl.haxx.se/libcurl/c/CURLOPT_SSL_CIPHER_LIST.html)* - Ciphers to use

*[CURLOPT_PROXY_SSL_CIPHER_LIST](https://curl.haxx.se/libcurl/c/CURLOPT_PROXY_SSL_CIPHER_LIST.html)* - Proxy ciphers to use

*[CURLOPT_SSL_SESSIONID_CACHE](https://curl.haxx.se/libcurl/c/CURLOPT_SSL_SESSIONID_CACHE.html)* - Disable SSL session-id cache

*[CURLOPT_SSL_OPTIONS](https://curl.haxx.se/libcurl/c/CURLOPT_SSL_OPTIONS.html)* - Control SSL behavior

*[CURLOPT_PROXY_SSL_OPTIONS](https://curl.haxx.se/libcurl/c/CURLOPT_PROXY_SSL_OPTIONS.html)* - Control proxy SSL behavior

*[CURLOPT_KRBLEVEL](https://curl.haxx.se/libcurl/c/CURLOPT_KRBLEVEL.html)* - Kerberos security level

*[CURLOPT_GSSAPI_DELEGATION](https://curl.haxx.se/libcurl/c/CURLOPT_GSSAPI_DELEGATION.html)* - Disable GSS-API delegation

### SSH

*[CURLOPT_SSH_AUTH_TYPES](https://curl.haxx.se/libcurl/c/CURLOPT_SSH_AUTH_TYPES.html)* - SSH authentication types

*[CURLOPT_SSH_COMPRESSION](https://curl.haxx.se/libcurl/c/CURLOPT_SSH_COMPRESSION.html)* - Enable SSH compression

*[CURLOPT_SSH_HOST_PUBLIC_KEY_MD5](https://curl.haxx.se/libcurl/c/CURLOPT_SSH_HOST_PUBLIC_KEY_MD5.html)* - MD5 of host's public key

*[CURLOPT_SSH_PUBLIC_KEYFILE](https://curl.haxx.se/libcurl/c/CURLOPT_SSH_PUBLIC_KEYFILE.html)* - File name of public key

*[CURLOPT_SSH_PRIVATE_KEYFILE](https://curl.haxx.se/libcurl/c/CURLOPT_SSH_PRIVATE_KEYFILE.html)* - File name of private key

*[CURLOPT_SSH_KNOWNHOSTS](https://curl.haxx.se/libcurl/c/CURLOPT_SSH_KNOWNHOSTS.html)* - File name with known hosts

*[CURLOPT_SSH_KEYFUNCTION](https://curl.haxx.se/libcurl/c/CURLOPT_SSH_KEYFUNCTION.html)* - Callback for known hosts handling

*[CURLOPT_SSH_KEYDATA](https://curl.haxx.se/libcurl/c/CURLOPT_SSH_KEYDATA.html)* - Custom pointer to pass to ssh key callback

### Прочее

*[CURLOPT_PRIVATE](https://curl.haxx.se/libcurl/c/CURLOPT_PRIVATE.html)* - Private pointer to store

*[CURLOPT_SHARE](https://curl.haxx.se/libcurl/c/CURLOPT_SHARE.html)* - Share object to use

*[CURLOPT_NEW_FILE_PERMS](https://curl.haxx.se/libcurl/c/CURLOPT_NEW_FILE_PERMS.html)* - Mode for creating new remote files

*[CURLOPT_NEW_DIRECTORY_PERMS](https://curl.haxx.se/libcurl/c/CURLOPT_NEW_DIRECTORY_PERMS.html)* - Mode for creating new remote directories

### Telnet

*[CURLOPT_TELNETOPTIONS](https://curl.haxx.se/libcurl/c/CURLOPT_TELNETOPTIONS.html)* - TELNET options
