#include "ProControlSystem.h"
#include "qpushbutton.h"
#include "qcolor.h"
#include "qdebug.h"
#include "qfiledialog.h"
#include "CVideoPlayer.h"
#include "qmediaplayer.h"
#include "qmediaplaylist.h"
#include "qvideowidget.h"
#include "qaction.h"
#include "qsettings.h"
#include "qdebug.h"

ProControlSystem::ProControlSystem(QWidget* parent) :
	QWidget(parent)
{
	funGetViewUi().setupUi(this);
	//this->setWindowOpacity(0.5f);
	this->setWindowTitle("控制系统");
	//this->setStyleSheet("background:gray");

	/*QPushButton* pushButton = new QPushButton("关闭", this);
	pushButton->move(100, 200);
	pushButton->resize(200, 100);
	pushButton->setStyleSheet("background:rgba(0,255,255,255)");
	this->resize(1280, 720);*/
	
	//connect(funGetViewUi().pushButton_3, &QPushButton::clicked, [=]()
	//	{
	//		funGetViewUi().pushButton_2->setText("改变考虑");
	//		qDebug() << "按钮被点击";
	//		auto file = QFileDialog::getOpenFileNames(this, "标题", QString(), "所有文件(*.*);;MP4(*.mp4);;");

	//		foreach(QString const& in, file)
	//		{
	//		}
	//	});

	//connect(funGetViewUi().pushButton_4, &QPushButton::clicked, [=]()
	//	{
	//		funGetViewUi().radar->funStartTimer();
	//		funGetViewUi().pushButton_4->setEnabled(false);
	//		funGetViewUi().pushButton_5->setEnabled(true);

	//		CLogMessage::funLogMessage("动画按钮触发！");
	//	});

	//connect(funGetViewUi().pushButton_5, &QPushButton::clicked, [=]()
	//	{
	//		/*funGetViewUi().deviceDataView->funCloseTimer();
	//		funGetViewUi().deviceDataView2->funCloseTimer();
	//		funGetViewUi().deviceDataView4->funCloseTimer();
	//		funGetViewUi().deviceDataView3->funCloseTimer();*/
	//		funGetViewUi().radar->funCloseTimer();
	//		funGetViewUi().pushButton_4->setEnabled(true);
	//		funGetViewUi().pushButton_5->setEnabled(false);

	//		CLogMessage::funLogMessage("关闭动画按钮触发！");
	//	});

	//funGetViewUi().pushButton_5->setEnabled(false);

	CLogMessage::funLogMessage("日志开始记录！", QColor(0, 255, 255));
}

ProControlSystem::~ProControlSystem()
{
}

Ui::ControlSystemClass& ProControlSystem::funGetViewUi()
{
	static Ui::ControlSystemClass myUi;
	return myUi;
}

void ProControlSystem::funReadSetting()
{
	QSettings settings("LZ", "ControlSystem");
	settings.beginGroup("mainWindow");
	restoreGeometry(settings.value("geometry").toByteArray());
	move(settings.value("pos").toPoint());
	resize(settings.value("size").toSize());
	settings.endGroup();
}

void ProControlSystem::funWritSetting()
{
	QSettings settings("LZ", "ControlSystem");
	settings.beginGroup("mainWindow");
	settings.setValue("pos", pos());
	settings.setValue("size", size());
	settings.setValue("geometry", saveGeometry());
	settings.endGroup();
}
