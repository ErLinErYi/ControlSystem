#pragma once
#include "CDeviceDataView.h"

class QTimer;
class CSteeringValueView : public CDeviceDataView
{
	Q_OBJECT

public:
	CSteeringValueView(QWidget* parent = nullptr);
	virtual ~CSteeringValueView();

	/// @brief 开启定时器函数
	virtual void funStartTimer();

	/// @brief 关闭定时器函数
	virtual void funCloseTimer();

	/// @brief 设置转向编码器值
	/// @param value 
	virtual void funSetSteeringValue(int value);

protected:
	virtual void paintEvent(QPaintEvent*) override;

	/// @brief 设置定时器
	virtual void funSetTimer();

	/// @brief 绘制框架
	virtual void funDrawFrame() override;

	/// @brief 绘制指针图像
	virtual void funDrawPointer() override;

	/// @brief 绘制速度函数
	virtual void funDrawSpeed() override;

protected:
	QTimer* _timer;
};
