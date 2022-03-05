#pragma once

#include <string>

class CControlCarSendData
{
public:
	CControlCarSendData();
	~CControlCarSendData();

public:
	/// @brief 修改控制帧信号函数
	/// @param group 按钮所在的组
	/// @param radioButtonId 按钮的id
	void funChangeControlFrameSignalGroup(const int group,const int radioButtonId);

	/// @brief 连接设备函数
	/// @return false表示连接失败、true表示成功
	bool funConnectDevice();

	/// @brief 断开设备函数
	/// @return false表示连接失败、true表示成功
	bool funCloseDevice();

	// @brief 改变速度百分比函数
	/// @param precent 百分比
	void funChangeSpeedPrecent(int precent);

	/// @brief 改变转向编码器值（0-1180）0表示最左边，1180表示最右边
	/// @param 转向值
	void funChangeSteeringEncoderValue();

	/// @brief 实时检测转向编码器值函数
	/// @param 转向值
	void funCheckSteeringEncoderValue();

private:
	/// @brief 更新控制帧信号
	void funUpdateControlFrameSignal();

	/// @brief 二进制转16进制函数
	void funBinaryToHexadecimal();

	/// @brief 二进制字符串转十进制
	/// @param pbin 所要转的字符串
	/// @return 返回十进制数
	long funBintoDec(const char* pbin);

	/// @brief 向设备发送控制帧信号数据的函数
	/// @param num 发送次数，默认为3次
	void funSendControlDataToDevice(int num = 3);

private:
	std::string _controlFrameSignalGroup[11];  ///< 控制帧信号字符串数组
	std::string _controlFrameSignal;           ///< 控制帧信号的字符串
	std::string _hexControlFrameSignal;        ///< 16进制控制帧信号字符串
};

