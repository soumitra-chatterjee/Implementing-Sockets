#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <tchar.h>

int main() {
    
    WORD wVersionRequired = MAKEWORD(2, 2);
    WSADATA wsaData;

    if(WSAStartup(wVersionRequired, &wsaData) != 0) {
        wprintf(L"failed to load winsock\n");
        getchar();
        return 1;
    }
    wprintf(L"\nloaded winsock2, version: %d.%d\n", LOBYTE(wsaData.wVersion), HIBYTE(wsaData.wVersion));

    // create socket
    SOCKET socketServer = INVALID_SOCKET;
    SOCKET socketAccept = INVALID_SOCKET;
    int af = AF_INET;
    int type = SOCK_STREAM;
    int protocol = IPPROTO_TCP;
    
    socketServer = socket(af, type, protocol);
    if(socketServer == INVALID_SOCKET) {
        wprintf(L"socket() failed! error: %d\n", WSAGetLastError());
        WSACleanup();
        getchar();
        return 1;
    }
    wprintf(L"socket created!\n");

    // bind an ip address and port number to the socket
    // basically give the socket a name to use
    int portListen = 55555;
    sockaddr_in name;
    name.sin_family = af;
    name.sin_port = htons(portListen);
    name.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    if(bind(socketServer, (SOCKADDR*) &name, sizeof(name)) == SOCKET_ERROR) {
        wprintf(L"bind() failed! error: %d\n", WSAGetLastError());
        closesocket(socketServer);
        WSACleanup();
        getchar();
        return 1;
    }
    wprintf(L"socket is bound to ip and port number\n");

    // listen/wait on the port for connections
    if(listen(socketServer, 1) == SOCKET_ERROR) {
        wprintf(L"listen() failed! error: %d\n", WSAGetLastError());
        closesocket(socketServer);
        WSACleanup();
        getchar();
        return 1;
    }
    wprintf(L"waiting for connections on port %d ...\n", portListen);

    // accept the connection if possible
    socketAccept = accept(socketServer, NULL, NULL);
    
    if(socketAccept == INVALID_SOCKET) {
        wprintf(L"accept() failed! error: %d\n", WSAGetLastError());
        closesocket(socketServer);
        WSACleanup();
        getchar();
        return 1;
    }
    wprintf(L"connection established!\n\n");

    // receive 
    char buf[100] = "";
    int byte;
    do
    {
        byte = recv(socketAccept, buf, 100, 0);
        if(byte < 0) {
            wprintf(L"recv() failed! error: %d\nconnection closed\n", WSAGetLastError());
            shutdown(socketAccept, 0);
            closesocket(socketAccept);
        }
        else if(byte == 0) {
            shutdown(socketAccept, 0);
            closesocket(socketAccept);
            wprintf(L"connection closed\n");
        }
        else {
            wprintf(L"[message received] %hs\n", buf);
        }

    } while(byte > 0);

    getchar();
    WSACleanup();
    
    return 0;
}
