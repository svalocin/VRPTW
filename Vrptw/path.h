#ifndef _path_h_
#define _path_h_

#include <string>

//路径接口
class IPath
{
public:
	//成本
	virtual double Get_cost() = 0;

	//输出路径字符串
	// |	表示路线
	// +	表示起点或终点
	virtual std::string Get_path_string() const = 0;

	//路线使用的车辆数目
	virtual int Get_vehicle_count() = 0;
};

#endif
