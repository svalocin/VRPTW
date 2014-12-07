#ifndef _ecology_class_h_
#define _ecology_class_h_

// std::shared_ptr<Cust>		_Gene	基因
// std::vector<_Gene>			Chro	染色体
// std::shared_ptr<Individual>	_Indi	个体
// std::vector<_Indi>			Popu	种群

#include <memory>
#include <vector>
#include <string>
#include "path.h"

class Cust;

typedef std::shared_ptr<Cust> _Gene;	//基因
typedef std::vector<_Gene> Chro;		//染色体

//个体类
class Individual : public IPath {
public:
	//构造函数
	//_start_vehicle：起点仓库；_chro：染色体；vehicle_capacity：车辆载重
	Individual(_Gene _start_vehicle, Chro* _chro, long vehicle_capacity);
	virtual ~Individual();

	//输出路径字符串
	// |	表示路线
	// +	表示起点或终点
	std::string Get_path_string() const;

	inline double Get_cost() {
		if (cost < 0) {
			cost = CalCost();
			return cost;
		}
		return cost;
	}

	inline Chro& Get_chro() const { return *_chro; }

	//反成本 = 最大成本 - 成本
	inline double Get_rev_cost() {
		return (double)max_cost - Get_cost();
	}

	//路线使用的车辆数目
	inline int Get_vehicle_count() {
		if (path.size() > 0 && cost > -1){
			return path.size();
		}
		else
		{
			CalCost();
			return path.size();
		}
	}

	//优化基因（优化结果不一定会比原来的好）
	friend void GeneOptimization(Individual& indi);

	//从头部删除重复基因
	//检查第一个基因到第n个基因，是否跟后面的基因重复，若重复删除后面的基因
	friend void DeleteRepeatGeneFromBegin(Individual& indi, int n);

protected:
	Chro* _chro;			//染色体
	_Gene _start_gene;		//起点仓库
	std::vector<int> path;	//路径
	double cost;			//成本，初始值为 -1，表示还没经过计算
	long vehicle_capacity;	//车辆载重
	double max_cost;		//最大成本

	//计算成本
	virtual double CalCost();
	//重置数据，更改基因时必须调用这个函数
	inline void SetProperty(){
		this->path.clear();	//清空路径
		this->cost = -1;	//重置成本
		this->max_cost = 1000 * _chro->size();
	}
};

typedef std::shared_ptr<Individual> _Indi;	//个体
typedef std::vector<_Indi> Popu;			//种群

typedef std::vector<_Gene>::const_iterator Chro_const_iter;
typedef std::vector<_Gene>::iterator Chro_iter;
typedef std::vector<_Indi>::const_iterator Popu_const_iter;
typedef std::vector<_Indi>::iterator Popu_iter;

//个体的降序谓词（按反成本排序）
bool IndiDESC(const _Indi&, const _Indi&);
//基因的升序谓词（按仓库的右时间排序）
bool GeneASC(const _Gene&, const _Gene&);
//求种群成本总和
double PopuSum(const Popu&);

#endif
