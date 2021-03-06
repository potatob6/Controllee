#include "HttpResponse.h"
#include <list>
#include "MyUtils.h"

using namespace std;
using namespace pb666;
HttpResponse HttpResponse::parseResponse(string resp)
{
	//char* buf = new char[10];
	regex re("(HTTP/1.[10]) ([0-9]{3}).*\r\n((.+)[ ]*:[ ]*(.+)\r\n)*\r\n");
	//regex re("(HTTP/1.[01]) ([0-9]{3})([ ]*.*)\n((.+)[ ]*:[ ]*(.+)\n)*");
	smatch result;
	bool match_re = regex_search(resp, result, re);
	if (match_re)
	{
		//匹配属性部分

		list<string>* lparameters = new list<string>;
		const char* delimiter = "\r\n";
		char* str = (char*)resp.c_str();
		char* token = NULL;
		char* next_token = NULL;

		token = strtok_s(str, delimiter, &next_token);

		while (token != NULL)
		{
			if (strcmp(token, "") != 0)
			{
				lparameters->push_back(token);
			}
			token = strtok_s(NULL, delimiter, &next_token);
		}

		regex attris("(.+?)[ ]*:[ ]*(.+)");     //HTTP属性匹配表达式

		auto itor = lparameters->begin();
		//匹配请求行
		HttpResponse hresp;
		{
			regex reqline_re("(HTTP/1.[01]) ([0-9]{3})[ ]*");     //无描述
			regex reqline_hre("(HTTP/1.[01]) ([0-9]{3})[ ]+(.+)");//有描述
			smatch result1;
			bool ret_1 = regex_search(*itor, result1, reqline_re);
			hresp.httpVersion = result1[1];
			hresp.returnCode = result1[2];
			if (ret_1 == true)
				hresp.returnCodeDescription = "";
			else
				hresp.returnCodeDescription = result1[3];
		}

		itor++;
		for (; itor != lparameters->end(); itor++)
		{
			smatch result_attris;
			bool ret = regex_search(*itor, result_attris, attris);

			if (ret) {
#ifdef DEBUG_MODE
				cout << u8"key:" << result_attris[1] << u8", Value:" << result_attris[2] << endl;
				//printf_s(u8"Key: %s, Value: %s\n", result_attris[1], result_attris[2]);
#endif
				hresp.attributes.push_back(pair<string, string>(result_attris[1], result_attris[2]));
			}
		}
		delete lparameters;
		return hresp;
	}
	else
	{
#ifdef DEBUG_MODE
		cout << u8"非法Http响应" << endl;
#endif
	}
	throw - 8;
	HttpResponse faild;
	return faild;
}

HttpResponse::HttpResponse(const HttpResponse& A)
{
	httpVersion = A.httpVersion;
	returnCode = A.returnCode;
	returnCodeDescription = A.returnCodeDescription;
	auto itor = A.attributes.begin();
	for (; itor != A.attributes.end(); itor++)
	{
		auto s1 = itor->first;
		auto s2 = itor->second;
		pair<string, string> p;
		p.first = s1;
		p.second = s2;
		attributes.push_back(p);
	}
}

//HttpResponse HttpResponse::operator=(const HttpResponse& A)
//{
//	httpVersion = A.httpVersion;
//	returnCode = A.returnCode;
//	returnCodeDescription = A.returnCodeDescription;
//	auto itor = A.attributes.begin();
//	for (; itor != A.attributes.end(); itor++)
//	{
//		auto s1 = itor->first;
//		auto s2 = itor->second;
//		pair<string, string> p;
//		p.first = s1;
//		p.second = s2;
//		attributes.push_back(p);
//	}
//}

HttpResponse::HttpResponse()
{

}


string HttpResponse::toString()
{
	list<char> chars;
	MyUtils::pushStringToList(httpVersion + " " + returnCode + 
		((returnCodeDescription.compare("") == 0) ? ("\r\n") : (" " + returnCodeDescription + "\r\n")), chars);
	{
		auto itor = attributes.begin();
		for (; itor != attributes.end(); itor++)
		{
			string first = itor->first;
			string second = itor->second;
			MyUtils::pushStringToList(first + ": " + second + "\r\n", chars);
		}
	}

	MyUtils::pushStringToList("\r\n", chars);
	
	char* nn = new char[chars.size() + 1];
	nn[chars.size()] = 0;
	{
		auto itor = chars.begin();
		int i = 0;
		for (; itor != chars.end(); itor++)
		{
			nn[i] = *itor;
			i++;
		}

	}
	string nn1(nn);
	delete[] nn;
	cout << nn1 << endl;
	return nn1;

}

bool HttpResponse::getPair(string key, SSP* _out)
{
	auto itor = attributes.begin();
	for (; itor != attributes.end(); itor++)
	{
		if (itor->first.compare(key) == 0) {
			_out->first = itor->first;
			_out->second = itor->second;
			return true;
		}
	}
	return false;
}