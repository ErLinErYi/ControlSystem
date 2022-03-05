#pragma once
#include "CConnectRadar.h"

class CControlRadar
{
public:
	/// @brief 获取类单例函数
	/// @return CControlRadar类单例
	static CControlRadar& funGetInstance();

	/// @brief 运行控制雷达
	void funRunRadar();

	/// @brief 暂停雷达函数
	void funPauseRadar();

	/// @brief 停止雷达函数
	void funStopRadar();

	/// @brief 重置雷达函数
	void funResetRadar();

	/// @brief 开始控制雷达
	bool funStartControlRadar();

	/// @brief 更新扫描数据
	std::vector<scanDataArr>& funGetScanData();

protected:
	CControlRadar();
	~CControlRadar();

	/// @brief 初始化雷达相关操作函数
	void funOnCreateData();

	/// @brief 检测设备函数
	void funCheckDeviceHealth();

protected:
	bool _isConnect;                                      ///< 是否连接
	std::vector<scanDataArr> _scanData;                   ///< 扫描数据
};

