#pragma once
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
using namespace std;

#include <libcurl/curl.h>



//#define COOKIE "p_ab_id=5; p_ab_id_2=8; _ga=GA1.2.291152945.1488708965; login_ever=yes; bookmark_tag_type=count; bookmark_tag_order=desc; bookToggle=cloud; a_type=0; is_sensei_service_user=1; module_orders_mypage=%5B%7B%22name%22%3A%22recommended_illusts%22%2C%22visible%22%3Atrue%7D%2C%7B%22name%22%3A%22everyone_new_illusts%22%2C%22visible%22%3Atrue%7D%2C%7B%22name%22%3A%22following_new_illusts%22%2C%22visible%22%3Atrue%7D%2C%7B%22name%22%3A%22mypixiv_new_illusts%22%2C%22visible%22%3Atrue%7D%2C%7B%22name%22%3A%22fanbox%22%2C%22visible%22%3Atrue%7D%2C%7B%22name%22%3A%22featured_tags%22%2C%22visible%22%3Atrue%7D%2C%7B%22name%22%3A%22contests%22%2C%22visible%22%3Atrue%7D%2C%7B%22name%22%3A%22sensei_courses%22%2C%22visible%22%3Atrue%7D%2C%7B%22name%22%3A%22spotlight%22%2C%22visible%22%3Atrue%7D%2C%7B%22name%22%3A%22booth_follow_items%22%2C%22visible%22%3Atrue%7D%5D; pixiv_embed=pix; __utmt=1; __utma=235335808.291152945.1488708965.1491560081.1491564793.8; __utmb=235335808.15.10.1491564793; __utmc=235335808; __utmz=235335808.1488708965.1.1.utmcsr=(direct)|utmccn=(direct)|utmcmd=(none); __utmv=235335808.|2=login%20ever=yes=1^3=plan=normal=1^4=p_ab_id_2=8=1^5=gender=male=1^6=user_id=17351941=1^9=p_ab_id=5=1^10=p_ab_id_2=8=1^11=illust_tag_placeholder=yes=1^12=fanbox_subscribe_button=blue=1^13=fanbox_fixed_otodoke_naiyou=yes=1^14=hide_upload_form=no=1^15=machine_translate_test=no=1"
string MODE = "d";//default

//string cookie = "Cookie:_ga=GA1.2.291152945.1488708965; bookToggle=cloud; p_ab_id=5; p_ab_id_2=8; login_ever=yes; PHPSESSID=17351941_c9adfe6e87cd403ab97ca0482733d60a; a_type=0; is_sensei_service_user=1; __utmt=1; bookmark_tag_type=count; bookmark_tag_order=desc; module_orders_mypage=%5B%7B%22name%22%3A%22recommended_illusts%22%2C%22visible%22%3Atrue%7D%2C%7B%22name%22%3A%22everyone_new_illusts%22%2C%22visible%22%3Atrue%7D%2C%7B%22name%22%3A%22following_new_illusts%22%2C%22visible%22%3Atrue%7D%2C%7B%22name%22%3A%22mypixiv_new_illusts%22%2C%22visible%22%3Atrue%7D%2C%7B%22name%22%3A%22fanbox%22%2C%22visible%22%3Atrue%7D%2C%7B%22name%22%3A%22featured_tags%22%2C%22visible%22%3Atrue%7D%2C%7B%22name%22%3A%22contests%22%2C%22visible%22%3Atrue%7D%2C%7B%22name%22%3A%22sensei_courses%22%2C%22visible%22%3Atrue%7D%2C%7B%22name%22%3A%22spotlight%22%2C%22visible%22%3Atrue%7D%2C%7B%22name%22%3A%22booth_follow_items%22%2C%22visible%22%3Atrue%7D%5D; __utma=235335808.291152945.1488708965.1491634060.1491659938.14; __utmb=235335808.11.9.1491659947315; __utmc=235335808; __utmz=235335808.1488708965.1.1.utmcsr=(direct)|utmccn=(direct)|utmcmd=(none); __utmv=235335808.|2=login%20ever=yes=1^3=plan=normal=1^4=p_ab_id_2=8=1^5=gender=male=1^6=user_id=151941=1^9=p_ab_id=5=1^10=p_ab_id_2=8=1^11=illust_tag_placeholder=yes=1^12=fanbox_subscribe_button=blue=1^13=fanbox_fixed_otodoke_naiyou=yes=1^14=hide_upload_form=no=1^15=machine_translate_test=no=1";
string cookie = "Cookie:PHPSESSID=17351941_38aa6b52ac3fc44c4a531f260ba56181; p_ab_id=7; p_ab_id_2=8; __utma=235335808.410965050.1500891062.1500891062.1500891062.1; __utmb=235335808.4.9.1500891320887; __utmc=235335808; __utmz=235335808.1500891062.1.1.utmcsr=(direct)|utmccn=(direct)|utmcmd=(none); __utmv=235335808.|2=login%20ever=yes=1^3=plan=normal=1^5=gender=male=1^6=user_id=17351941=1^9=p_ab_id=7=1^10=p_ab_id_2=8=1^11=lang=zh=1; login_bc=1; device_token=caa6f261b0f65f14bb305f4fad5ec6c3; login_ever=yes";
string keyword = "";
string keywordA = "%e9%9f%bf%20000";//KEYWORD[URLED]
unordered_map<int, int> info;
unordered_map<int, string> globalUrl;
/*
enum imageType {
	default=0,				//work  _work
	multiple=1,				//work  _work multiple
	ugoku=2,				//work  _work ugoku-illust
	manga_multiple=3,		//work  _work manga multiple
	multiple_rtl=4,			//work  _work multiple rtl
	manga=5,				//work  _work manga
	manga_multiple_rtl=6,	//work  _work manga multiple rtl
	other=7					//unknown
};
*/
//unordered_map<int, enum imageType> imageType;
unordered_map<int, int> imageType;
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
int completed = 0;
bool isAllCompleted = false;
string keywordB;
string ratioA;
HANDLE outputH;
int ThreadNum = 8;
int ThreadNumA = 0;

GumboNode * findTag(const GumboNode * root, GumboTag tag);
GumboNode * findTag(const GumboNode * root, GumboTag tag, string attr, string data);
void getImageInfo(GumboNode * root, string dubugHtml);
multimap<int, int, greater<int>> sortInfo();

void outputLog(string text, int color_index);
size_t write_callback(void * ptr, size_t size, size_t nmemb, void * stream);
void outHTMLSORTED(multimap<int, int, greater<int>> sortedInfo);
void saveFile();
void readFile();
void initGet();
void paintProgress(int total);
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