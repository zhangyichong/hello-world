// test123333.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<Ws2tcpip.h>
#include <WinSock2.h>
#include <windows.h>
#pragma comment(lib,"WS2_32")

SOCKET S;
SOCKADDR_IN ServerAddr;
int Port = 5150;
int IsConnectedFlag = 0;
DWORD WINAPI ThreadOne(LPVOID lpParameter);
int _tmain(int argc, _TCHAR* argv[])
{
	WSADATA wsaData;
	char buf[100] = {0};
	int len = 100;
	int flag;
	HANDLE HOne;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		return 0;
	}

	S = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(Port);
	inet_pton(AF_INET, "192.168.1.70", (void*)&ServerAddr.sin_addr.S_un.S_addr);
	int ret = connect(S, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr));
	if (ret != 0)
	{  
		printf("Connect error\n");   
		if (S != INVALID_SOCKET)
		{   
			IsConnectedFlag = 0;
			closesocket(S); // 关掉 socket  
			return 0 ;
		}
	}
	printf("Connect success\n"); 
	IsConnectedFlag = 1;
	HOne=CreateThread(NULL,0,ThreadOne,NULL,0,NULL);
	while(1)
	{
		flag = recv(S,buf,len,0);
		if (flag<=0)
		{
			if (IsConnectedFlag == 0)
			{
				continue;
			}
			printf("断开链接\n");
			IsConnectedFlag = 0;
			closesocket(S);
			continue;
		}
		printf("%s\n",buf);
	}
	return 0;
}
DWORD WINAPI ThreadOne(LPVOID lpParameter)
{
	while(1)
	{
		Sleep(10000);
		if(IsConnectedFlag == 0)
		{
			S = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			ServerAddr.sin_family = AF_INET;
			ServerAddr.sin_port = htons(Port);
			inet_pton(AF_INET, "192.168.1.70", (void*)&ServerAddr.sin_addr.S_un.S_addr);
			int ret = connect(S, (SOCKADDR*)&ServerAddr, sizeof(ServerAddr));

			if (ret != 0)
			{  
				printf("客户端请求链接\n"); 
				if (S != INVALID_SOCKET)
				{   
					IsConnectedFlag = 0;
					closesocket(S); // 关掉 socket  
					continue;
				}
			}
			IsConnectedFlag = 1;
			printf("Connect success\n"); 

		}
	}
}
