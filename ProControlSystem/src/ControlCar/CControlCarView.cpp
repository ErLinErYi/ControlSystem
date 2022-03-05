#include "qaction.h"
#include "qradiobutton.h"
#include "qslider.h"
#include "qtimer.h"

#include "CControlCarView.h"
#include "CLogMessage.h"
#include "CControlCarSendData.h"
#include "CControlCarReceiveData.h"
#include "ProControlSystem.h"
#include "ui_ProControlSystem.h"

CControlCarView::CControlCarView(QWidget *parent):
	QWidget(parent),
	_controlCarSendData(new CControlCarSendData()),
	_timer(nullptr),
	_isConnectSuccessful(false)
{
	funAddMenuAction();
}

CControlCarView::~CControlCarView()
{
	delete _controlCarSendData;
}

void CControlCarView::funAddMenuAction()
{
	auto myConnectCar = new QAction("连接小车", this);
	addAction(myConnectCar);

	auto myClosecar = new QAction("断开连接", this);
	myClosecar->setEnabled(false);
	addAction(myClosecar);

	/*auto myOptionReset = new QAction("选项重置", this);
	myOptionReset->setEnabled(false);
	addAction(myOptionReset);*/

	setContextMenuPolicy(Qt::ActionsContextMenu);

	connect(myConnectCar, &QAction::triggered, this, [=]()
		{
			if (_controlCarSendData->funConnectDevice())
			{
				myConnectCar->setEnabled(false);
				myClosecar->setEnabled(true);
				_isConnectSuccessful = true;

				funSetGroupButtonEnable();
				funGroupRadioButtonControl();
				funChangeSliderNumber();

				/// @brief 开启定时接收返回数据的定时器
				funCreateTimer();
			}
		}, Qt::UniqueConnection);

	connect(myClosecar, &QAction::triggered, this, [=]()
		{
			if (_controlCarSendData->funCloseDevice())
			{
				myConnectCar->setEnabled(true);
				myClosecar->setEnabled(false);
				_isConnectSuccessful = false;

				funSetGroupButtonEnable(false);

				/// @brief 销毁接收返回数据的定时器
				funDestroyTimer();
			}
		}, Qt::UniqueConnection);
}

void CControlCarView::funSetGroupButtonEnable(const bool enable)
{
	ProControlSystem::funGetViewUi().controlWidget->setEnabled(enable);
}

void CControlCarView::funGroupRadioButtonControl()
{
	QRadioButton* myRadioButtonGroup[][3] = { 
		{
			ProControlSystem::funGetViewUi().modeUncontrolled,
			ProControlSystem::funGetViewUi().modeProgramControl,
			ProControlSystem::funGetViewUi().modeRemoteControl
		},
		{
			ProControlSystem::funGetViewUi().directionUncontrol,
			ProControlSystem::funGetViewUi().directionFoward,
			ProControlSystem::funGetViewUi().directionBackward
        },
		{
			ProControlSystem::funGetViewUi().motorBrakeUncontrol,
			ProControlSystem::funGetViewUi().motorBrake,
			ProControlSystem::funGetViewUi().motorBrakeNo
		},
		{
			ProControlSystem::funGetViewUi().gearLow,
			ProControlSystem::funGetViewUi().gearMiddle,
			ProControlSystem::funGetViewUi().gearHigh
		},
		{
			ProControlSystem::funGetViewUi().electricPushBrakeUncontrol,
			ProControlSystem::funGetViewUi().electricPushBrakeShrink,
			ProControlSystem::funGetViewUi().electricPushBrakeRelease
		},
		{
			ProControlSystem::funGetViewUi().turnUncontrol,
			ProControlSystem::funGetViewUi().turnLeft,
			ProControlSystem::funGetViewUi().turnRight
		}
	};

	QString myLogMessage[][3] = {
		{"控制模式->无控制按钮触发",     "控制模式->程序控制按钮触发",  "控制模式->遥控控制按钮触发"},
		{"行车方向->无控制按钮触发",     "行车方向->前进按钮触发",      "行车方向->后退按钮触发"},
		{"电机刹车信号->无控制按钮触发",  "电机刹车信号->刹车按钮触发",  "电机刹车信号->无刹车按钮触发"},
		{"档位信号->低速档按钮触发",     "档位信号->中速档按钮触发",    "档位信号->高速档按钮触发"},
		{"电推杆刹车->无控制按钮触发",   "电推杆刹车->收缩按钮触发",    "电推杆刹车->释放按钮触发"},
		{"转向信号->无控制按钮触发",     "转向信号->左转按钮触发",     "转向信号->右转按钮触发"},
	};

	int mySerialNumber[6] = { 3,2,1,0,6,5 };

	for (int i = 0; i < 6; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			QRadioButton* radioButton = myRadioButtonGroup[i][j];
			connect(radioButton, &QRadioButton::toggled, this, [=](bool isChecked)
				{
					if (isChecked)
					{
						CLogMessage::funLogMessage(myLogMessage[i][j], Qt::yellow);
						_controlCarSendData->funChangeControlFrameSignalGroup(mySerialNumber[i], j);
					}
				}, Qt::UniqueConnection);
		}
	}
}

void CControlCarView::funChangeSliderNumber()
{
	connect(ProControlSystem::funGetViewUi().turnSlider, &QSlider::valueChanged, this, [=](int value)
		{
			ProControlSystem::funGetViewUi().turnSliderNumber->setText(QString().sprintf("%d", value));
			_controlCarSendData->funChangeSteeringEncoderValue();
		}, Qt::UniqueConnection);

	connect(ProControlSystem::funGetViewUi().deviceSpeedSlider, &QSlider::valueChanged, this, [=](int value)
		{
			ProControlSystem::funGetViewUi().deviceSpeedSliderNumber->setText(QString().sprintf("%d %", value));
			_controlCarSendData->funChangeSpeedPrecent(value);
		}, Qt::UniqueConnection);
}

void CControlCarView::funCreateTimer()
{
	_timer = new QTimer(this);
	_timer->start(100);

	connect(_timer, &QTimer::timeout, this, [=]()
		{
			if (_isConnectSuccessful)
			{
				CControlCarReceiveData::funGetInstance().funReciveDeviceData();
			}
		}, Qt::UniqueConnection);

	auto myCheckSteeringEncoderValueTimer = new QTimer(this);
	myCheckSteeringEncoderValueTimer->start(50);
	connect(myCheckSteeringEncoderValueTimer, &QTimer::timeout, this, [=]()
		{
			_controlCarSendData->funCheckSteeringEncoderValue();
		}, Qt::UniqueConnection);

	ProControlSystem::funGetViewUi().leftWheelSpeedView->funStartTimer();
	ProControlSystem::funGetViewUi().rightWheelSpeedView->funStartTimer();
	ProControlSystem::funGetViewUi().steeringValueView->funStartTimer();
	ProControlSystem::funGetViewUi().speedSignalView->funStartTimer();
}

void CControlCarView::funDestroyTimer()
{
	_timer->stop();
	disconnect(_timer, &QTimer::timeout, nullptr, nullptr);

	ProControlSystem::funGetViewUi().leftWheelSpeedView->funCloseTimer();
	ProControlSystem::funGetViewUi().rightWheelSpeedView->funCloseTimer();
	ProControlSystem::funGetViewUi().steeringValueView->funCloseTimer();
	ProControlSystem::funGetViewUi().speedSignalView->funCloseTimer();
}
