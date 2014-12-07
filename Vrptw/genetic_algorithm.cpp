#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <algorithm> 
#include <ctime>
#include "algorithm.h"
#include "cust.h"
#include "ecology_class.h"
#include "my_function.h"

using std::string;
using std::vector;
using std::make_shared;

string GeneticAlgorithm::Run(char* data_file_path){
	//初始化条件
	InputData(data_file_path);
	srand(clock()); //设置随机数的种子

	time_t start_t, end_t;
	start_t = time(NULL);

	//算法开始
	while (IsSire())
	{
		popu_indi_count_now = popu.size();	//设置当前种群个体数
		p_now_rev_cost_sum = PopuSum(popu);	//计算种群总和

		//设置最优个体，最优个体为种群第一个个体，需确保种群是经过排序的
		//种群排序发生在 SelectOffspring() 函数
		if (popu[0]->Get_cost() < _indi_optimal->Get_cost())
		{
			_indi_optimal = popu[0];
		}

		Crossover();
		Mutation();
		//Fitness();
		SelectOffspring();
	}
	//算法结束

	end_t = time(NULL);

	return OutputData(_indi_optimal, start_t, end_t);
}

void GeneticAlgorithm::InputData(char* data_file_path) {
	vector< vector<string> > vs = ReadFile(data_file_path);	//读取数据

	auto s = (vs.begin())->begin();							//处理数据
	vehicle_capacity = std::stol(*s);							//设置车辆载重

	Chro chro;													//设置染色体
	for (auto v_i = vs.begin() + 1; v_i != vs.end(); ++v_i) {
		if (v_i->size() == 7) {
			_Gene cu = make_shared<Cust>(*v_i);
			chro.push_back(cu);
		}
	}

	_start_vehicle = chro[0];									//设置起始仓库

	chro.erase(chro.begin());									//去掉起点仓库
	gene_count = chro.size();									//设置基因数量
	_Indi _i = make_shared<Individual>(_start_vehicle, &chro, vehicle_capacity);
	GeneOptimization(*_i);											//优化个体基因

	popu.push_back(_i);											//设置初始种群
	_indi_optimal = _i;											//设置最优个体
}

string GeneticAlgorithm::OutputData(_Indi _i, time_t start_t, time_t end_t) {
	// - 路线
	// * 车辆数
	// = 成本
	// : 时间
	string str;

	str = _i->Get_path_string();
	str += "* " + std::to_string(_i->Get_vehicle_count()) + '\n';
	str += "= " + std::to_string(_i->Get_cost()) + '\n';
	str += ": " + OutTime(end_t, start_t) + '\n';

	return str;
}

int GeneticAlgorithm::Selection() const {
	//轮盘赌算法
	double m = 0; //反成本占比
	double r = MyRand(); //随机数

	Popu_const_iter p_i = popu.begin();
	Popu_const_iter p_i_e = popu.begin() + popu_indi_count_now;
	while (p_i != p_i_e)
	{
		m += ((*p_i)->Get_rev_cost() / p_now_rev_cost_sum);
		if (r <= m) return p_i - popu.begin();

		++p_i;
	}
	return 0;
}

void  GeneticAlgorithm::Crossover() {
	const int n = 5 * (gene_count / 10); //最大取n个基因进行交换
	int crossover_count = CalProbabilityCount(pc, 2); //交叉次数
#pragma loop(no_vector)
	while ((--crossover_count) != 0)
	{
		int a = rand() % gene_count;							//染色体a交换的基因的起点位置
		int b = rand() % gene_count;							//染色体b交换的基因的起点位置

		int a_crossover_number = (gene_count - a) < n ? (gene_count - a) : n;	//染色体a的基因交换数量
		int b_crossover_number = (gene_count - b) < n ? (gene_count - b) : n;	//染色体b的基因交换数量

		Chro chro_a = popu[Selection()]->Get_chro();
		Chro chro_b = popu[Selection()]->Get_chro();

		Chro_const_iter a_begin = chro_a.begin() + a;			//染色体a交换的基因的起点
		Chro_const_iter a_end = a_begin + a_crossover_number;	//染色体a交换的基因的终点
		Chro_const_iter b_begin = chro_b.begin() + b;			//染色体b交换的基因的起点
		Chro_const_iter b_end = b_begin + b_crossover_number;	//染色体b交换的基因的终点

		Chro* _chro_new_a = new vector<_Gene>(chro_a); //新染色体a
		Chro* _chro_new_b = new vector<_Gene>(chro_b); //新染色体b

		//把要交换的基因加入新染色体头部
		_chro_new_a->insert(_chro_new_a->begin(), b_begin, b_end);
		_chro_new_b->insert(_chro_new_b->begin(), a_begin, a_end);
		//用新染色体生成新个体
		_Indi _ia = make_shared<Individual>(_start_vehicle, _chro_new_a, vehicle_capacity);
		_Indi _ib = make_shared<Individual>(_start_vehicle, _chro_new_b, vehicle_capacity);
		//去除重复基因
		DeleteRepeatGeneFromBegin(*_ia, a_crossover_number);
		DeleteRepeatGeneFromBegin(*_ib, b_crossover_number);
		//加入下一代种群
		popu.push_back(_ia);
		popu.push_back(_ib);
	}
}

void  GeneticAlgorithm::Mutation() {
	int mutation_count = CalProbabilityCount(pm, 2); //变异次数
#pragma loop(no_vector)
	while ((--mutation_count) != 0)
	{
		int a = rand() % gene_count; //变异的基因a的位置
		int b = rand() % gene_count; //变异的基因b的位置

		Chro* _chro_new = new vector<_Gene>(); // 新的染色体

		*_chro_new = popu[Selection()]->Get_chro();

		Chro_iter g_a = _chro_new->begin() + a; //变异的基因a
		Chro_iter g_b = _chro_new->begin() + b; //变异的基因b

		//交换基因
		_Gene g = *g_a;
		*g_a = *g_b;
		*g_b = g;

		//生成新个体
		_Indi i = make_shared<Individual>(_start_vehicle, _chro_new, vehicle_capacity);
		//加入下一代种群
		popu.push_back(i);
	}
}

//void  GeneticAlgorithm::Fitness() {}

void GeneticAlgorithm::SelectOffspring() {
	//排序种群
	std::sort(popu.begin(), popu.end(), IndiDESC);
	std::sort(popu.begin(), popu.end(), IndiDESC);
	if ((int)popu.size() > max_popu_indi_count) {
		popu.erase(popu.begin() + max_popu_indi_count, popu.end());
	}
}

bool GeneticAlgorithm::IsSire() {
	if (popu[0]->Get_rev_cost() >= _indi_optimal->Get_rev_cost())
	{
		--reproduce_count;
		if (reproduce_count <= 0) { return false; }
		return true;
	}
	reproduce_count = max_reproduce_count;
	return true;
}