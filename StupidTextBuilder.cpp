#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include "json/json.h"
//这个头文件是用来字符转码用的
#include <windows.h>


using namespace std;

// 在名人名言后面的废话
vector<string> before;
// 在名人名言前面的废话
vector<string> after;
// 代表文章主要废话来源,x 处插入主题
vector<string> bosh;
// 名人名言:a 代表前面垫话，b代表后面垫话
vector<string> famous;
// 标题
string title;

vector<string> Keywords;

void ReadJson();
//生成一句废话
string Crap();
//生成一段(要确定一段有多少废话)
string Paragraph(int _n,int _max ,int _min);
//GBK转UTF8编码
string GbkToUtf8(const char *src_str);



int main()
{
	//先产生随机数种子
	srand((int)time(0));

	ReadJson();
	ofstream saveFile("data.txt");
	//有多少个关键字
	int n;
	string temp;
	string UTFtemp;

	cout << "请输入题目的标题：";
	cin >> temp;
	UTFtemp = GbkToUtf8(temp.c_str());
	Keywords.push_back(UTFtemp);

	cout << "\n本文有多少个关键字：";
	cin >> n;

	for (int i = 0; i < n ; i++)
	{
		cin >> temp;
		UTFtemp = GbkToUtf8(temp.c_str());
		Keywords.push_back(UTFtemp);
	}

	cout << "\n想要生成多少段：";
	cin >> n;

	//for (auto i = Keywords.begin(); i != Keywords.end(); i++)
	//{
	//	saveFile << (*i);
	//}
	saveFile << Paragraph(n, 3, 5);

	
	cout << "\n文本生成完毕" << endl;

	std::cin;
}

void ReadJson() {


	Json::Value root;
	Json::Reader reader;
	ifstream ifs("data.json");
	if (!reader.parse(ifs, root))
	{
		return;
	}

#pragma region 直接获取Key
	/** 可以直接得到Json的Key */
//vector<string> strKeys;
//Json::Value::Members members;
//members = root.getMemberNames();
//for (auto i = members.begin(); i != members.end(); i++)
//{
//	strKeys.push_back(*i);		
//}


#pragma endregion

	string temp = "";
	for (auto i = root["famous"].begin(); i != root["famous"].end(); i++)
	{
		famous.push_back((*i).asString());
	}
	for (auto i = root["bosh"].begin(); i != root["bosh"].end(); i++)
	{
		bosh.push_back((*i).asString());
	}
	for (auto i = root["after"].begin(); i != root["after"].end(); i++)
	{
		after.push_back((*i).asString());
	}
	for (auto i = root["before"].begin(); i != root["before"].end(); i++)
	{
		before.push_back((*i).asString());
	}
}

string Crap() {
	//先生成随机数
	int b = rand() % (bosh.size()-1);
	int f = rand() % (famous.size() - 1);
	int aft = rand() % (after.size() - 1);
	int bef = rand() % (before.size() - 1);
	int keyW = 0;
	if (Keywords.size() > 1 )
	{
		keyW = rand() % (Keywords.size() - 1);
	}

	string temp;

	// 这里是生成第一段bosh
	string tempBosh = bosh[b];
	auto ix = tempBosh.find('x');
	if (ix != -1)
	{
		replace(tempBosh.begin(), tempBosh.end(), 'x', ' ');
		tempBosh.insert(ix, Keywords[keyW]);
	}
	temp.append(tempBosh);
	// 生成famous
	string tempfamous = famous[f];
	auto ia = tempfamous.find('a');
	auto ib = tempfamous.find('b');
	replace(tempfamous.begin(), tempfamous.end(), 'a', ' ');
	replace(tempfamous.begin(), tempfamous.end(), 'b', ' ');
	
	tempfamous.insert(ib, after[aft]);
	tempfamous.insert(ia, before[bef]);
	temp.append(tempfamous);

	return temp;
}

string Paragraph(int _n, int _max, int _min) {

	string temp;
	int range = 1;

	for (int i = 0; i < _n; i++)
	{
		temp.append("  ");
		range = _min + rand() % (_max - _min);
		for (int i = 0; i < range; i++)
		{
			temp.append(Crap());
		}
		temp.append("\n");
	}

	return temp;
}

string GbkToUtf8(const char *src_str)
{
	int len = MultiByteToWideChar(CP_ACP, 0, src_str, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, src_str, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	string strTemp = str;
	if (wstr) delete[] wstr;
	if (str) delete[] str;
	return strTemp;
}

