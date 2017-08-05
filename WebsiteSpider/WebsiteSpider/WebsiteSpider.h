#pragma once
#pragma comment ( lib, "ws2_32.lib" )
#pragma comment ( lib, "wldap32.lib" )

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <mutex>
#include <thread>
#include <vector>
#include <map>
using namespace std;

#include <gumbo/gumbo.h>
#include <libcurl/curl.h>





HANDLE outputH;
vector<string> errorUrlVec;
vector<string> mainUrlVec;





int mainOrderNum = 0;
mutex orderLocker;
int NoCompleted = 0;
mutex runLocker;


void ThreadLoop(const string sProxy, const unsigned int uiTimeout, const string Cookie);
string curlGetData(const string sUrl, const string sProxy, const unsigned int uiTimeout, const string Cookie);

int parseHtml(const string html);
GumboNode * findTag(const GumboNode * root, GumboTag tag);
GumboNode * findTag(const GumboNode * root, GumboTag tag, string attr, string data);

void outputData(map<int, int> sortedData);

string GBKToUTF8(const std::string & strGBK);
string UTF8ToGBK(const std::string & strUTF8);

char dec2hexChar(short int n);
short int hexChar2dec(char c);
string escapeURL(const string & URL);
string deescapeURL(const string & URL);