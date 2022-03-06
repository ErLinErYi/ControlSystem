#include "CLogMessage.h"
#include "qdatetime.h"
#include "qaction.h"
#include "qfiledialog.h"
#include "qfile.h"
#include "qshortcut.h"
#include "qscrollbar.h"
#include "ProControlSystem.h"

CLogMessage::CLogMessage(QWidget* parent)
	: QTextEdit(parent)
{
	funCreateRightButtonMenuAction();
}

CLogMessage::~CLogMessage()
{
}

void CLogMessage::funLogMessage(const QString& msg, QColor color)
{
	QString myText;
	myText.append(QDateTime::currentDateTime().toString("[yyyy-MM-dd hh:mm:ss]") + " ");
	myText.append(msg);

	ProControlSystem::funGetViewUi().logMessage->setTextColor(color);
	ProControlSystem::funGetViewUi().logMessage->append(myText);
}

void CLogMessage::funCreateRightButtonMenuAction()
{
	QAction* clearAction = new QAction("清空所有内容", this);
	clearAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Delete));
	this->addAction(clearAction);
	connect(clearAction, &QAction::triggered, [=]()
		{
			this->clear();
			funLogMessage("日志清空完成！", QColor(0, 255, 255));
		});

	QAction* selectAction = new QAction("选择所有内容", this);
	selectAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_A));
	this->addAction(selectAction);
	connect(selectAction, &QAction::triggered, [=]()
		{
			this->selectAll();
		});

	QAction* caveAction = new QAction("保存日志", this);
	caveAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
	this->addAction(caveAction);
	connect(caveAction, &QAction::triggered, [=]()
		{
			auto myLogFile = QFileDialog::getSaveFileName(this, "保存日志",
				QDateTime::currentDateTime().toString("yyyy-MM-dd-hh-mm-ss-") + "CSlogMessage.log", "日志文件(*.log);;所有文件(*.*)");
			if (!myLogFile.isEmpty())
			{
				QFile file(myLogFile);
				file.open(QIODevice::WriteOnly);
				file.write(this->toPlainText().toUtf8());
				file.close();

				funLogMessage("日志保存成功！");
			}
			else
			{
				funLogMessage("日志保存失败！", QColor(255, 0, 0));
			}
		});

	setContextMenuPolicy(Qt::ActionsContextMenu);
}

void CLogMessage::enterEvent(QEvent*)
{
	this->verticalScrollBar()->show();
}

void CLogMessage::leaveEvent(QEvent*)
{
	this->verticalScrollBar()->hide();
}