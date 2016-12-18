#pragma once
//#pragma comment(lib,"libcurl/libcurl.lib")
#pragma comment ( lib, "ws2_32.lib" )
#pragma comment ( lib, "wldap32.lib" )
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <map>
#include <cmath>
#include <unordered_map>
#include <windows.h>
#include <gumbo/gumbo.h>
//#include <unordered_set>
using namespace std;

#include <libcurl/curl.h>

#define BIGURL "http://i3.pixiv.net/img-original/img/"
#define REPLACE_SMALLURL1 "http://i3.pixiv.net/c/150x150/img-master/img/"
#define REPLACE_SMALLURL2 "_master1200"
#define COOKIE "p_ab_id=0; login_ever=yes; manga_viewer_expanded=1; bookmark_tag_type=count; bookmark_tag_order=desc; a_type=0; is_sensei_service_user=1; b_type=1; __utmt=1; device_token=c274a993f4a46162dcafbb0afbcaa19d; module_orders_mypage=%5B%7B%22name%22%3A%22recommended_illusts%22%2C%22visible%22%3Atrue%7D%2C%7B%22name%22%3A%22spotlight%22%2C%22visible%22%3Atrue%7D%2C%7B%22name%22%3A%22everyone_new_illusts%22%2C%22visible%22%3Atrue%7D%2C%7B%22name%22%3A%22fanbox%22%2C%22visible%22%3Atrue%7D%2C%7B%22name%22%3A%22sensei_courses%22%2C%22visible%22%3Atrue%7D%2C%7B%22name%22%3A%22featured_tags%22%2C%22visible%22%3Atrue%7D%2C%7B%22name%22%3A%22contests%22%2C%22visible%22%3Atrue%7D%2C%7B%22name%22%3A%22following_new_illusts%22%2C%22visible%22%3Atrue%7D%2C%7B%22name%22%3A%22mypixiv_new_illusts%22%2C%22visible%22%3Atrue%7D%2C%7B%22name%22%3A%22booth_follow_items%22%2C%22visible%22%3Atrue%7D%5D; __utma=235335808.252990467.1480775136.1481887370.1481891032.12; __utmb=235335808.7.9.1481892146088; __utmc=235335808; __utmz=235335808.1481537845.4.2.utmcsr=baidu|utmccn=(organic)|utmcmd=organic; __utmv=235335808.|2=login%20ever=yes=1^3=plan=normal=1^5=gender=male=1^6=user_id=17351941=1"
#define MULTI_CURL_NUM 6
#define MODE "NSORT"

string keyword = "";
string keywordA = "%e5%a5%b3%e3%81%ae%e5%ad%90%20000";
unordered_map<int, int> info;
unordered_map<int, string> globalUrl;
mutex mut;
std::string     PROXY = ""; //don't use it if not necessary
unsigned int    TIMEOUT = 30000; // ms 
bool firstRun = true;
unsigned int numofImage;
vector<string> errorUrlVec(0);
unordered_map<int, string> errorUrlMap(0);
int running = 0;
mutex mut2;
int strangeError = 0;
mutex mut3;
int Nocompleted = 0;

GumboNode * findTag(const GumboNode * root, GumboTag tag);
GumboNode * findTag(const GumboNode * root, GumboTag tag, string attr, string data);
void getImageInfo(GumboNode * root, string dubugHtml);
multimap<int, int, greater<int>> sortInfo();

size_t write_callback(void * ptr, size_t size, size_t nmemb, void * stream);
void outHTMLSORTED(multimap<int, int, greater<int>> sortedInfo);
void saveFile();
void readFile();
void initGet();
int curlMultithreading(string keyword, int num);
bool curlEasyThread(const string sUrl, const string sProxy, unsigned int uiTimeout, int orderNum);
int parseHtml(string html);
void downloadImage(string url, int id, int num);
std::string GBKToUTF8(const std::string & strGBK);
std::string UTF8ToGBK(const std::string & strUTF8);

char dec2hexChar(short int n);
short int hexChar2dec(char c);
string escapeURL(const string & URL);
string deescapeURL(const string & URL);