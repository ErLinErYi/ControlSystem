#pragma once
#include "qtextedit.h"
#include "qcolor.h"

class CLogMessage : public QTextEdit
{
	Q_OBJECT

public:
	CLogMessage(QWidget* parent = nullptr);
	~CLogMessage();

	static void funLogMessage(const QString& msg, QColor color = QColor(255, 255, 255));

protected:
	/// @brief 鼠标进入事件
	/// @param  暂无使用
	void enterEvent(QEvent*) override;

	/// @brief 鼠标离开事件
	/// @param  暂无使用
	void leaveEvent(QEvent*) override;

	/// @brief 创建右键菜单
	void funCreateRightButtonMenuAction();
};
