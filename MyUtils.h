#pragma once
#include "GlobalParameters.h"
#include <string>
#include <list>
using namespace std;
using namespace pb666;
class MyUtils
{
public:
	static void pushStringToList(string n, list<char>& chars);
	static inline void setBit(char*, ULL, ULL);   //���ñ���λ
	static inline char getBit(char*, ULL, ULL);  //��ñ���λ
	static void sha1(char*, ULL, char*);
};

