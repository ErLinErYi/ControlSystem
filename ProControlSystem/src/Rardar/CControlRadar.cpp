#include "qmessagebox.h"

#include "CControlRadar.h"
#include "CLogMessage.h"

CControlRadar::CControlRadar():
	_isConnect(false)
{
}

CControlRadar::~CControlRadar()
{
}

CControlRadar& CControlRadar::funGetInstance()
{
	static CControlRadar myControlRadar;
	return myControlRadar;
}

void CControlRadar::funOnCreateData()
{
	funCheckDeviceHealth();

	CConnectRadar::funGetInstance().funOnCreateData();
}

void CControlRadar::funPauseRadar()
{
	CConnectRadar::funGetInstance().funPauseRadar();

	CLogMessage::funLogMessage("雷达已暂停！");
}

void CControlRadar::funStopRadar()
{
	CConnectRadar::funGetInstance().funStopRadar();
	CLogMessage::funLogMessage("雷达已停止！");
}

void CControlRadar::funResetRadar()
{
	if (QMessageBox::Cancel == QMessageBox::question(nullptr, "提示", "这个设备将会重新启动。你确定吗？",
		QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Cancel))
	{
		return;
	}
	
	funPauseRadar();
	CConnectRadar::funGetInstance().funResetRadar();

	CLogMessage::funLogMessage("雷达已重新启动！");
}

bool CControlRadar::funStartControlRadar()
{
	if (!_isConnect)
	{
		if (CConnectRadar::funGetInstance().funOnConnect())
		{
			_isConnect = true;
			CLogMessage::funLogMessage("雷达已连接！");

			funOnCreateData();
			funRunRadar();	

			return true;
		}
		else
		{
			CLogMessage::funLogMessage("雷达连接失败！", Qt::red);

			return false;
		}
	}
	return true;
}

void CControlRadar::funRunRadar()
{
	if (_isConnect)
	{
		CConnectRadar::funGetInstance().funRunRadar();

		CLogMessage::funLogMessage("雷达开始运行！");
	}
}

std::vector<scanDataArr>& CControlRadar::funGetScanData()
{
	if (_isConnect)
	{
		CConnectRadar::funGetInstance().funGetScanData(_scanData);
	}

	return _scanData;
}

void CControlRadar::funCheckDeviceHealth()
{
	int errorcode;
	if (!CConnectRadar::funGetInstance().funCheckDeviceHealth(&errorcode)) {
		char msg[200];
		sprintf(msg, "设备运转不正常。你需要重新启动设备。\n错误码: 0x%08x", errorcode);

		QMessageBox::warning(nullptr, "警告", msg);
		CLogMessage::funLogMessage(msg, Qt::red);
	}
}
