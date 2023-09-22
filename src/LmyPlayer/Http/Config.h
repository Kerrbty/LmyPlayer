#ifndef _HTTP_CONFIG_HEADER_201904141545_H_
#define _HTTP_CONFIG_HEADER_201904141545_H_
#include <Windows.h>
// #define USE_WINHTTP

#ifdef USE_WINHTTP 
    // 适 server 端编程 
    // Windows XP，Windows 2000 Professional SP3 及以后版本 
    // win8 开始启用websocket协议，例如：WinHttpWebSocketSend和WinHttpWebSocketReceive 
    #include <winhttp.h>
    #pragma comment(lib, "winhttp.lib")
#else // USE_WINHTTP 
    // 适合 client 端编程 
    // Windows XP和Windows Server 2003 R2及更早版本 
    // WinINet不支持服务器实现,对于服务器实现或服务使用WinHTTP 
    #include <WinInet.h>
    #pragma comment(lib, "wininet.lib")
#endif // USE_WINHTTP 

#endif // _HTTP_CONFIG_HEADER_201904141545_H_