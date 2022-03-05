#pragma once

#include "qvideowidget.h"

class CVideoWidget : public QVideoWidget
{
	Q_OBJECT

signals:
	/// @brief 控件大小改变
	void funWidgetResized();

public:
	CVideoWidget(QWidget* parent = nullptr);
	~CVideoWidget();

protected:
	void keyPressEvent(QKeyEvent* event) override;
	void mouseDoubleClickEvent(QMouseEvent* event) override;
};
