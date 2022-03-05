#pragma once

#include <QWidget>

class CControlCarView : public QWidget
{
	Q_OBJECT

public:
	CControlCarView(QWidget* parent = nullptr);
	~CControlCarView();

private:
	/// @brief 添加右键菜单
	void funAddMenuAction();

	/// @brief 设置按钮组是否可用
	/// @param enable 默认可用，true为可用，flase为不可用
	void funSetGroupButtonEnable(const bool enable = true);

	/// @brief 单选按钮组控制函数
	void funGroupRadioButtonControl();

	/// @brief 修改滑动条数值函数
	void funChangeSliderNumber();

	/// @brief 创建定时器
	void funCreateTimer();

	/// @brief 销毁定时器
	void funDestroyTimer();

private:
	class CControlCarSendData* _controlCarSendData;         ///< 发送控制数据类实例指针
	class QTimer* _timer;                                   ///< 定时器实例指针

	bool _isConnectSuccessful;                              ///< 判断连接是否成功
};
