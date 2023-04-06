
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

int main() {
    WORD wVersionRequired = MAKEWORD(2, 2);
    WSADATA wsaData;
    if(WSAStartup(wVersionRequired, &wsaData) != 0) {
        wprintf(L"failed to load winsock\n");
        return 1;
    }
    wprintf(L"\nloaded winsock2, version: %d.%d\n", LOBYTE(wsaData.wVersion), HIBYTE(wsaData.wVersion));

    // create a unconnected socket
    SOCKET socketClient = INVALID_SOCKET;
    int af = AF_INET;
    int type = SOCK_STREAM;
    int proto = IPPROTO_TCP;

    socketClient = socket(af, type, proto);

    if(socketClient == INVALID_SOCKET) {
        wprintf(L"socket() failed! error: %d\n", WSAGetLastError());
        WSACleanup();
        getchar();
        return 1;
    }
    wprintf(L"socket created\n");

    // connect to the server socket whick is listening on a specific port

    sockaddr_in sName;  // name of the server 
    sName.sin_family = af;
    sName.sin_port = htons(/*the port number on which the server socket listening*/);
    sName.sin_addr.s_addr = inet_addr("/*ip address of server*/");
    
    if(connect(socketClient, (SOCKADDR*) & sName, sizeof(sName)) == SOCKET_ERROR) {
        wprintf(L"connect() failed! error: %d\n", WSAGetLastError());
        WSACleanup();
        getchar();
        return 1;
    }
    wprintf(L"connected with server\n\n");

    // send to server
    char buf[100];
    int byte;
    do {
        wprintf(L"send message: ");
        std::cin.getline(buf, 100);
        if(strcmp(buf, "SHUTDOWN") == 0) {
            shutdown(socketClient, 1);
            closesocket(socketClient);
            wprintf(L"[connection closed]\n");
            byte = 0;
            continue;
        }
        byte = send(socketClient, buf, 100, 0);
        if(byte == SOCKET_ERROR) {
            wprintf(L"send() failed! error: %d\n", WSAGetLastError());
            shutdown(socketClient, 1);
            closesocket(socketClient);
        }
        else {
            wprintf(L"[message sent]\n\n");
        }
    }
    while(byte > 0);
    
    getchar();
    WSACleanup();

    return 0;
    
}
