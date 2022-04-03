#pragma once
#include <string>
#include <Winsock2.h>
#include <ws2tcpip.h>
#include <map>
#include <list>
#include <regex>
#include <Windows.h>
#include "GlobalParameters.h"
using namespace std;

class HttpClient {
private:
	char* _recv_buf;
	unsigned long byte_pointer = 0;
	unsigned long total_recv = 0;
	unsigned long current_stream_length = 0;
	unsigned char getRnRn = 0;            // end of head bytes \r \n \r \n
	unsigned char getRn0Rn = 0;           // end of chuncked   \r \n 0 \r \n
	unsigned char enterEscapeMode = 0;
	char getNextByte();

	//read next byte and judge is this stream read end of http response
	bool judgeRn0Rn(char nb);

	//read next byte and judge is this stream read end of http head
	bool judgeRnRn(char nb);
	SOCKET server;
	string domain;
	string ip;
	int iPort;
	string sPort;

	UI recvPackages = 0;
public:
	ULL mbs = 0;         //mbs(Max Body Size)Http�������ճ��ȣ��������˳��Ⱥ���Ĳ��ֽ��ᱻ��������Ϊ0�����ޣ����ܻ��ڴ������

 	string Send(string method, string url, string content);       //����Http���ģ�ֻ����Ĭ������
	string Send(string method, string url, map<string, string> extraAttribute, string content); //����Http���ģ������Զ�������

	void ReadContentLengthToMemory(ULL, char*);//��ȡContent-Length���ȵ��ڴ��У��ܿ���mbs
	string ReadNextLineToMemory();     //��ȡ��һ��
	string ReceiveHead();              //��ȡ��Ӧͷ
	string ReadBodyToMemory();         //��ȡ��Ӧ�嵽�ڴ���
	void ReadBodyToFile(string filePath); //��ȡ��Ӧ�嵽�ļ���
	int StartUp(string domain, int port);                    //������HttpClient����
	int StartUpIP(string ip, int port);                      //��IP HttpClient����
	int Close();                                   //HttpClient�ػ�

	HttpClient();
	~HttpClient();
};