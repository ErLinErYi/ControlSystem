#include "CControlReceiveDeviceData.h"
#include "ProControlSystem.h"

CControlReceiveDeviceData::CControlReceiveDeviceData()
{
}

CControlReceiveDeviceData::~CControlReceiveDeviceData()
{
}

void CControlReceiveDeviceData::funShowLeftRearWheelSpeed(int value)
{
	ProControlSystem::funGetViewUi().leftWheelSpeedView->funSetSpeed(value);
}

void CControlReceiveDeviceData::funShowRightRearWheelSpeed(int value)
{
	ProControlSystem::funGetViewUi().rightWheelSpeedView->funSetSpeed(value);
}

void CControlReceiveDeviceData::funShowSpeedControlSignalValue(int value)
{
	ProControlSystem::funGetViewUi().speedSignalView->funSetSpeedValue(value);
}

void CControlReceiveDeviceData::funShowSteeringEncoderValue(int value)
{
	ProControlSystem::funGetViewUi().steeringValueView->funSetSteeringValue(value);
}

void CControlReceiveDeviceData::funShowDrivingDirectionText(int value)
{
	ProControlSystem::funGetViewUi().directionValue->setText(value ? "后退" : "前进");
}

void CControlReceiveDeviceData::funShowBrakeSignalText(int value)
{
	ProControlSystem::funGetViewUi().brakeValue->setText(value ? "有效" : "无效");
}

void CControlReceiveDeviceData::funShowGearSignalText(int value)
{
	QString dw[3] = { "低速","中速","高速" };
	ProControlSystem::funGetViewUi().gearValue->setText(dw[value]);
}

void CControlReceiveDeviceData::funShowTurnToSignalText(int value)
{
	QString dw[3] = { "无动作","左转","右转" };
	ProControlSystem::funGetViewUi().turnValue->setText(dw[value]);
}

void CControlReceiveDeviceData::funShowBrakeElectricPushRodignalText(int value)
{
	QString dw[3] = { "无动作","收缩（刹车）","释放" };
	ProControlSystem::funGetViewUi().brakeElectricValue->setText(dw[value]);
}
