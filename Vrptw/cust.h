#ifndef _cust_h_
#define _cust_h_

#include <string>
#include <vector>

//仓库类
class Cust {
public:
	Cust(const std::vector<std::string>& v);

	//仓库ID
	inline const int Get_id() const { return id; }
	//坐标x
	inline const double Get_x() const { return x; }
	//坐标y
	inline const double Get_y() const { return y; }
	//需求
	inline const long Get_demand() const { return demand; }
	//最早到达时间
	inline const double Get_ready_time() const { return ready_time; }
	//最迟到达时间
	inline const double Get_due_time() const { return due_time; }
	//服务时间
	inline const double Get_service() const { return service_time; }
	//仓库内容字符串
	//  [仓库ID] [坐标X] [坐标Y]
	const std::string Get_cust_string() const;
	//计算到达下一仓库的时间
	double CalTimeToNext(const Cust& cust_next, const double time_now);
	//计算到下一仓库的距离
	double CalDistanceToNext(const Cust& cust_next);

private:
	int id;
	double x;			 //坐标x
	double y;			 //坐标y
	long demand;		 //需求
	double ready_time;	 //最早到达时间
	double due_time;	 //最迟到达时间
	double service_time; //服务时间
};
#endif