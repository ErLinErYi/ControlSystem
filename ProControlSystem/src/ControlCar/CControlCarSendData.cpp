#include "CControlCarSendData.h"
#include "CLogMessage.h"
#include "CConnectDevice.h"
#include "CControlCarReceiveData.h"
#include "ProControlSystem.h"

CControlCarSendData::CControlCarSendData():
    _controlFrameSignalGroup{
        {"00"} /* 电机控制器档位信号
                * 00: 无控制
                * 01: 低速档
                * 10: 中速档
                * 11: 高速档
                */,
        {"00"} /* 电机控制器刹车信号
                * 00: 无控制
                * 01: 刹车
                * 10: 无刹车
                * 11: 预留
                */,
        {"00"} /* 电机控制器行车方向信号
                * 00: 无控制
                * 01: 前进
                * 10: 后退
                * 11: 预留
                */,
        {"00"} /* 小车控制模式
                * 00: 无效
                * 01: PC控制
                * 10: 遥控器控制
                * 11: 预留
                */,
        {"0000"} /* 预留 */,
        {"00"} /* 转向电动推杆动作
                * 00: 无控制
                * 01: 收缩(左转)
                * 10: 释放(右转)
                * 11: 无效
                */,
        {"00"} /* 刹车电动推杆动作
                * 00: 无控制
                * 01: 收缩(刹车)
                * 10: 释放
                * 11: 无效
                */,
        {"00000000"} /* 预留 */,
        {"00000000"} /* 电机控制器转速控制DAC值信号百分比
                          * 0-100: 有效范围
                          * 4095: 无控制
                          */,
        {"0000"},
        {"0000000000000000000000000000"} /* 预留 */
    }
{
}

CControlCarSendData::~CControlCarSendData()
{
}

void CControlCarSendData::funChangeControlFrameSignalGroup(const int group, const int radioButtonId)
{
    std::string name[9][4] =
    {
        {"01","10","11"}, // 电机控制器档位信号
        {"00","01","10"}, // 电机控制器刹车信号
        {"00","01","10"}, // 电机控制器行车方向信号
        {"00","01","10"}, // 控制模式
        {""},             // 预留 无效
        {"00","01","10"}, // 转向电动推杆动作
        {"00","01","10"}  // 刹车电动推杆动作
    };

    _controlFrameSignalGroup[group] = name[group][radioButtonId];

    funUpdateControlFrameSignal();
}

bool CControlCarSendData::funConnectDevice()
{
    CConnectDevice::funGetInstance().funCloseDevice();
    if (CConnectDevice::funGetInstance().funOpenDevice())
    {
        CLogMessage::funLogMessage("小车连接成功！");
        return true;
    }
    else
    {
        CLogMessage::funLogMessage("小车连接失败！", Qt::red);
        return false;
    }
}

bool CControlCarSendData::funCloseDevice()
{
    if (CConnectDevice::funGetInstance().funCloseDevice())
    {
        CLogMessage::funLogMessage("断开小车连接成功！");
        return true;
    }
    else
    {
        CLogMessage::funLogMessage("断开小车连接失败！", Qt::red);
        return false;
    }
}

void CControlCarSendData::funChangeSpeedPrecent(int precent)
{
    char bin[10];
    itoa(precent, bin, 2);

    int n = 0;
    for (auto b : bin)
    {
        if (b != 0)
        {
            ++n;
        }
        else
        {
            break;
        }
    }

    std::string str;
    for (int i = 0; i < 8 - n; ++i)
    {
        str += "0";
    }
    str += bin;

    _controlFrameSignalGroup[8] = str;

    funUpdateControlFrameSignal();
}

void CControlCarSendData::funChangeSteeringEncoderValue()
{
    std::string name[] = { "00", "01", "10" };
    auto value = ProControlSystem::funGetViewUi().turnSlider->value();
    auto valueNow = CControlCarReceiveData::funGetInstance().funGetSteeringEncoderValue();

    if (valueNow > value + 20)
    {
        _controlFrameSignalGroup[5] = name[1];
    }
    else if (valueNow < value - 20)
    {
        _controlFrameSignalGroup[5] = name[2];
    }
    else
    {
        _controlFrameSignalGroup[5] = name[0];
    }

    funUpdateControlFrameSignal();
}

void CControlCarSendData::funCheckSteeringEncoderValue()
{
    auto valueNow = CControlCarReceiveData::funGetInstance().funGetSteeringEncoderValue();
    auto value = ProControlSystem::funGetViewUi().turnSlider->value();

    if (value <= valueNow + 50 && value >= valueNow - 50)
    {
        if (_controlFrameSignalGroup[5] != "00")
        {
            _controlFrameSignalGroup[5] = "00";

            funUpdateControlFrameSignal();
        }
    }
    else
    {
        if (!CControlCarReceiveData::funGetInstance().funGetTurnDirectionSignalValue())
        {
            funChangeSteeringEncoderValue();
        }
    }
}

void CControlCarSendData::funUpdateControlFrameSignal()
{
    _controlFrameSignal.clear();
    _hexControlFrameSignal.clear();

    for (auto& str : _controlFrameSignalGroup)
    {
        _controlFrameSignal += str;
    }

    CLogMessage::funLogMessage(("二进制控制帧信号：" + _controlFrameSignal).c_str());

    funBinaryToHexadecimal();
}

void CControlCarSendData::funBinaryToHexadecimal()
{
    int i = 0;
    int sum = 0;
    std::string str;
    do
    {
        str = _controlFrameSignal.substr(i, 4);
        sum = funBintoDec(str.c_str());

        char hex[2];
        itoa(sum, hex, 16);
        _hexControlFrameSignal += hex[0];

        i += 4;

        if (i % 8 == 0 && i < 64)
        {
            _hexControlFrameSignal += " ";
        }
    } while (i < _controlFrameSignal.size());

    CLogMessage::funLogMessage(("16进制控制帧信号：" + _hexControlFrameSignal).c_str());

    funSendControlDataToDevice();
}

long CControlCarSendData::funBintoDec(const char* pbin)
{
    int i = 0;
    long result = 0;

    while (pbin[i] != 0)
    {
        result = result * 2 + (pbin[i] - '0');
        ++i;
    }

    return result;
}

void CControlCarSendData::funSendControlDataToDevice(int num)
{
    for (int i = 0; i < num; ++i)
    {
        CConnectDevice::funGetInstance().funSendData(_hexControlFrameSignal);
    }
}
