#pragma once
#include <string>

#pragma comment(lib,"ProControlCarDll.lib")

/// @brief 用于连接设备的类
class __declspec(dllimport) CConnectDevice
{
public:
	/// @brief 接收数据的结构
	struct CConDataArr
	{
		std::string receive[2500]; ///< string字符串类型
		int len;                   ///< 长度
	};

	/// @brief 获取设备单例
	/// @return 设备单例
	static CConnectDevice& funGetInstance();

	/// @brief 用于打开设备的函数
	/// @param deviceId 设备编号，默认为0
	/// @return 返回true为打开正确，false为打开失败
	bool funOpenDevice(int deviceId = 0); 

	/// @brief 用于关闭设备的函数
	/// @return 返回true为打开正确，false为打开失败
	bool funCloseDevice();

	/// @brief 用于重置设备的函数
	/// @return 返回true为打开正确，false为打开失败
	bool funResetDevice();

	/// @brief 用于接收设备数据的函数
	/// @return 接收到的数据
	CConDataArr funReceiveDeviceData();

	/// @brief 用于发送数据的函数
	/// @param strSendData 所要发送的数据，string类型
	/// @return 返回1表示正常，0表示发送失败，-1表示设备没有打开
	int funSendData(std::string strSendData);
};