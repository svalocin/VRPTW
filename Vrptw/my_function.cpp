#include <fstream>
#include <ctime>
#include "my_function.h"

using std::string;
using std::vector;
using std::to_string;

bool String_IsNullOrEmpty(string& s) {
	typedef string::const_iterator  string_iter;

	string_iter _s_e = s.end();
	string_iter _s = s.begin();

	while (_s != _s_e && isspace(*_s))
		++_s;

	if (_s != _s_e)
		return true;

	return false;
}

vector<string> SplitStringByEmpty(string& s) {
	vector<string> ret;
	typedef string::size_type string_size;
	string_size i = 0;

	while (i != s.size())
	{
		//获得单词的起点
		while (i != s.size() && isspace(s[i]))
			++i;
		string_size j = i;
		//获得单词的终点
		while (j != s.size() && !isspace(s[j]))
			++j;

		if (i != j) {
			ret.push_back(s.substr(i, j - i));
			i = j;
		}
	}

	return ret;
}

vector< vector<string> > ReadFile(char* path) {
	std::ifstream fin(path);
	vector< vector<string> > v;
	string s;
	while (std::getline(fin, s))
	{
		if (String_IsNullOrEmpty(s)) {
			vector<string> ss = SplitStringByEmpty(s);
			v.push_back(ss);
		}
	}
	fin.close();
	return v;
}

void WriteFile(char* path, string str) {
	std::ofstream out(path);
	out << str << std::endl;
	out.close();
}

double MyRand() { return rand() % 1000 / 1000.0; }

string OutTime(time_t end_t, time_t start_t) {
	long t = (long)difftime(end_t, start_t); //单位秒
	long h = t / 3600;
	long m = (t % 3600); //去掉小时后的剩余秒数
	long s = m % 60;
	m = m / 60; //真正的分钟数

	if (end_t > start_t && h == 0 && m == 0 && s == 0){ s = 1; } //不足一秒，补足为一秒

	return to_string(h) + ':' + to_string(m) + ':' + to_string(s);
}

int CalProbabilityCount(const double probability, int min_number) {
	int number = min_number;
	//生成交换次数
	while (MyRand() < probability)
	{
		++number;
	}
	return number;
}

bool FileIsHas(char* path) {
	std::fstream file;
	file.open(path);
	if (file){
		file.close();
		return true;
	}
	else{
		file.close();
		return false;
	}
}