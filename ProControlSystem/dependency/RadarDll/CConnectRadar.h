#pragma once
#include <string>
#include <vector>

#pragma comment(lib,"ProRplidarDriverDll.lib")

struct scanDataArr {
	int quality;
	float angle;
	float dist;
};

class ILidarDriver;
class _declspec(dllimport) CConnectRadar
{
public:
	static CConnectRadar& funGetInstance();

	/// @brief 连接雷达函数
	/// @param port 端口
	/// @param baudrate 波特率 
	/// @return 返回是否成功，true为成功，false为失败
	bool funOnConnect(std::string port = "\\\\.\\com3", int baudrate = 115200);

	/// @brief 检测是否连接成功
	/// @return 返回是否成功，true为成功，false为失败
	bool funIsConnected();

	/// @brief 检测设备是否健康
	/// @param errorcode 错误代码
	/// @return true为成功，false为失败
	bool funCheckDeviceHealth(int* errorcode = nullptr);

	/// @brief 断开连接
	void funOnDisconnect();

	/// @brief 创建数据
	void funOnCreateData();

	/// @brief 运行雷达
	void funRunRadar();

	/// @brief 暂停雷达
	void funPauseRadar();

	/// @brief 关闭雷达
	void funStopRadar();

	/// @brief 重置雷达
	void funResetRadar();

	/// @brief 设置电机速度
	/// @param precent 控制点击的速度的值 0-800
	void funSetMotorSpeed(int precent);

	/// @brief 获取使用的扫描模式
	/// @return 模式编号
	int funGetUsingScanMode();

	/// @brief 获取电机信息
	/// @param id 信息编号
	/// @return 值
	int funGetMotorInfo(int id);

	/// @brief 获取模式名称
	/// @param id 编号
	/// @return 返回模式名称
	std::string funGetModeStr(int id);

	/// @brief 获取样本持续时间
	/// @return 样本持续时间
	float funGetSampleDuration();

	/// @brief 获取样本扫描速度
	/// @return 扫描速度
	float funGetScanSpeed();

	/// @brief 获取雷达驱动实例指针
	/// @return 
	ILidarDriver* funGetILidarDriver();

	// @brief 获取扫描数据
	/// @param data scanDataArr类型vector数据
	void funGetScanData(std::vector<scanDataArr>& data);

	/// @brief 获取序列号
	/// @return 序列号
	std::string funGetSerialNumber();
};