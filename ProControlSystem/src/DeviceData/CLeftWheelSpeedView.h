#pragma once
#include "CDeviceDataView.h"

class QTimer;
class CLeftWheelSpeedView : public CDeviceDataView
{
	Q_OBJECT

public:
	CLeftWheelSpeedView(QWidget* parent = nullptr);
	virtual ~CLeftWheelSpeedView();

	/// @brief 开启定时器函数
	virtual void funStartTimer();

	/// @brief 关闭定时器函数
	virtual void funCloseTimer();

	/// @brief 设置速度值函数
	/// @param value 速度值
	virtual void funSetSpeed(int value);

protected:
	virtual void paintEvent(QPaintEvent*) override;

	/// @brief 设置定时器
	virtual void funSetTimer();

protected:
	QTimer* _timer;
};
