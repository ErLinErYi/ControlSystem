#pragma once

#include <QtWidgets/QWidget>
#include "ui_ProControlSystem.h"
#include "qevent.h"

class CVideoPlayer;
class ProControlSystem : public QWidget
{
    Q_OBJECT

public:
    ProControlSystem(QWidget *parent = Q_NULLPTR);
    ~ProControlSystem();

    /// @brief 获取界面UI单例
    /// @return 返回ui单例
    static Ui::ControlSystemClass& funGetViewUi();

private:
    /// @brief 读取设置
    void funReadSetting();

    /// @brief 写入设置
    void funWritSetting();
};
