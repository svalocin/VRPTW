#ifndef _algorithm_h_
#define _algorithm_h_

#include <string>
#include "matrix.h"
#include "ecology_class.h"

//算法接口
class IAlgorithm
{
public:
	//运行算法
	virtual std::string Run(char* data_file_path) = 0;
};

class GeneticAlgorithm : public IAlgorithm
{
public:
	GeneticAlgorithm() : vehicle_capacity(0), p_now_rev_cost_sum(0), reproduce_count(max_reproduce_count), gene_count(0) {}
	~GeneticAlgorithm(){}
	std::string Run(char* data_file_path);

protected:
	//选择函数
	virtual int Selection() const;
	//交叉函数
	virtual void  Crossover();
	//变异函数
	virtual void  Mutation();
	//适应度函数
	//virtual void  Fitness();
	//挑选子代
	virtual void SelectOffspring();
	//是否繁殖
	virtual bool IsSire();

private:
	const int max_reproduce_count = 300;	//最大繁殖次数
	const int max_popu_indi_count = 50;		//种群最大个体数
	const double pc = 0.9;					//交叉概率
	const double pm = 0.9;					//突变概率

	long vehicle_capacity;		//车辆载重
	_Gene _start_vehicle;		//起点仓库

	Popu popu;					//种群
	size_t popu_indi_count_now; //当前种群个体数量
	double p_now_rev_cost_sum;	//当前种群的反成本总和
	_Indi _indi_optimal;		//最优个体
	int reproduce_count;		//繁殖次数
	int gene_count;				//基因数量

	//读取文件数据
	//  数据文件格式：
	//      [车辆载重] [车辆数]
	//      [  起点仓库ID] [坐标X] [坐标Y] [需求] [最早到达时间] [最迟到达时间] [服务时间]
	//      [非起点仓库ID] [坐标X] [坐标Y] [需求] [最早到达时间] [最迟到达时间] [服务时间]
	//      [非起点仓库ID] [坐标X] [坐标Y] [需求] [最早到达时间] [最迟到达时间] [服务时间]
	//      ...
	//  注意： 1.数据之间以空格分开;2.仓库数据第一行必须为起点仓库
	void InputData(char* data_file_path);
	//输出文件数据
	// |	表示路线
	// +	表示起点或终点
	// *	表示车辆数
	// =	表示成本
	// :	表示花费时间
	std::string OutputData(_Indi _i, time_t start_t, time_t end_t);
};

#endif
