#include "PixivScanner.h"

int n = 0;

void outputLog(string text, int color_index) {
	SetConsoleTextAttribute(outputH, color_index);
	printf(text.c_str());
	SetConsoleTextAttribute(outputH, 7);
	return;
}

size_t write_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
	std::string * pStream = static_cast<std::string *>(stream);
	(*pStream).append((char *)ptr, size * nmemb);
	return size * nmemb;
}

int main() {
	//init
	string dir1 = "temp/",dir2 = "out/";
	struct _stat fileStat;
	if (!((_stat(dir1.c_str(), &fileStat) == 0) && (fileStat.st_mode & _S_IFDIR)))
		CreateDirectory(dir1.c_str(), NULL);
	if (!((_stat(dir2.c_str(), &fileStat) == 0) && (fileStat.st_mode & _S_IFDIR)))
		CreateDirectory(dir2.c_str(), NULL);
	outputH = GetStdHandle(STD_OUTPUT_HANDLE);
	curl_global_init(CURL_GLOBAL_ALL);
	//main loop
	
	//use Chinese or Japanese
	cout << "Welcome to use PixivScanner 3.1.1 By Callfromfuture" << endl;
	cout << "Date: July 24, 2017" << endl;
	cout << "HTTPS Supported" << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	cout << "[Necessary]Keyword:";
	getline(cin, keywordB);
	cout << "[Optional]Cookie:";
	string cookieA = "";
	getline(cin, cookieA);
	if (cookieA != "") cookie = cookieA;
	cout << "[Optional]Ratio:";
	getline(cin, ratioA);
	cout << "[Necessary]Mode(s-sortOnly,d-default,t-AutoTransfer):";
	cin >> MODE;
	if (MODE == "t")
		keywordA = escapeURL(GBKToUTF8(keywordB));
	else { 
		keywordA = keywordB;
		keywordB = UTF8ToGBK(deescapeURL(GBKToUTF8(keywordB)));
	}
	readFile();
	if (MODE != "s") {
		initGet();
		if(errorUrlVec.size()>0){
		int n = 0;
		for (auto beg = errorUrlVec.begin(), end = errorUrlVec.end(); beg != end; beg++) {
			n++;
			try {
				string a = *beg;
				string::size_type idx = a.find("https://www.pixiv.net/search.php");
				if (idx == string::npos)
					continue;
				thread T(curlEasyThread, a, PROXY, 100000, 10000 + n);
				T.detach();
			} catch(bad_alloc){
				SetConsoleTextAttribute(outputH, FOREGROUND_RED | FOREGROUND_INTENSITY);
				cerr << "[WARN]errorUrl[" << n << "]:BAD_ALLOC\n";
				SetConsoleTextAttribute(outputH, 7);
			}

		}
		
		
		SetConsoleTextAttribute(outputH, FOREGROUND_RED | FOREGROUND_GREEN);
		printf("[INFO]please wait until %d", 10000 + n);
		SetConsoleTextAttribute(outputH, 7);
		}
		system("pause");
		for (auto beg = errorUrlMap.begin(), end = errorUrlMap.end(); beg != end; beg++) {
			downloadImage(beg->second, beg->first, 0);
		}
		saveFile();
	}

	multimap<int, int, greater<int>> sortedInfo = sortInfo();
	outHTMLSORTED(sortedInfo);
	//close all
	curl_global_cleanup();
	system("PAUSE");
	return 0;
}

/*
output sortedDatas' .html file
author:Callfromfuture
status:TESTING
Security level:Unsafe 1
*/
void outHTMLSORTED(multimap<int, int, greater<int>> sortedInfo) {

	ofstream out("out/" + keywordB + "."+ratioA+".html");
	out << "<base target='_blank'><table border='1'>" << endl;
	int i = 0;
	stringstream buffer1, buffer2;
	for (auto beg = sortedInfo.begin(), end = sortedInfo.end(); beg != end; beg++) {
		string or = globalUrl[beg->second];
		string href = or.replace(or .size() - 15, 15, "");
		switch (imageType[beg->second]) {
		case 1:
		case 4:
			buffer1 << "<th>" << beg->first << "[M]<a href='https://i.pximg.net/img-original/img/" << href << ".jpg" << "' download>J</a>/" << "<a href='https://i.pximg.net/img-original/img/" << href << ".png" << "' download>P</a>" << "</th>" << endl;
			break;
		case 3:
		case 5:
		case 6:
			buffer1 << "<th>" << beg->first << "[G]<a href='https://i.pximg.net/img-original/img/" << href << ".jpg" << "' download>J</a>/" << "<a href='https://i.pximg.net/img-original/img/" << href << ".png" << "' download>P</a>" << "</th>" << endl;
			break;
		case 2:
			buffer1 << "<th>" << beg->first << "[U]<a href='https://i1.pixiv.net/img-zip-ugoira/img/" << href << "_ugoira1920x1080.zip" << "' download>ZIP</a>" << "</th>" << endl;
			break;
		case 0:
		default:
			buffer1 << "<th>" << beg->first << "[C]<a href='https://i.pximg.net/img-original/img/" << href << ".jpg" << "' download>J</a>/" << "<a href='https://i.pximg.net/img-original/img/" << href << ".png" << "' download>P</a>" << "</th>" << endl;
			break;
		}
		
		//buffer1 << "<th>" << beg->first << "<a href='https://i.pximg.net/img-original/img/"<<href<<".jpg"<<"' download>JPG</a>/"<< "<a href='https://i.pximg.net/img-original/img/" << href << ".png" << "' download>PNG</a>" <<"</th>" << endl;
		//buffer2 << "<td><a href='http://i3.pixiv.net/c/1200x1200/img-master/img/" << globalUrl[beg->second] << "' download='" << beg->second << ".jpg'><img src='temp/" << beg->second << ".jpg'></img></a>" << endl;
		
		buffer2 << "<td><a href='https://www.pixiv.net/member_illust.php?mode=medium&illust_id="<< beg->second <<"'><img src='../temp/" << beg->second << ".jpg'></img></a>" << endl;
		if (i != 11) {
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
save Datas' .out file
author:Callfromfuture
status:TESTING
Security level:Unsafe 1
*/
void saveFile() {
	stringstream buffer1;
	for (auto beg = info.begin(), end = info.end(); beg != end; beg++)
		buffer1 << beg->first << " " << beg->second << " " << globalUrl[beg->first] << " " << imageType[beg->first] << endl;
	ofstream out("out/" + keywordB + "." + ratioA + ".out", ios::out);
	out << buffer1.str();
	out.close();
}

/*
read Datas' .out file
author:Callfromfuture
status:TESTING
Security level:Unsafe 1
*/
void readFile() {
	ifstream in("out/" + keywordB + "." + ratioA + ".out");
	std::string str((std::istreambuf_iterator<char>(in)),
		std::istreambuf_iterator<char>());
	stringstream input(str);
	int key;
	int value1;
	string value2;
	int value3;
	//enum imageType value3;
	while (input >> key >> value1 >> value2 >> value3) {
		if (value2 == "")
			cout << "read failed" << endl;
		info[key] = value1;
		globalUrl[key] = value2;
		imageType[key] = value3;
	}
	in.close();
}

/*
get Datas from pixiv
author:Callfromfuture
status:TESTING
Security level:Dangerous 0
*/
void initGet() {
	LARGE_INTEGER  large_interger;
	//double dff;
	LONGLONG dff2;
	__int64  c1, c2;
	QueryPerformanceFrequency(&large_interger);
	dff2 = large_interger.QuadPart;
	QueryPerformanceCounter(&large_interger);
	c1 = large_interger.QuadPart;

	stringstream out;
	out << "https://www.pixiv.net/search.php?word=" << keywordA << "&p=" << 1 << "&ratio=" << ratioA;
	curlEasyThread(out.str(), PROXY, TIMEOUT, 0);
	int pagenum = ceil(float(numofImage) / 40.0f);

	if (pagenum != 0 && pagenum <= 1000){
		thread T(paintProgress, pagenum);
		T.detach();
		curlMultithreading(keywordA, pagenum);
	}
	else if (pagenum > 1000){
		thread T(paintProgress, 1000);
		T.detach();
		curlMultithreading(keywordA, 1000);
	}
	else curlMultithreading(keywordA, 0);
	
	QueryPerformanceCounter(&large_interger);
	c2 = large_interger.QuadPart;
	SetConsoleTextAttribute(outputH, FOREGROUND_RED | FOREGROUND_GREEN);
	cout << "[INFO]time(ms) : " << (c2 - c1) * 1000 / dff2 << endl;
	SetConsoleTextAttribute(outputH, 7);
	//system("pause");

	//	curlMultithreading(keywordA, 3);



}

void paintProgress(int total) {
	HWND hwnd;
	HDC hdc;
	float i=0;
	hwnd = GetConsoleWindow();
	hdc = GetDC(hwnd);
	char p[7] = "00.00%";
	while(!isAllCompleted){
		i = (float)completed / (float)total;
		sprintf(p, "%2.2f%%", i*100);
		TextOut(hdc, 0, 0, p, 6);
		Sleep(2);
	}
}

/*
create thread to get imageInfo
author:Callfromfuture
status:TESTING
Security level:Unsafe 1
*/
int curlMultithreading(string keyword, int num) {
	int rn = num, aa = 1;
	if (num == 0) {
		cout << "Nothing" << endl;
		return -1;
	}
	while (aa < num) {
		
		if (Nocompleted <= ThreadNum) {//todo
			stringstream out;
			out << "https://www.pixiv.net/search.php?word=" << keyword << "&p=" << aa + 1 << "&ratio=" << ratioA;
			//thread T(curlEasyThread, out.str(), PROXY, RspVec[idx], TIMEOUT);
			mut3.lock();
			Nocompleted++;
			mut3.unlock();
			thread T(curlEasyThread, out.str(), PROXY, TIMEOUT, aa);
			T.detach();
			aa++;
		}
		else Sleep(1000);
	}
	while (Nocompleted > 0) {
		SetConsoleTextAttribute(outputH, FOREGROUND_RED | FOREGROUND_GREEN);
		printf("[INFO]running: %d\n", Nocompleted);
		SetConsoleTextAttribute(outputH, 7);
		Sleep(1000);
	}
	isAllCompleted = true;
	return 0;
}

/*
Data Scanner
author:Callfromfuture
status:TESTING
Security level:Unsafe 1
*/
bool curlEasyThread(const string sUrl, const string sProxy, unsigned int uiTimeout, int orderNum) {
	SetConsoleTextAttribute(outputH, FOREGROUND_RED | FOREGROUND_GREEN);
	printf("[INFO]%d:started\n", orderNum);
	SetConsoleTextAttribute(outputH, 7);
	CURL *curl;
	CURLcode res;
	BOOL rtc = false;
	string sRsp;
	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, sUrl.c_str());
		if (uiTimeout > 0)
			curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, uiTimeout);
		if (!sProxy.empty())
			curl_easy_setopt(curl, CURLOPT_PROXY, sProxy.c_str());
		struct curl_slist *headers = NULL;
		headers = curl_slist_append(headers, cookie.c_str());
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &sRsp);
		
		res = curl_easy_perform(curl);
		curl_slist_free_all(headers);
		if (res) {
			SetConsoleTextAttribute(outputH, FOREGROUND_RED|FOREGROUND_INTENSITY);
			cerr << "[WARN]url_easy_perform() failed: " << curl_easy_strerror(res) << endl;
			SetConsoleTextAttribute(outputH, 7);
			errorUrlVec.push_back(sUrl);
			rtc = false;
		}
		else {

			int code = parseHtml(UTF8ToGBK(sRsp));

			if (code == 0) {
				SetConsoleTextAttribute(outputH, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
				printf("[WELL]%d:completed\n", orderNum);
				SetConsoleTextAttribute(outputH, 7);
				
				rtc = true;
			}
			else if (code == -1) {
				SetConsoleTextAttribute(outputH, FOREGROUND_RED | FOREGROUND_INTENSITY);
				printf("[WARN]%d: Root Node Error\n", orderNum);
				SetConsoleTextAttribute(outputH, 7);
				rtc = false;
			}
			else {
				SetConsoleTextAttribute(outputH, FOREGROUND_RED | FOREGROUND_INTENSITY);
				printf("[WARN]%d: Get Num Error\n", orderNum);
				SetConsoleTextAttribute(outputH, 7);
				rtc = false;
			}

		}
	}
	mut3.lock();
	Nocompleted--;
	completed++;
	mut3.unlock();
	curl_easy_cleanup(curl);

	return rtc;
}


/*
parse Html datas
author:Callfromfuture
status:TESTING
Security level:Dangerous 0
*/
int parseHtml(string html) {
	GumboOutput *output = gumbo_parse(html.c_str());
	GumboNode *temp;
	temp = findTag(output->root, GUMBO_TAG_BODY);
	if (temp == nullptr) return -1;
	temp = findTag(temp, GUMBO_TAG_DIV, "id", "wrapper");
	if (temp == nullptr) return -1;
	temp = findTag(temp, GUMBO_TAG_DIV, "class", "layout-body");
	if (temp == nullptr) return -1;
	temp = findTag(temp, GUMBO_TAG_DIV, "class", "_unit");
	if (temp == nullptr) return -1;

	if (firstRun) {
		GumboNode *node2 = findTag(temp, GUMBO_TAG_DIV, "class", "column-header");
		if (node2 == nullptr) return -2;
		node2 = findTag(node2, GUMBO_TAG_SPAN, "class", "count-badge");
		if (node2 == nullptr) return -2;
		string a = ((GumboNode*)node2->v.element.children.data[0])->v.text.text;
		numofImage = stoi(a.replace(a.size() - 1, 1, ""));
		firstRun = false;
	}
	temp = findTag(temp, GUMBO_TAG_SECTION, "class", "column-search-result");
	if (temp == nullptr) return -1;
	temp = findTag(temp, GUMBO_TAG_UL, "class", "_image-items autopagerize_page_element");
	if (temp == nullptr) return -1;
	getImageInfo(temp, html);

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
get imageInfo
author:Callfromfuture
status:TESTING
Security level:Dangerous 0
*/
void getImageInfo(GumboNode *root, string dubugHtml) {
	int typeA = 7;
	GumboVector items = root->v.element.children;
	for (int i = 0; i < items.length; i++) {
		GumboNode *item = (GumboNode*)(items.data[i]);
		
		if (item == nullptr)
			return;
		GumboNode *node1 = findTag(item, GUMBO_TAG_A, "class", "work  _work ");	//root's child
		typeA = 0;
		if (node1 == nullptr) {
			node1 = findTag(item, GUMBO_TAG_A, "class", "work  _work multiple ");	//root's child	
			typeA = 1;
		}
		if (node1 == nullptr) {
			node1 = findTag(item, GUMBO_TAG_A, "class", "work  _work ugoku-illust ");	//root's child
			typeA = 2;
		}
		if (node1 == nullptr){
			node1 = findTag(item, GUMBO_TAG_A, "class", "work  _work manga multiple ");	//root's child	
			typeA = 3;
		}
		if (node1 == nullptr){
			node1 = findTag(item, GUMBO_TAG_A, "class", "work  _work multiple rtl ");	//root's child	
			typeA = 4;
		}
		if (node1 == nullptr){
			node1 = findTag(item, GUMBO_TAG_A, "class", "work  _work manga ");	//root's child	
			typeA = 5;
		}
		if (node1 == nullptr){
			node1 = findTag(item, GUMBO_TAG_A, "class", "work  _work manga multiple rtl ");	//root's child	
			typeA = 6;
		}
		if (node1 == nullptr) {
			SetConsoleTextAttribute(outputH, FOREGROUND_RED | FOREGROUND_GREEN);
			printf("[INFO]Get Info Error\n");
			SetConsoleTextAttribute(outputH, 7);
			strangeError++;
			continue;
		}
		GumboNode *node2;
		int mark;
		node2 = findTag(item, GUMBO_TAG_UL);

		if (node2 != nullptr) {
			node2 = findTag(node2, GUMBO_TAG_LI);
			if (node2 != nullptr) {
				node2 = (GumboNode*)findTag(node2, GUMBO_TAG_A)->v.element.children.data[1];
				if (node2 != nullptr)
					mark = stoi(node2->v.text.text);
				else mark = 0;
			}
			else mark = 0;

		}
		else mark = 0;





		GumboNode *node3 = findTag(findTag(node1, GUMBO_TAG_DIV), GUMBO_TAG_IMG);//node1's child
		if (node3 == nullptr)
			return;
		string id = gumbo_get_attribute(&node1->v.element.attributes, "href")->value;

		string imageUrl = gumbo_get_attribute(&node3->v.element.attributes, "data-src")->value;
		imageUrl.replace(4, 1, "");
		
		int intid = stoi(id.replace(0, 41, ""));
		thread t(downloadImage, imageUrl, intid, i);
		t.detach();
		mut.lock();
		info[intid] = mark;
		globalUrl[intid] = imageUrl.replace(0, 44, "");
		imageType[intid] = typeA;
		mut.unlock();
	}
	return;

}


/*
sort data
author:Callfromfuture
status:TESTING
Security level:Unsafe 1
*/
multimap<int, int, greater<int>> sortInfo() {
	int i = 0;
	multimap<int, int, greater<int>> infoSorted;
	for (auto beg = info.begin(), end = info.end(); beg != end; beg++) {
		infoSorted.insert(pair<int, int>(beg->second, beg->first));
		i++;
	}
	//output
	return infoSorted;

}

/*
imageDownloader callback function
author:Callfromfuture
status:TESTING
Security level:Unsafe 1
*/
size_t imageCB(void *ptr, size_t size, size_t nmemb, void *userdata) {
	FILE* stream = (FILE*)userdata;
	if (!stream) {
		SetConsoleTextAttribute(outputH, FOREGROUND_RED | FOREGROUND_INTENSITY);
		printf("!!! No stream\n");
		SetConsoleTextAttribute(outputH, 7);
		return 0;
	}

	size_t written = fwrite((FILE*)ptr, size, nmemb, stream);
	return written;
}

/*
image downloader
author:Callfromfuture
status:TESTING
Security level:Unsafe 1
*/
void downloadImage(string url, int id, int num) {
	stringstream a;
	a << "temp/" << id << ".jpg";
	string b = a.str();
	struct stat f_stat;
	stat(b.c_str(), &f_stat);
	if (f_stat.st_size != 0)
		return;
	while (running > 200) {
		Sleep(500);
	}
	mut2.lock();
	running++;
	mut2.unlock();

	FILE *fp = fopen(b.c_str(), "wb+");

	if (!fp) {
		SetConsoleTextAttribute(outputH, FOREGROUND_RED | FOREGROUND_INTENSITY);
		cout << "[WARN]connot open file:"<< id << endl;
		SetConsoleTextAttribute(outputH, 7);
		errorUrlMap[id] = url;
		return;
	}

	CURL *curl;
	CURLcode res;

	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		struct curl_slist *headers = NULL;
		headers = curl_slist_append(headers, "Referer:http://www.pixiv.net/");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 100);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, imageCB);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		res = curl_easy_perform(curl);
		curl_slist_free_all(headers);
		if (res) {
			SetConsoleTextAttribute(outputH, FOREGROUND_RED | FOREGROUND_INTENSITY);
			if (res == CURLE_OPERATION_TIMEOUTED)
				printf("[WARN]%d:download error\n", id);
			printf("[WARN]download failed:%s\n", curl_easy_strerror(res));
			SetConsoleTextAttribute(outputH, 7);
			errorUrlMap[id] = url;
		}
	}
	fclose(fp);
	mut2.lock();
	running--;
	mut2.unlock();
	curl_easy_cleanup(curl);
	printf("[INFO]%d:completed\n", id);
}

/*
GBKtoUTF8
author:Internet
status:Using
Security level:Safe 5
*/
std::string GBKToUTF8(const std::string& strGBK) {
	std::string strOutUTF8 = "";
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
std::string UTF8ToGBK(const std::string& strUTF8)
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
	std::string strTemp(szGBK);
	try{
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
	for (unsigned int i = 0; i<URL.size(); i++) {
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
	for (unsigned int i = 0; i<URL.size(); i++) {
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