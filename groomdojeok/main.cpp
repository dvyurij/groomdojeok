// ver 1.01

//#define _WINSOCK_DEPRECATED_NO_WARNINGS
//#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
//#pragma comment(lib,"ws2_32.lib")

using namespace std;

void ConvertSpecialChar(string &sp);
void CropBracedArea(string &inputstr, size_t from, size_t to);
void RemoveSign(string &inputstr);
std::string ReplaceAll(std::string &str, const std::string& from, const std::string& to);
int parsehtml(string &_html);
//int run(string inputurl, string &strbuf);
int CopyTextToClipboard(char *ap_string);
char *CopyClipboardToText();

int main()
{
	int counter = 0;
	int err = 0;
	//string url;
	//string buf;
	string htmlcode;
	while (1)
	{
		cout << "*종료 : X버튼\n";
		cout << "아무 키를 입력하여 시작하세요.\n";
		system("pause");
		htmlcode = CopyClipboardToText();
		/*
		cout << htmlcode << endl;
		url = CopyClipboardToText();
		cout << url << endl;
		cin >> url;
		run(url, buf);
		//vector<char> writable(url.begin(), url.end());
		*/
		if ((err = parsehtml(htmlcode)) == -1)
		{
			system("cls");
			cout << "올바른 html을 복사해주세요." << endl;
		}
		else if (err == 0)
		{
			vector<char> writable(htmlcode.begin(), htmlcode.end());
			writable.push_back('\0');
			char* ptr = &writable[0];
			//std::cout << ptr;
			CopyTextToClipboard(ptr);

			system("cls");
			cout << counter << endl;
			cout << "클립보드에 복사되었습니다.\n\n";
			counter++;
		}
		cout << "<현재 클립보드>" << endl;
		cout << htmlcode << endl << endl;

		htmlcode = "";
		err = 0;
	}
	return 0;
}

int parsehtml(string &_html)
{
	if (_html.substr(0, 5) != "<body")
		return -1;

	size_t left_pos, right_pos = 0;

	if ((left_pos = _html.find("#include")) != string::npos)
	{
		cout << "found" << endl;
		cout << left_pos << endl;
	}

	if ((right_pos = _html.find("</div>", left_pos)) != string::npos)
	{
		cout << "found" << endl;
		cout << right_pos << endl;
	}

	CropBracedArea(_html, left_pos, right_pos);
	RemoveSign(_html);
	ConvertSpecialChar(_html);

	_html = "#include <stdio.h>\n\nint main(void)\n" + _html;

	return 0;
}

void ConvertSpecialChar(string &sp)
{
	ReplaceAll(sp, "&quot;", "\"");
	ReplaceAll(sp, "“", "\"");
	ReplaceAll(sp, "”", "\"");
	ReplaceAll(sp, "&amp;", "&");
	ReplaceAll(sp, "&lt;", "<");
	ReplaceAll(sp, "&gt;", ">");
	ReplaceAll(sp, "&nbsp;", " ");
	ReplaceAll(sp, ";", ";\n");
	ReplaceAll(sp, "{", "{\n");
	ReplaceAll(sp, "}", "}\n");
}

void CropBracedArea(string &inputstr, size_t from, size_t to)
{
	size_t left_b, right_b;
	left_b = inputstr.find("{", from);
	right_b = inputstr.rfind("}", to);
	inputstr = inputstr.substr(left_b, right_b - left_b + 1);
}

void RemoveSign(string &inputstr)
{
	string selectedstr;
	size_t left_t, right_t;

	while ((left_t = inputstr.find("<")) != string::npos)
	{
		right_t = inputstr.find(">", left_t);
		selectedstr = inputstr.substr(left_t, right_t - left_t + 1);
		ReplaceAll(inputstr, selectedstr, "");
	}
}

std::string ReplaceAll(std::string &str, const std::string& from, const std::string& to)
{
	size_t start_pos = 0; //string처음부터 검사
	while ((start_pos = str.find(from, start_pos)) != std::string::npos)  //from을 찾을 수 없을 때까지
	{
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // 중복검사를 피하고 from.length() > to.length()인 경우를 위해서
	}
	return str;
}

/*
int run(string inputurl, string &strbuf)
{
//inputurl = "www.google.com";
//string socketmsg = "GET / HTTP / 1.1\r\nHost: www.google.com\r\nConnection: close\r\n\r\n";
string socketmsg = "GET / HTTP / 1.1\r\nHost: " + inputurl + "\r\nConnection: close\r\n\r\n";

WSADATA wsaData;
if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
cout << "WSAStartup failed.\n";
system("pause");
return 1;
}
SOCKET Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
struct hostent *host;
host = gethostbyname(inputurl.c_str());

SOCKADDR_IN SockAddr;
SockAddr.sin_port = htons(80);
SockAddr.sin_family = AF_INET;
SockAddr.sin_addr.s_addr = *((unsigned long*)host->h_addr);
cout << "Connecting...\n";
if (connect(Socket, (SOCKADDR*)(&SockAddr), sizeof(SockAddr)) != 0) {
cout << "Could not connect";
system("pause");
return 1;
}
cout << "Connected.\n";

send(Socket, socketmsg.c_str(), strlen(socketmsg.c_str()), 0);
char buffer[10000];
int nDataLength;
while ((nDataLength = recv(Socket, buffer, 10000, 0)) > 0) {
int i = 0;
while (buffer[i] >= 32 || buffer[i] == '\n' || buffer[i] == '\r') {
cout << buffer[i];
//strbuf += buffer[i];
i += 1;
}
}
closesocket(Socket);
WSACleanup();
system("pause");
return 0;
}
*/
int CopyTextToClipboard(char *ap_string)
{
	int string_length = strlen(ap_string) + 1;

	HANDLE h_data = ::GlobalAlloc(GMEM_DDESHARE | GMEM_MOVEABLE, string_length);

	char *p_data = (char *)::GlobalLock(h_data);

	if (NULL != p_data)
	{
		memcpy(p_data, ap_string, string_length);

		::GlobalUnlock(h_data);

		if (::OpenClipboard(NULL)) // handler를 집어넣는게 일반적.
		{
			::EmptyClipboard();
			::SetClipboardData(CF_TEXT, h_data);
			::CloseClipboard();
		}
	}
	return 0;
}

char *CopyClipboardToText()
{
	unsigned int priority_list = CF_TEXT;
	char *p_string = NULL;
	// 클립보드에는 문자열만 저장할수 있는것이 아니기 때문에  
	// 현재 문자열이 저장되어 있는지 확인한다. 
	if (::GetPriorityClipboardFormat(&priority_list, 1) == CF_TEXT)
	{
		// 클립보드에 있는 비트맵 정보를 얻기 위해서 클립보드를 연다. 
		if (::OpenClipboard(NULL))   // handler를 집어넣는게 일반적
		{
			// 클립보드에서 문자열이 저장된 메모리의 핸들 값을 얻는다. 
			HANDLE h_clipboard_data = ::GetClipboardData(CF_TEXT); if (h_clipboard_data != NULL)
			{
				// 메모리 핸들 값을 이용하여 실제 사용가능한 주소를 얻는다. 
				char *p_clipboard_data = (char *)::GlobalLock(h_clipboard_data);
				// 클립보드에 저장된 문자열의 길이를 구한다. ('\0'포함 크기) 
				int string_len = strlen(p_clipboard_data) + 1;
				// 클립보드에 저장된 문자열을 저장하기 위해 메모리를 할당한다. 
				p_string = new char[string_len];
				// 할당된 메모리에 클리보드 문자열을 복사한다. 
				memcpy(p_string, p_clipboard_data, string_len);
				// 문자열을 복사하기 위해서 Lock했던 메모리를 해제한다. 
				::GlobalUnlock(h_clipboard_data);
			}
			// 클립보드를 닫는다. 
			::CloseClipboard();
		}
	}
	// 클립보드 문자열을 복사한 메모리의 주소를 반환한다. 
	return p_string;
}