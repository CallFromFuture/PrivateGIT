#include "PixivScanner.h"

int n = 0;
//string Rsp[num];
size_t write_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
	std::string * pStream = static_cast<std::string *>(stream);
	(*pStream).append((char *)ptr, size * nmemb);

	return size * nmemb;
}

int main() {
	//init
	curl_global_init(CURL_GLOBAL_ALL);
	//main loop

	//use Chinese or Japanese
	//keywordA = escapeURL(GBKToUTF8(keyword));

	readFile();
	if (MODE != "SORT") {
		initGet();
		
		int n = 0;
		for (auto beg = errorUrlVec.begin(), end = errorUrlVec.end(); beg != end; beg++) {
			n++;
			string a = *beg;
			thread T(curlEasyThread,a, PROXY, 100000, 10000 + n);
			T.detach();
		}

		printf("please wait until %d", 10000 + n);
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

	ofstream out(keywordA + ".html");
	out << "<base target='_blank'><table border='1'>" << endl;
	int i = 0;
	stringstream buffer1, buffer2;
	for (auto beg = sortedInfo.begin(), end = sortedInfo.end(); beg != end; beg++) {
		string or = globalUrl[beg->second];
		string href = or.replace(or .size() - 15, 15, "");
		buffer1 << "<th>" << beg->first << "<a href='http://i4.pixiv.net/img-original/img/"<<href<<".jpg"<<"' download>JPG</a>/"<< "<a href='http://i4.pixiv.net/img-original/img/" << href << ".png" << "' download>PNG</a>" <<"</th>" << endl;//TODO_Optional:add url to pixiv 
		//buffer2 << "<td><a href='http://i3.pixiv.net/c/1200x1200/img-master/img/" << globalUrl[beg->second] << "' download='" << beg->second << ".jpg'><img src='temp/" << beg->second << ".jpg'></img></a>" << endl;
		buffer2 << "<td><img src='temp/" << beg->second << ".jpg'></img>" << endl;
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
		buffer1 << beg->first << " " << beg->second << " " << globalUrl[beg->first] << endl;
	ofstream out(keywordA+".out", ios::out);
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
	ifstream in(keywordA+".out");
	std::string str((std::istreambuf_iterator<char>(in)),
		std::istreambuf_iterator<char>());
	stringstream input(str);
	int key;
	int value1;
	string value2;
	while (input >> key >> value1 >> value2) {
		if (value2 == "")
			cout << "read failed" << endl;
		info[key] = value1;
		globalUrl[key] = value2;
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
	double dff;
	__int64  c1, c2;
	QueryPerformanceFrequency(&large_interger);
	dff = large_interger.QuadPart;
	QueryPerformanceCounter(&large_interger);
	c1 = large_interger.QuadPart;

	stringstream out;
	out << "http://www.pixiv.net/search.php?word=" << keywordA << "&p=" << 1;
	curlEasyThread(out.str(), PROXY, TIMEOUT, 0);

	if (numofImage != 0)
		curlMultithreading(keywordA, ceil(float(numofImage) / 20.0f));
	else if (numofImage > 20000)
		curlMultithreading(keywordA, 1000);
	else curlMultithreading(keywordA, 0);

	system("pause");
	//	curlMultithreading(keywordA, 3);


	QueryPerformanceCounter(&large_interger);
	c2 = large_interger.QuadPart;
	cout << "time(ms) : " << (c2 - c1) * 1000 / dff << endl;
}

/*
create thread to get imageInfo
author:Callfromfuture
status:TESTING
Security level:Unsafe 1
*/
int curlMultithreading(string keyword, int num) {
	//vector<string> RspVec(num);
	int rn = num, a = 0, aa = 1;
	if (num == 0) {
		cout << "Nothing" << endl;
		return -1;
	}
	while (aa < num) {
		if (Nocompleted <= 70) {
			stringstream out;
			out << "http://www.pixiv.net/search.php?word=" << keyword << "&p=" << aa + 1;
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
		printf("running: %d\n", Nocompleted);
		Sleep(1000);
	}
	return 0;
}

/*
Data Scanner
author:Callfromfuture
status:TESTING
Security level:Unsafe 1
*/
bool curlEasyThread(const string sUrl, const string sProxy, unsigned int uiTimeout, int orderNum) {
	//cout << orderNum << ": started" << endl;
	printf("%d:started\n", orderNum);
	CURL *curl;
	CURLcode res;
	BOOL rtc;
	string sRsp;
	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, sUrl.c_str());
		if (uiTimeout > 0)
			curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, uiTimeout);
		if (!sProxy.empty())
			curl_easy_setopt(curl, CURLOPT_PROXY, sProxy.c_str());
		struct curl_slist *headers = NULL;
		headers = curl_slist_append(headers, "Cookie:__utma=235335808.291152945.1488708965.1491560081.1491564793.8; __utmb=235335808.15.10.1491564793; __utmc=235335808; __utmv=235335808.|2=login%20ever=yes=1^3=plan=normal=1^4=p_ab_id_2=8=1^5=gender=male=1^6=user_id=17351941=1^9=p_ab_id=5=1^10=p_ab_id_2=8=1^11=illust_tag_placeholder=yes=1^12=fanbox_subscribe_button=blue=1^13=fanbox_fixed_otodoke_naiyou=yes=1^14=hide_upload_form=no=1^15=machine_translate_test=no=1; __utmz=235335808.1488708965.1.1.utmcsr=(direct)|utmccn=(direct)|utmcmd=(none); _ga=GA1.2.291152945.1488708965; bookmark_tag_order=desc; bookmark_tag_type=count; bookToggle=cloud; module_orders_mypage=%5B%7B%22name%22%3A%22recommended_illusts%22%2C%22visible%22%3Atrue%7D%2C%7B%22name%22%3A%22everyone_new_illusts%22%2C%22visible%22%3Atrue%7D%2C%7B%22name%22%3A%22following_new_illusts%22%2C%22visible%22%3Atrue%7D%2C%7B%22name%22%3A%22mypixiv_new_illusts%22%2C%22visible%22%3Atrue%7D%2C%7B%22name%22%3A%22fanbox%22%2C%22visible%22%3Atrue%7D%2C%7B%22name%22%3A%22featured_tags%22%2C%22visible%22%3Atrue%7D%2C%7B%22name%22%3A%22contests%22%2C%22visible%22%3Atrue%7D%2C%7B%22name%22%3A%22sensei_courses%22%2C%22visible%22%3Atrue%7D%2C%7B%22name%22%3A%22spotlight%22%2C%22visible%22%3Atrue%7D%2C%7B%22name%22%3A%22booth_follow_items%22%2C%22visible%22%3Atrue%7D%5D; p_ab_id=5; p_ab_id_2=8; PHPSESSID=17351941_c9adfe6e87cd403ab97ca0482733d60a; pixiv_embed=pix; login_ever=yes; a_type=0; is_sensei_service_user=1");
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		//curl_easy_setopt(curl, CURLOPT_COOKIE, COOKIE);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &sRsp);
		res = curl_easy_perform(curl);
		if (res) {
			cerr << "url_easy_perform() failed: " << curl_easy_strerror(res) << endl;
			errorUrlVec.push_back(sUrl);
			rtc = false;
		}
		else {

			int code = parseHtml(UTF8ToGBK(sRsp));

			if (code == 0) {
				//cout << "number: " << orderNum << "completed";
				printf("%d:completed\n", orderNum);
				//threadCom[orderNum] = true;
				
				rtc = true;
			}
			else if (code == -1) {
				//cout << orderNum << ": Root Node Error" << endl;
				printf("%d: Root Node Error\n", orderNum);
				rtc = false;
			}
			else {
				//cout << orderNum << ": Get Num Error" << endl;
				printf("%d: Get Num Error\n", orderNum);
				rtc = false;
			}

		}
	}
	mut3.lock();
	Nocompleted--;
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
	//ofstream t("1.html", ios::out);
	//t << html;
	//t.close();
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
		cout << "first" << endl;
		GumboNode *node2 = findTag(temp, GUMBO_TAG_DIV, "class", "column-label");
		if (node2 == nullptr) return -2;
		node2 = findTag(node2, GUMBO_TAG_SPAN, "class", "count-badge");
		if (node2 == nullptr) return -2;
		string a = ((GumboNode*)node2->v.element.children.data[0])->v.text.text;
		numofImage = stoi(a.replace(a.size() - 1, 1, ""));
		firstRun = false;
	}
	temp = findTag(temp, GUMBO_TAG_SECTION, "class", "column-search-result");
	//temp = findTag(temp, GUMBO_TAG_SECTION);
	if (temp == nullptr) return -1;
	temp = findTag(temp, GUMBO_TAG_UL, "class", "_image-items autopagerize_page_element");
	if (temp == nullptr) return -1;
	//thread a(getImageInfo,temp,html);
	//a.detach();
	getImageInfo(temp, html);
	//cout << html.size() << endl;

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

	//std::lock_guard<std::mutex> lck(mut);
	vector<thread> T;
	GumboVector items = root->v.element.children;
	//vector<bool> threadCom2(false,items.length);
	for (int i = 0; i < items.length; i++) {
		GumboNode *item = (GumboNode*)(items.data[i]);

		if (item == nullptr)
			return;
		GumboNode *node1 = findTag(item, GUMBO_TAG_A, "class", "work  _work ");	//root's child
		if (node1 == nullptr)
			node1 = findTag(item, GUMBO_TAG_A, "class", "work  _work multiple ");	//root's child		
		if (node1 == nullptr)
			node1 = findTag(item, GUMBO_TAG_A, "class", "work  _work ugoku-illust ");	//root's child
		if (node1 == nullptr)
			node1 = findTag(item, GUMBO_TAG_A, "class", "work  _work manga multiple ");	//root's child	
		if (node1 == nullptr)
			node1 == findTag(item, GUMBO_TAG_A, "class", "work  _work multiple rtl ");	//root's child		
		if (node1 == nullptr)
			node1 == findTag(item, GUMBO_TAG_A, "class", "work  _work manga ");	//root's child		
		if (node1 == nullptr)
			node1 == findTag(item, GUMBO_TAG_A, "class", "work  _work manga multiple rtl ");	//root's child	

		if (node1 == nullptr) {
			printf("Get Info Error\n");
			//cout << "error" << endl;
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
		//T.push_back(thread(downloadImage, imageUrl, intid, i));
		//downloadImage(imageUrl, intid, i);
		mut.lock();
		info[intid] = mark;
		globalUrl[intid] = imageUrl.replace(0, 44, "");
		mut.unlock();
	}

	//cout << "COMPLETED" << endl;
	return;

	//
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
	//map<int, int> infoSorted;
	for (auto beg = info.begin(), end = info.end(); beg != end; beg++) {
		//infoSorted[beg->second] = beg->first;
		infoSorted.insert(pair<int, int>(beg->second, beg->first));
		i++;
	}
	//output
	return infoSorted;/*
	for (auto beg = infoSorted.begin(), end = infoSorted.end(); beg != end; beg++)
		cout << beg->second << " " << beg->first << " " << globalUrl[beg->second] << endl;
		*/

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
		printf("!!! No stream\n");
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
		cout << "connot open file" << endl;
		errorUrlMap[id] = url;
		//errorUrlVec2.push_back(url);
	}

	//char c = fgetc(fp);
	//if (c != EOF) 
		//return;
	CURL *curl;
	CURLcode res;
	string img;

	curl = curl_easy_init();
	if (curl) {
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		struct curl_slist *headers = NULL;
		headers = curl_slist_append(headers, "Referer:http://www.pixiv.net/");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_TIMEOUT, 100);
		//headers = curl_slist_append(headers, "charset:utf-8");
		//curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "cookie");
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, imageCB);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		res = curl_easy_perform(curl);
		if (res) {
			if (res == CURLE_OPERATION_TIMEOUTED)
				printf("%d:download error\n", id);
				//cout << id << ": download error" << endl;
			//cerr << "download failed: " << curl_easy_strerror(res) << endl;
			printf("download failed:%s\n", curl_easy_strerror(res));
			errorUrlMap[id] = url;
			//errorUrlVec2.push_back(url);
		}
	}
	//threadCom2[num] = true;
	fclose(fp);
	mut2.lock();
	running--;
	mut2.unlock();
	curl_easy_cleanup(curl);
	printf("%d:completed\n", id);
	//cout << id << ": completed" << endl;
	//
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
	delete[]szGBK;
	delete[]wszGBK;
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