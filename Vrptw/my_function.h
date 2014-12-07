#ifndef _my_function_h_
#define _my_function_h_

#include <vector>
#include <string>

//判断字符串是否为空值或由空白字符组成
bool String_IsNullOrEmpty(std::string& s);

//按空格分割字符串
std::vector<std::string> SplitStringByEmpty(std::string& s);

//读取文件
std::vector< std::vector<std::string> > ReadFile(char* path);

//写文件文件
void WriteFile(char* path, std::string str);

//求随机数
double MyRand();

//时间输出字符串，输入秒，返回 00: 00: 00
std::string OutTime(time_t, time_t);

//根据概率求数
//1. 设置结果数为最小数
//2. 生成一个小于1，大于0的随机数，当这个随机数小于概率时结果数+1
//3. 不断重复 2，直到随机数大于概率时，返回结果数
//probability：概率；min_number：最小数
int CalProbabilityCount(const double probability, int min_number);

//判断文件是否存在
bool FileIsHas(char* path);

#endif