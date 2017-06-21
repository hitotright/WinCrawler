#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include <WinSock2.h>
#include <queue>
#include <set>

#pragma comment(lib,"WS2_32.lib")
using namespace std;

queue<string> hrefUrl;
set<string> visitedUrl;
set<string> visitedImg;

//����URL������������������Դ�� 
bool ParseURL(const string & url, string & host, string & resource)
{
	if (strlen(url.c_str()) > 2000) {
		return false;
	}
	const char * pos = strstr(url.c_str(), "http://");
	if (pos == NULL)
	{
		pos = url.c_str();
	}
	else {
		pos += strlen("http://");
	}

	if (strstr(pos, "/") == 0) {
		return false;
	}
	char pHost[100];
	char pResource[2000];

	sscanf(pos, "%[^/]%s", pHost,pResource);
	host = pHost;
	resource = pResource;

	return true;

	
}

//ʹ��Get���󣬵õ���Ӧ  
bool GetHttpResponse(const string & url, char * &response, int &bytesRead)
{
	string host, resource;
	if (!ParseURL(url, host, resource)) {
		cout << "can not parse the url" << endl;
		return false;
	}

	//����socket
	struct hostent * hp = gethostbyname(host.c_str());
	if (hp == NULL) {
		cout << "can not find host address!" << endl;
		return false;
	}

	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock <= 0) {
		cout << "can not create sock!" << endl;
		return false;
	}

	//���������ַaddr
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(80);
	memcpy(&addr.sin_addr, hp->h_addr, 4);

	//��������
	if (0 == connect(sock, (SOCKADDR*)&addr, sizeof(addr)))
	{
		cout << "can not connect:" << url << endl;
		return false;
	}

}

//��ȡ���е�URL�Լ�ͼƬURL  
void HTMLParse(string & htmlResponse, vector<string> & imgurls, const string & host);

//��URLת��Ϊ�ļ���  
string ToFileName(const string &url);

//����ͼƬ��img�ļ���  
void DownLoadImg(vector<string> & imgurls, const string &url);

//��ȱ���  
void BFS(const string & url) {
	char * response;
	int bytes;
	if (!GetHttpResponse(url, response, bytes))
	{
		cout << "url have some error!" << endl;
		return;
	}


}

void main()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		printf("wsastartup error!");
		return;
	}

	CreateDirectory(TEXT("./img"), 0);
	CreateDirectory(TEXT("./html"), 0);

	string urlStart = "http://item.taobao.com/item.htm?spm=a230r.1.14.19.sBBNbz&id=36366887850&ns=1#detail";

	BFS(urlStart);

	return;
}
