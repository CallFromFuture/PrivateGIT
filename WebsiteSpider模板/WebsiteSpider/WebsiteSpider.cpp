#include "WebsiteSpider.h"

size_t write_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
	string * pStream = static_cast<string *>(stream);
	(*pStream).append((char *)ptr, size * nmemb);
	return size * nmemb;
}

int main() {
	HWND hwnd = GetConsoleWindow();
	HDC hdc = GetDC(hwnd);
	char p[7] = "00.00%";
	string PROXY = ""; //don't use it if not necessary
	unsigned int TIMEOUT = 30000; // ms 
	string COOKIE = "";
	int threadNum = 4;
	outputH = GetStdHandle(STD_OUTPUT_HANDLE);

	string dir1 = "log/", dir2 = "out/";
	struct _stat fileStat;
	if (!((_stat(dir1.c_str(), &fileStat) == 0) && (fileStat.st_mode & _S_IFDIR)))
		CreateDirectory(dir1.c_str(), NULL);
	if (!((_stat(dir2.c_str(), &fileStat) == 0) && (fileStat.st_mode & _S_IFDIR)))
		CreateDirectory(dir2.c_str(), NULL);

	curl_global_init(CURL_GLOBAL_ALL);

	cout << "Welcome to use WebsiteSpider 0.1 By Callfromfuture" << endl;
	cout << "Date: August 5, 2017" << endl;
	//data input

	NoCompleted = threadNum;
	//fill mainUrlVec
	for (int i = 0; i <= 1000; i++) {
		//TODO
	}
	//mainLoop
	for (int i = 0; i < threadNum; i++) {
		thread T(ThreadLoop, PROXY, TIMEOUT, COOKIE);
		T.detach();
	}
	while (NoCompleted) {
		if(mainUrlVec.size() != 0)
			sprintf(p, "%2.2f%%", ((float)mainOrderNum / (float)mainUrlVec.size()) * 100);
		else sprintf(p, "Error!\0");
		TextOut(hdc, 0, 0, p, 6);
		Sleep(2);//TODO: try a lot to modify it
	}
	//data translation

	system("PAUSE");
	return 0;
}

void ThreadLoop(const string sProxy, const unsigned int uiTimeout, const string Cookie) {
	int orderNum = 0;
	while (1) {
		orderLocker.lock();
		if (mainOrderNum < mainUrlVec.size()) {
			orderNum = mainOrderNum;
			mainOrderNum++;
			orderLocker.unlock();
		}
		else {
			orderLocker.unlock();
			break;
		}

		SetConsoleTextAttribute(outputH, FOREGROUND_RED | FOREGROUND_GREEN);
		printf("[INFO]%d:started\n", orderNum);
		SetConsoleTextAttribute(outputH, 7);
		switch (parseHtml(curlGetData(mainUrlVec[orderNum], sProxy, uiTimeout, Cookie))) {
		case 0:
			SetConsoleTextAttribute(outputH, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			printf("[WELL]%d: Completed\n", orderNum);
			SetConsoleTextAttribute(outputH, 7);
			break;
		case -1:
			SetConsoleTextAttribute(outputH, FOREGROUND_RED | FOREGROUND_INTENSITY);
			printf("[WARN]%d: Root Node Error\n", orderNum);
			SetConsoleTextAttribute(outputH, 7);
			break;
		default:
			SetConsoleTextAttribute(outputH, FOREGROUND_RED | FOREGROUND_INTENSITY);
			printf("[WARN]%d: Unknown Error\n", orderNum);
			SetConsoleTextAttribute(outputH, 7);
			break;
		}
	}
	runLocker.lock();
	NoCompleted--;
	runLocker.unlock();
}

/*
Data Scanner
author:Callfromfuture
status:TESTING
Security level:Unsafe 1
*/
string curlGetData(const string sUrl, const string sProxy, const unsigned int uiTimeout, const string Cookie) {
	CURL *curl;
	CURLcode res;
	string rts = "";
	string sRsp;
	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, sUrl.c_str());
		if (uiTimeout > 0)
			curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, uiTimeout);
		if (!sProxy.empty())
			curl_easy_setopt(curl, CURLOPT_PROXY, sProxy.c_str());

		struct curl_slist *headers = NULL;
		headers = curl_slist_append(headers, Cookie.c_str());
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &sRsp);

		res = curl_easy_perform(curl);
		curl_slist_free_all(headers);
		if (res) {
			SetConsoleTextAttribute(outputH, FOREGROUND_RED | FOREGROUND_INTENSITY);
			cerr << "[WARN]url_easy_perform() failed: " << curl_easy_strerror(res) << endl;
			SetConsoleTextAttribute(outputH, 7);
			mainUrlVec.push_back(sUrl);//TODO: it could be replaced by 'mainUrlVec' *But it may cause endless loop
		}
		else rts = UTF8ToGBK(sRsp);
	}
	curl_easy_cleanup(curl);
	return rts;
}

/*
parse Html datas
author:Callfromfuture
status:TESTING
Security level:Dangerous 0
*/
int parseHtml(const string html) {
	GumboOutput *output = gumbo_parse(html.c_str());
	GumboNode *temp;
	temp = findTag(output->root, GUMBO_TAG_BODY);
	if (temp == nullptr) return -1;
	//TODO
	return 0;
}

/*
find tag from html
author:Callfromfuture
status:TESTING
Security level:Dangerous 0
*/
GumboNode* findTag(const GumboNode *root, GumboTag tag) {
	const GumboVector *root_children = &root->v.element.children;
	for (int i = 0; i < root_children->length; ++i) {
		GumboNode *child = (GumboNode*)(root_children->data[i]);
		if (child->type == GUMBO_NODE_ELEMENT &&
			child->v.element.tag == tag) {
			return child;
		}
	}
	return nullptr;
}

/*
find tag and the attribute from html
author:Callfromfuture
status:TESTING
Security level:Dangerous 0
*/
GumboNode* findTag(const GumboNode *root, GumboTag tag, string attr, string data) {
	const GumboVector *root_children = &root->v.element.children;
	GumboAttribute *attri;
	for (int i = 0; i < root_children->length; ++i) {
		GumboNode *child = (GumboNode*)(root_children->data[i]);
		if (child->type == GUMBO_NODE_ELEMENT &&
			child->v.element.tag == tag &&
			(attri = gumbo_get_attribute(&child->v.element.attributes, attr.c_str()))) {
			if (attri->value == data)
				return child;
		}
	}
	return nullptr;
}

/*
output sortedDatas' .html file
author:Callfromfuture
status:TESTING
Security level:Unsafe 1
*/
void outputData(map<int, int> sortedData) {
	string fileName = "";//TODO
	ofstream out("out/" + fileName);
	out << "<base target='_blank'><table border='1'>" << endl;
	int i = 0;
	stringstream buffer1, buffer2;
	for (auto beg = sortedData.begin(), end = sortedData.end(); beg != end; beg++) {
		//TODO:fill buffer

		if (i != 1) {//TODO
			i++;
		}
		else {
			out << "<tr>" << buffer1.str() << "</tr>" << endl;
			out << "<tr>" << buffer2.str() << "</tr>" << endl;
			buffer1.str("");
			buffer2.str("");
			i = 0;
		}
	}
	if (i != 0) {
		out << "<tr>" << buffer1.str() << "</tr>" << endl;
		out << "<tr>" << buffer2.str() << "</tr>" << endl;
	}

	out << "</table>" << endl;
	out.close();
}

/*
GBKtoUTF8
author:Internet
status:Using
Security level:Safe 5
*/
string GBKToUTF8(const string& strGBK) {
	string strOutUTF8 = "";
	WCHAR * str1;
	int n = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, NULL, 0);
	str1 = new WCHAR[n];
	MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, str1, n);
	n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);
	char * str2 = new char[n];
	WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, n, NULL, NULL);
	strOutUTF8 = str2;
	delete[]str1;
	str1 = NULL;
	delete[]str2;
	str2 = NULL;
	return strOutUTF8;
}

/*
UTF8toGBK
author:Internet
status:Using
Security level:Safe 5
*/
string UTF8ToGBK(const string& strUTF8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);
	unsigned short * wszGBK = new unsigned short[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)strUTF8.c_str(), -1, (LPWSTR)wszGBK, len);

	len = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
	char *szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, (LPSTR)szGBK, len, NULL, NULL);
	//strUTF8 = szGBK;
	string strTemp(szGBK);
	try {
		delete[]szGBK;
		delete[]wszGBK;
	}
	catch (system_error) {
		return strTemp;
	}
	return strTemp;
}

char dec2hexChar(short int n) {
	if (0 <= n && n <= 9) {
		return char(short('0') + n);
	}
	else if (10 <= n && n <= 15) {
		return char(short('A') + n - 10);
	}
	else {
		return char(0);
	}
}

short int hexChar2dec(char c) {
	if ('0' <= c && c <= '9') {
		return short(c - '0');
	}
	else if ('a' <= c && c <= 'f') {
		return (short(c - 'a') + 10);
	}
	else if ('A' <= c && c <= 'F') {
		return (short(c - 'A') + 10);
	}
	else {
		return -1;
	}
}

/*
escape Url
author:Internet
status:Using
Security level:Safe 5
*/
string escapeURL(const string &URL)
{
	string result = "";
	for (unsigned int i = 0; i < URL.size(); i++) {
		char c = URL[i];
		if (
			('0' <= c && c <= '9') ||
			('a' <= c && c <= 'z') ||
			('A' <= c && c <= 'Z') ||
			c == '/' || c == '.'
			) {
			result += c;
		}
		else {
			int j = (short int)c;
			if (j < 0) {
				j += 256;
			}
			int i1, i0;
			i1 = j / 16;
			i0 = j - i1 * 16;
			result += '%';
			result += dec2hexChar(i1);
			result += dec2hexChar(i0);
		}
	}
	return result;
}

/*
deescape URL
author:Internet
status:Using
Security level:Safe 5
*/
string deescapeURL(const string &URL) {
	string result = "";
	for (unsigned int i = 0; i < URL.size(); i++) {
		char c = URL[i];
		if (c != '%') {
			result += c;
		}
		else {
			char c1 = URL[++i];
			char c0 = URL[++i];
			int num = 0;
			num += hexChar2dec(c1) * 16 + hexChar2dec(c0);
			result += char(num);
		}
	}
	return result;
}