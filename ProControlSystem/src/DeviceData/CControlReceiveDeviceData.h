#pragma once
//#include <string>

class CControlReceiveDeviceData
{
public:
	CControlReceiveDeviceData();
	~CControlReceiveDeviceData();

public:
	/// @brief 显示左后轮速度值函数
	/// @param value 速度值
	void funShowLeftRearWheelSpeed(int value);

	/// @brief 显示右后轮速度值函数
	/// @param value 速度值
	void funShowRightRearWheelSpeed(int value);

	/// @brief 显示转向编码器值函数
	void funShowSteeringEncoderValue(int value);

	/// @brief 显示速度控制描述函数
	void funShowSpeedControlSignalValue(int value);

	/// @brief 显示移动方向函数
	void funShowDrivingDirectionText(int value);

	/// @brief 显示刹车信号函数
	void funShowBrakeSignalText(int value);

	/// @brief 显示档位信号函数
	void funShowGearSignalText(int value);

	/// @brief 显示转向信号函数
	void funShowTurnToSignalText(int value);

	/// @brief 显示刹车信号函数
	void funShowBrakeElectricPushRodignalText(int value);
};

