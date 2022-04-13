#include "HttpResponseHandler.h"
#include "HttpClient.h"
#include "HttpResponse.h"

HttpResponseHandler::HttpResponseHandler(HttpClient* client, HttpResponse* response)
{
	this->client = client;
	this->response = response;
}

//�ܽ���˵�һ�㶼���ļ����ͣ�������ʱ�Ȳ�����������
errno_t HttpResponseHandler::Handle()
{
	SSP clh, rspm, connm;
	SSP trf;
	bool finded = response->getPair("Content-Length", &clh);
	bool findedTranser = response->getPair("Transfer-Encoding", &trf);
	//bool cmdm = response->getPair("Response-Mode", &rspm);
	//bool conn = response->getPair("Connection", &connm);
	if (finded)
	{
		//��Content-Length
		//����������ȼ��ص��ڴ�
		ULL l = atoi(clh.second.c_str());
		char* cs = new char[l + 1];
		cs[l] = 0;
		try {
			client->ReadContentLengthToMemory(l, cs);

		}
		catch (int e) {
			delete[] cs;
			//����ʧ��
			//����ǰMessageNo�Լ�������ظ�WebSocket
			//TODO
			return e;
		}

		delete[] cs;
		return 0;
	}
	else if(findedTranser)
	{

		return 0;
	}
	return 1;
}