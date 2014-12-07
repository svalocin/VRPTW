#include <string>
#include <iostream>
#include "my_function.h"
#include "algorithm.h"

using std::string;

//主函数
//参数1：数据文件路径；参数2（可选）：结果文件输出路径
int main(int argc, char* argv[10])
{
	if(argc > 1) {
		char* path = argv[1];

		if(!FileIsHas(path)) {
			std::cout << "文件路径不正确，请输入正确路径" << std::endl;
			return 0;
		}

		IAlgorithm* alg = new GeneticAlgorithm();
		string out_string = alg->Run(path);
		delete alg;

		char* out_path = "result.txt";
		if(argc > 2) {
			out_path = argv[2];
		}

		WriteFile(out_path, out_string);

		std::cout << "计算结束，请打开 " << out_path << " 查看结果\n" <<
			"数据文件格式如下：\n"	<<
			" |	表示路线\n"			<<
			" +	表示起点或终点\n"	<<
			" *	表示车辆数\n"		<<
			" =	表示成本\n"			<< std::endl;
	}
	else {
		std::cout << "请输入参数" << std::endl;
	}
	return 0;
}
