#pragma once

#include "CLeftWheelSpeedView.h"

class CRightWheelSpeedView : public CLeftWheelSpeedView
{
	Q_OBJECT

public:
	CRightWheelSpeedView(QWidget* parent = nullptr);
	virtual ~CRightWheelSpeedView();

protected:
	virtual void paintEvent(QPaintEvent*) override;

	/// @brief 设置定时器
	virtual void funSetTimer() override;
};
