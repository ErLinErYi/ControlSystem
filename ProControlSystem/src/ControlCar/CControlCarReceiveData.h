#pragma once
#include <string>

class CControlCarReceiveData
{
public:
	/// @brief 获取类单例函数
	/// @return CControlCarReceiveData类实例
	static CControlCarReceiveData& funGetInstance();

	/// @brief 获取设备数据
	void funReciveDeviceData();

	/// @brief 获取转向信号值
	/// @return 0表示未转向，1表示左转，2表示右转
	int funGetTurnDirectionSignalValue();

	/// @brief 获取转向编码器值
	/// @return 数值
	int funGetSteeringEncoderValue();

private:
	CControlCarReceiveData();
	~CControlCarReceiveData();

	/// @brief 解析接收到的设备数据
	void funParsingReceiveDeviceData();

	/// @brief 重置字符串值
	void funResetStringValue();

	/// @brief 16进制字符串转二进制字符串
	/// @param begin 开始位置
	/// @param len 长度
	/// @return 2进制字符串
	std::string funHexStrToBinStr(int begin, int len);

	/// @brief 16进制字符串转10进制数
	/// @param str 所要转的字符串
	/// @return 10进制数
	int funHexStrToTenNumber(std::string str);

	/// @brief 2进制字符串转10进制数
	/// @param str 所要转的字符串
	/// @return 10进制数
	int funBinStrToTenNumber(std::string str);

private:
	std::string _reciveData;                        ///< 存储接收到的数据
	std::string _leftRearWheelSpeed;                ///< 左后轮转速
	std::string _rightRearWheelSpeed;               ///< 右后轮转速
	std::string _steeringEncoderValue;              ///< 转向编码器值
	std::string _drivingDirection;                  ///< 行车方向
	std::string _brakeSignal;                       ///< 刹车信号
	std::string _gearSignal;                        ///< 档位信号
	std::string _turnToSignal;                      ///< 转向信号
	std::string _brakeElectricPushRodignal;         ///< 刹车电推杆信号
	std::string _speedControlSignal;                ///< 速度控制信号
	bool _firstReciveData;                          ///< 是否第一次接收数据

	class CControlReceiveDeviceData* _controlReceiveDeviceData;  ///< 控制接收设备数据类实例
};

