#pragma once
#include "CDeviceDataView.h"
#include "CSteeringValueView.h"

class CSpeedSignalView : public CSteeringValueView
{
	Q_OBJECT

public:
	CSpeedSignalView(QWidget* parent = nullptr);
	virtual ~CSpeedSignalView();

	/// @brief 设置速度值函数
	/// @param value 速度值
	virtual void funSetSpeedValue(int value);

protected:
	virtual void paintEvent(QPaintEvent*) override;

	/// @brief 设置定时器
	virtual void funSetTimer() override;

	/// @brief 绘制框架
	virtual void funDrawFrame() override;

	/// @brief 绘制指针图像
	virtual void funDrawPointer() override;
};
