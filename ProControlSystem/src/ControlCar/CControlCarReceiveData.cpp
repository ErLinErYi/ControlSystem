#include "CControlCarReceiveData.h"
#include "CConnectDevice.h"
#include "CControlReceiveDeviceData.h"
#include "ProControlSystem.h"

CControlCarReceiveData::CControlCarReceiveData():
	_controlReceiveDeviceData(new CControlReceiveDeviceData()),
	_firstReciveData(true)
{
}

CControlCarReceiveData::~CControlCarReceiveData()
{
	delete _controlReceiveDeviceData;
}

CControlCarReceiveData& CControlCarReceiveData::funGetInstance()
{
	static CControlCarReceiveData myControlCarReceiveData;
	return myControlCarReceiveData;
}

void CControlCarReceiveData::funReciveDeviceData()
{
	auto str = CConnectDevice::funGetInstance().funReceiveDeviceData();
	if (str.len > 2)
	{
		int i = str.len - 1;
		do
		{
			_reciveData = str.receive[i];
			--i;

		} while (!_reciveData.compare("0000000000000000") && i > 0);

		funParsingReceiveDeviceData();
	}
}

int CControlCarReceiveData::funGetTurnDirectionSignalValue()
{
	return funBinStrToTenNumber(_turnToSignal);
}

int CControlCarReceiveData::funGetSteeringEncoderValue()
{
	auto value = funHexStrToTenNumber(_steeringEncoderValue);
	return value > 1200 ? 0 : value;
}

void CControlCarReceiveData::funParsingReceiveDeviceData()
{
	funResetStringValue();

	std::string myLeftValue;
	std::string myRightValue;

	myLeftValue = _reciveData.substr(0, 2);
	myRightValue = _reciveData.substr(2, 2);
	_leftRearWheelSpeed = myRightValue + myLeftValue;

	myLeftValue = _reciveData.substr(4, 2);
	myRightValue = _reciveData.substr(6, 2);
	_rightRearWheelSpeed = myRightValue + myLeftValue;

	myLeftValue = _reciveData.substr(8, 2);
	myRightValue = _reciveData.substr(11, 1);
	_steeringEncoderValue = myRightValue + myLeftValue;

	myLeftValue = funHexStrToBinStr(10, 1);
	_drivingDirection = myLeftValue.substr(3, 1);
	_brakeSignal = myLeftValue.substr(2, 1);
	_gearSignal = myLeftValue.substr(0, 2);

	myLeftValue = funHexStrToBinStr(13, 1);
	_turnToSignal = myLeftValue.substr(0, 2);
	_brakeElectricPushRodignal = myLeftValue.substr(2, 2);

	myLeftValue = _reciveData.substr(12, 1);
	myRightValue = _reciveData.substr(15, 1);
	_speedControlSignal = myRightValue + myLeftValue;

	_controlReceiveDeviceData->funShowLeftRearWheelSpeed(funHexStrToTenNumber(_leftRearWheelSpeed));
	_controlReceiveDeviceData->funShowRightRearWheelSpeed(funHexStrToTenNumber(_rightRearWheelSpeed));
	_controlReceiveDeviceData->funShowSteeringEncoderValue(funHexStrToTenNumber(_steeringEncoderValue));
	_controlReceiveDeviceData->funShowSpeedControlSignalValue(funHexStrToTenNumber(_speedControlSignal));
	_controlReceiveDeviceData->funShowDrivingDirectionText(funBinStrToTenNumber(_drivingDirection));
	_controlReceiveDeviceData->funShowBrakeSignalText(funBinStrToTenNumber(_brakeSignal));
	_controlReceiveDeviceData->funShowGearSignalText(funBinStrToTenNumber(_gearSignal));
	_controlReceiveDeviceData->funShowTurnToSignalText(funBinStrToTenNumber(_turnToSignal));
	_controlReceiveDeviceData->funShowBrakeElectricPushRodignalText(funBinStrToTenNumber(_brakeElectricPushRodignal));

	if (_firstReciveData)
	{
		_firstReciveData = false;
		ProControlSystem::funGetViewUi().turnSlider->setValue(funHexStrToTenNumber(_steeringEncoderValue));
		ProControlSystem::funGetViewUi().deviceSpeedSlider->setValue(funHexStrToTenNumber(_speedControlSignal));
	}

	//controlSysView->_radioButtonGroup[2]->setSelectedButton(funBinStrToTenNumber(_drivingDirection) + 1); // 行车方向选择
	//controlSysView->_radioButtonGroup[1]->setSelectedButton(2 - funBinStrToTenNumber(_brakeSignal)); // 刹车信号选择
	////controlSysView->_radioButtonGroup[0]->setSelectedButton(binToTen(_gearSignal) + 1); // 档位选择
	//controlSysView->_radioButtonGroup[5]->setSelectedButton(funBinStrToTenNumber(_turnToSignal)); // 转向选择
	//controlSysView->_radioButtonGroup[6]->setSelectedButton(funBinStrToTenNumber(_brakeElectricPushRodignal)); // 刹车电推杆选择
	//controlSysView->_slider[0]->setPercent(funHexStrToTenNumber(_speedControlSignal));
}

void CControlCarReceiveData::funResetStringValue()
{
	_leftRearWheelSpeed.clear();
	_rightRearWheelSpeed.clear();
	_steeringEncoderValue.clear();
	_drivingDirection.clear();
	_brakeSignal.clear();
	_gearSignal.clear();
	_turnToSignal.clear();
	_brakeElectricPushRodignal.clear();
	_speedControlSignal.clear();
}

std::string CControlCarReceiveData::funHexStrToBinStr(int begin, int len)
{
	auto myStr = _reciveData.substr(begin, len);
	char bin[5];
	itoa(funHexStrToTenNumber(myStr), bin, 2);
	myStr = bin;
	std::reverse(myStr.begin(), myStr.end());
	for (int i = strlen(bin); i < 4; ++i)
	{
		myStr += "0";
	}
	std::reverse(myStr.begin(), myStr.end());

	return myStr;
}

int CControlCarReceiveData::funHexStrToTenNumber(std::string str)
{
	int myNum = 0;
	sscanf(str.c_str(), "%x", &myNum);
	return myNum;
}

int CControlCarReceiveData::funBinStrToTenNumber(std::string str)
{
	int myNum = 0;
	for (int i = 0; i < str.length(); ++i)
	{
		if (str[i] == '1')
		{
			int j = pow(2, str.length() - i - 1);
			myNum += j;
		}
	}

	return myNum;
}
