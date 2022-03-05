#pragma once
#include "qopenglwidget.h"

struct scanDataArr;
class QPainter;
class QBrush;
class CRadarScanView : public QOpenGLWidget
{
	Q_OBJECT
public:
	CRadarScanView(QWidget* parent = nullptr);
	~CRadarScanView();

	/// @brief 开启动画效果
	void funStartTimer();

	/// @brief 关闭动画效果
	void funCloseTimer();

	/// @brief 显示激光线
	/// @param isShowLine false表示不显示，true表示显示
	void funSetShowDataLine(bool isShowLine = false);

protected:
	virtual void paintEvent(QPaintEvent*) override;

	/// @brief 初始化画家函数
	virtual void funInitPainter();

	/// @brief 绘制雷达视图函数
	virtual void funDrawRadarView();

	/// @brief 绘制扫描数据
	virtual void funDrawScanData();

	/// @brief 绘制扫描动画
	virtual void funDrawScanAnimation();

	/// @brief 开启动画效果
	virtual void funSetTimer();

	/// @brief 设置扫描数据
	virtual void funSetScanData();

private:
	/// @brief 添加右键菜单
	void funAddMenuAction();

protected:
	QPainter* _painter;                          ///< 画家
	QBrush _backgroundColor;                     ///< 组件背景颜色
	QTimer* _timer;                              ///< 定时器
	float _drawScan;                             ///< 扫描动图数值
	bool _isShowLine;                            ///< 是否显示激线
	std::vector<scanDataArr> _scanData;          ///< 雷达扫描数据
};
