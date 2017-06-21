#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include <WinSock2.h>
#include <queue>
#include <set>

#pragma comment(lib,"WS2_32.lib")
using namespace std;

#define DEFAULT_PAGE_BUF_SIZE 1048576 
queue<string> hrefUrl;
set<string> visitedUrl;
set<string> visitedImg;

//解析URL，解析出主机名，资源名 
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

//使用Get请求，得到响应  
bool GetHttpResponse(const string & url, char * &response, int &bytesRead)
{
	string host, resource;
	if (!ParseURL(url, host, resource)) {
		cout << "can not parse the url" << endl;
		return false;
	}

	//创建socket
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

	//创建服务地址addr
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(80);
	memcpy(&addr.sin_addr, hp->h_addr, 4);

	//创建连接
	if (0 != connect(sock, (SOCKADDR*)&addr, sizeof(addr)))
	{
		cout << "can not connect:" << url << endl;
		closesocket(sock);
		return false;
	}

	//准备发送数据
	string request = "GET " + resource + " HTTP/1.1\r\nHost:" + host + "\r\nConnection:Close\r\n\r\n";

	//发送数据
	if (SOCKET_ERROR == send(sock, request.c_str(), request.size(), 0))
	{
		cout << "send error" << endl;
		closesocket(sock);
		return false;
	}

	//接收数据
	int m_nContentLength = DEFAULT_PAGE_BUF_SIZE;
	char *pageBuf = (char *)malloc(m_nContentLength);
	memset(pageBuf, 0, m_nContentLength);

	bytesRead = 0;
	int ret = 1;
	cout << "Read: ";
	while (ret > 0) {
		ret = recv(sock, pageBuf + bytesRead, m_nContentLength - bytesRead, 0);

		if (ret > 0)
		{
			bytesRead += ret;
		}

		if (m_nContentLength - bytesRead<100) {
			cout << "\nRealloc memorry" << endl;
			m_nContentLength *= 2;
			pageBuf = (char*)realloc(pageBuf, m_nContentLength);       //重新分配内存  
		}
		cout << ret << " ";
	}
	cout << endl;

	pageBuf[bytesRead] = '\0';
	response = pageBuf;
	closesocket(sock);
	return true;


}

//提取所有的URL以及图片URL  
void HTMLParse(string & htmlResponse, vector<string> & imgurls, const string & host);

//把URL转化为文件名  
string ToFileName(const string &url);

//下载图片到img文件夹  
void DownLoadImg(vector<string> & imgurls, const string &url);

//广度遍历  
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
