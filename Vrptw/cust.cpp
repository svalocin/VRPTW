#include <iostream>
#include <string>
#include <vector>
#include "cust.h"

using std::string;
using std::vector;

Cust::Cust(const vector<string>& v) :
id(0), x(0.0), y(0.0), demand(0), ready_time(0.0), due_time(0.0), service_time(0.0) {
	if (v.size() >= 7) {
		vector<string>::const_iterator iter = v.begin();
		id = std::stoi(*iter);
		x = std::stod(*(++iter));
		y = std::stod(*(++iter));
		demand = std::stol(*(++iter));
		ready_time = std::stod(*(++iter));
		due_time = std::stod(*(++iter));
		service_time = std::stod(*(++iter));
	}
	else {
		std::cerr << "数据流格式不对";
	}
}

const std::string Cust::Get_cust_string() const {
	return std::to_string(this->Get_id()) + " "
		+ std::to_string(this->Get_x()) + " "
		+ std::to_string(this->Get_y());
}

double Cust::CalTimeToNext(const Cust& cust_next, const double time_now) {
	const double dist = CalDistanceToNext(cust_next);
	double t_wait = time_now < (this->Get_ready_time()) ? (this->Get_ready_time()) - time_now : 0;
	double t_next = time_now + this->Get_service() + t_wait + dist;
	return t_next;
}

double Cust::CalDistanceToNext(const Cust& cust_next) {
	double xx = std::fabs(this->Get_x() - cust_next.Get_x());
	double yy = std::fabs(this->Get_y() - cust_next.Get_y());
	return sqrt((xx*xx) + (yy*yy));
}
