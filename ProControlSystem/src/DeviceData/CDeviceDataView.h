#pragma once

#include "qopenglwidget.h"

class CDeviceDataView : public QOpenGLWidget
{
	Q_OBJECT

public:
	CDeviceDataView(QWidget* parent = nullptr);
	virtual ~CDeviceDataView();

protected:
	/// @brief 绘制框架
	virtual void funDrawFrame();

	/// @brief 绘制指针图像
	virtual void funDrawPointer();

	/// @brief 绘制速度函数
	virtual void funDrawSpeed();

	/// @brief 初始化画家函数
	virtual void funInitPainter();

protected:
	QPainter* _painter;           ///< 画家
	QBrush _backgroundColor;      ///< 组件背景颜色
	int _speed;                   ///< 速度
	int _status;                  ///< 状态
	qreal _angle;                 ///< 角度
	QString _infomationText;      ///< 信息文本

	QPoint _point[4] =
	{
		QPoint(0, 10),
		QPoint(-10, 0),
		QPoint(0, -170),
		QPoint(10, 0),
	};
};
