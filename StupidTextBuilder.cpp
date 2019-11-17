#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include "json/json.h"
//���ͷ�ļ��������ַ�ת���õ�
#include <windows.h>


using namespace std;

// ���������Ժ���ķϻ�
vector<string> before;
// ����������ǰ��ķϻ�
vector<string> after;
// ����������Ҫ�ϻ���Դ,x ����������
vector<string> bosh;
// ��������:a ����ǰ��滰��b�������滰
vector<string> famous;
// ����
string title;

vector<string> Keywords;

void ReadJson();
//����һ��ϻ�
string Crap();
//����һ��(Ҫȷ��һ���ж��ٷϻ�)
string Paragraph(int _n,int _max ,int _min);
//GBKתUTF8����
string GbkToUtf8(const char *src_str);



int main()
{
	//�Ȳ������������
	srand((int)time(0));

	ReadJson();
	ofstream saveFile("data.txt");
	//�ж��ٸ��ؼ���
	int n;
	string temp;
	string UTFtemp;

	cout << "��������Ŀ�ı��⣺";
	cin >> temp;
	UTFtemp = GbkToUtf8(temp.c_str());
	Keywords.push_back(UTFtemp);

	cout << "\n�����ж��ٸ��ؼ��֣�";
	cin >> n;

	for (int i = 0; i < n ; i++)
	{
		cin >> temp;
		UTFtemp = GbkToUtf8(temp.c_str());
		Keywords.push_back(UTFtemp);
	}

	cout << "\n��Ҫ���ɶ��ٶΣ�";
	cin >> n;

	//for (auto i = Keywords.begin(); i != Keywords.end(); i++)
	//{
	//	saveFile << (*i);
	//}
	saveFile << Paragraph(n, 3, 5);

	
	cout << "\n�ı��������" << endl;

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

#pragma region ֱ�ӻ�ȡKey
	/** ����ֱ�ӵõ�Json��Key */
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
	//�����������
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

	// ���������ɵ�һ��bosh
	string tempBosh = bosh[b];
	auto ix = tempBosh.find('x');
	if (ix != -1)
	{
		replace(tempBosh.begin(), tempBosh.end(), 'x', ' ');
		tempBosh.insert(ix, Keywords[keyW]);
	}
	temp.append(tempBosh);
	// ����famous
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

