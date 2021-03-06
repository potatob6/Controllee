#pragma once
#include <string>
#include <Winsock2.h>
#include <ws2tcpip.h>
#include <map>
#include <list>
#include <regex>
#include <Windows.h>
#include "GlobalParameters.h"

class HttpRequest;
using namespace std;
using namespace pb666;
class HttpClient {
private:
	char* _recv_buf;
	unsigned long byte_pointer = 0;
	long total_recv = 0;
	unsigned long current_stream_length = 0;
	unsigned char getRnRn = 0;            // end of head bytes \r \n \r \n
	unsigned char getRn0Rn = 0;           // end of chuncked   \r \n 0 \r \n
	unsigned char enterEscapeMode = 0;
	char getNextByte();

	//read next byte and judge is this stream read end of http response
	bool judgeRn0Rn(char nb);

	//read next byte and judge is this stream read end of http head
	bool judgeRnRn(char nb);

	void ResetAllFlags();

	SOCKET server = 0;
	string domain;
	string ip;
	int iPort;
	string sPort;
	void getConnection();
	UI recvResponses = 0;
	bool autoReconnect = true;                //断开后是否自动重连（除非客户端已经关闭）
	HANDLE SendSemaphore;
	HANDLE AddMessageLock;
	list<HttpRequest*> addMessgaeBuffer;      //即将发送的消息队列
public:
	WCHAR nextResponseSaveToFile;             //下一次Response的Response-Mode不为file时保存的位置
	UI    messageNo;                          //下一次Request对应的消息编号，用于返回给WebSocket报告情况
	ULL   mmbs = 0;                           //mmbs(Memory Max Body Size)Http体接收到内存最大接收长度，若超过此长度后面的部分将会被丢弃，若为0则无限（可能会内存崩溃）
	ULL   fmbs = 0;                           //fmbs(File Max Body Size)Http体接收到文件的最大长度，同上

 	void Send();                              //发送Http报文
	//string Send(string method, string url, map<string, string> extraAttribute, string content); //发送Http报文，可以自定义属性

	void addMessage(string method, string url, string content);
	void addMessage(string method, string url, map<string, string> extraAttribute, string content);

	void ReadContentLengthToMemory(ULL, char*);//读取Content-Length长度到内存中，受控于mmbs
	string ReadNextLineToMemory();            //读取下一行
	string ReceiveHead();                     //读取响应头
	string ReadBodyToMemory();                //读取响应体到内存中
	void ReadBodyToFile(string filePath);     //读取响应体到文件中
	int StartUp(string domain, int port);     //以域名HttpClient开机
	int StartUpIP(string ip, int port);       //以IP HttpClient开机
	int Close();                              //HttpClient关机

	HttpClient();
	~HttpClient();
};