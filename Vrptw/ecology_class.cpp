#include <algorithm> 
#include "ecology_class.h"
#include "cust.h"

using std::string;
using std::vector;

Individual::Individual(_Gene _start_vehicle, Chro* _chro, long vehicle_capacity) :
_start_gene(_start_vehicle), _chro(_chro), vehicle_capacity(vehicle_capacity) { 
	SetProperty();
}

Individual::~Individual() { delete _chro; }

string Individual::Get_path_string() const {
	string p("");

	if (cost > -1) {
		string route_logo = "| "; //路线标识
		string start_logo = "+ "; //起点标识

		vector<int> v = path;
		v.push_back(-1); //防止越界

		vector<int>::iterator p_i = v.begin();

		Chro_const_iter c_i = _chro->begin();
		Chro_const_iter c_i_e = _chro->end();

		while (c_i != c_i_e)
		{
			//判断是否为新路径起点
			if ((*c_i)->Get_id() == *p_i) {
				p += start_logo + _start_gene->Get_cust_string() + '\n';
				p += start_logo + (*c_i)->Get_cust_string() + '\n';
				++p_i;
			}
			else {
				p += route_logo + (*c_i)->Get_cust_string() + '\n';
			}
			++c_i;
		}

		p += start_logo + _start_gene->Get_cust_string() + '\n';
	}

	return p;
}

double Individual::CalCost(){
	//初始化条件
	this->path.clear();					//清空路径
	double cost = 0;					//成本
	long capacity_now = 0;				//当前载重量
	double time_now = 0;				//当前使用时间
	_Gene cust_now = this->_start_gene;	//当前仓库
	Chro_iter C_i = this->_chro->begin();
	Chro_const_iter C_i_e = this->_chro->end();
	_Gene cust_next = *C_i;				//下一仓库
	this->path.push_back(cust_next->Get_id());

	//遍历基因
	while (C_i != C_i_e)
	{
		cust_next = *C_i;
		const double dist = cust_now->CalDistanceToNext(*cust_next);

		long capacity_next = capacity_now + cust_next->Get_demand();			//未来载重量
		double time_next = cust_now->CalTimeToNext(*cust_next, time_now);	//未来使用时间


		if (capacity_next <= vehicle_capacity &&
			time_next <= cust_next->Get_due_time() &&
			cust_next->CalTimeToNext(*(this->_start_gene), time_next) <= (this->_start_gene)->Get_due_time()) {
			//能到达下一仓库且能返回起点
			cost += dist;
			capacity_now = capacity_next;
			time_now = time_next;
			cust_now = cust_next;
			++C_i;
		}
		else {
			//开新车，重置条件
			cost += cust_now->CalDistanceToNext(*_start_gene);
			capacity_now = 0;
			time_now = 0;
			cust_now = _start_gene;
			this->path.push_back(cust_next->Get_id());
		}
	}

	return cost;
}

void GeneOptimization(Individual& indi) {
	//排序基因
	Chro chro_copy = *(indi._chro);
	std::sort(chro_copy.begin(), chro_copy.end(), GeneASC);

	Chro* _chro_new = new Chro();

	while (chro_copy.size() != 0)
	{
		//初始化条件
		long capacity_now = 0;				//当前载重量
		double time_now = 0;				//当前使用时间
		_Gene cust_now = indi._start_gene;	//当前仓库
		_Gene cust_next;					//下一仓库

		//遍历基因
		int i = 0;
		Chro_iter C_i = chro_copy.begin() + i;
		while (C_i != chro_copy.end())
		{
			cust_next = *C_i;
			const double dist = cust_now->CalDistanceToNext(*cust_next);

			long capacity_next = capacity_now + cust_next->Get_demand();			//未来载重量
			double time_next = cust_now->CalTimeToNext(*cust_next, time_now);	//未来使用时间

			if (capacity_next <= indi.vehicle_capacity &&
				time_next <= cust_next->Get_due_time() &&
				cust_next->CalTimeToNext(*(indi._start_gene), time_next) <= (indi._start_gene)->Get_due_time()) {
				//能到达下一仓库且能返回起点
				_chro_new->push_back(cust_next);	//把这个基因添加进新的染色体
				chro_copy.erase(C_i);			//把这个基因添从复制染色体删除
				C_i = chro_copy.begin() + i;
				capacity_now = capacity_next;
				time_now = time_next;
			}
			else {
				//不能到达一下仓库，把迭代器指向下一个
				++i;
				C_i = chro_copy.begin() + i;
			}
			if (C_i == chro_copy.end()){
				capacity_now = 0;
				time_now = 0;
			}
		}
	}

	indi._chro = _chro_new;

	indi.SetProperty();
}

void DeleteRepeatGeneFromBegin(Individual& indi, int n) {
	for (int i = 0; i != n; ++i) {
		Chro_const_iter c_c_i = indi._chro->begin() + i;
		Chro_iter c_i = indi._chro->begin() + n;

		while (c_i != indi._chro->end())
		{
			if ((*c_c_i)->Get_id() == (*c_i)->Get_id()) {
				indi._chro->erase(c_i);
				break;
			}
			++c_i;
		}
	}

	indi.SetProperty();
}

bool IndiDESC(const _Indi& _I1, const _Indi& _I2) {
	return _I1->Get_cost() < _I2->Get_cost();
}

bool GeneASC(const _Gene& g1, const _Gene& g2) {
	return g1->Get_due_time() < g2->Get_due_time();
}

double PopuSum(const Popu& P) {
	double sum = 0;
	Popu_const_iter P_i = P.begin();
#pragma loop(no_vector)
	while (P_i != P.end())
	{
		sum += (*P_i)->Get_rev_cost();
		++P_i;
	}
	return sum;
}