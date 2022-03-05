#include "qpainter.h"
#include "qlabel.h"
#include "qbrush.h"
#include "qtimer.h"
#include "qmath.h"
#include "qaction.h"

#include "CRadarScanView.h"
#include "CControlRadar.h"
#include "ProControlSystem.h"

CRadarScanView::CRadarScanView(QWidget* parent) :
	QOpenGLWidget(parent),
	_timer(nullptr),
	_drawScan(0),
	_isShowLine(false),
	_backgroundColor(QBrush(QColor(34,34,34)))
{
	QSurfaceFormat myFmt;
	myFmt.setSamples(8);
	setFormat(myFmt);

	funAddMenuAction();
}

CRadarScanView::~CRadarScanView()
{
}

void CRadarScanView::funStartTimer()
{
	if (!_timer)
	{
		funSetTimer();
	}
	else
	{
		_timer->start();
	}
}

void CRadarScanView::funCloseTimer()
{
	_timer->stop();

	update();
}

void CRadarScanView::funSetTimer()
{
	_timer = new QTimer(this);
	_timer->start(100);

	connect(_timer, &QTimer::timeout, [=]()
		{
			_drawScan += 5;
			if (_drawScan > 360)_drawScan = 0;

			funSetScanData();

			update();
		});
}

void CRadarScanView::funAddMenuAction()
{
	auto myConnectRadar = new QAction("连接雷达设备", this);
	addAction(myConnectRadar);

	auto myCloseRadar = new QAction("停止雷达设备", this);
	myCloseRadar->setEnabled(false);
	addAction(myCloseRadar);

	auto myResumeRadar = new QAction("恢复雷达设备", this);
	myResumeRadar->setEnabled(false);
	addAction(myResumeRadar);

	auto myResetRadar = new QAction("重启雷达设备", this);
	myResetRadar->setEnabled(false);
	addAction(myResetRadar);

	auto myShowLineAction = new QAction("显示激光线", this);
	addAction(myShowLineAction);
	
	setContextMenuPolicy(Qt::ActionsContextMenu);

	connect(myConnectRadar, &QAction::triggered, [=]() 
		{
			if (CControlRadar::funGetInstance().funStartControlRadar())
			{
				ProControlSystem::funGetViewUi().radar->funStartTimer();

				myConnectRadar->setEnabled(false);
				myCloseRadar->setEnabled(true);
				myResetRadar->setEnabled(true);
			}
		});
	connect(myCloseRadar, &QAction::triggered, [=]() 
		{
			CControlRadar::funGetInstance().funPauseRadar();
			ProControlSystem::funGetViewUi().radar->funCloseTimer();

			myCloseRadar->setEnabled(false);
			myResumeRadar->setEnabled(true);
		});
	connect(myResumeRadar, &QAction::triggered, [=]() 
		{
			CControlRadar::funGetInstance().funRunRadar(); 
			ProControlSystem::funGetViewUi().radar->funStartTimer();

			myCloseRadar->setEnabled(true);
			myResumeRadar->setEnabled(false);
		});
	connect(myResetRadar, &QAction::triggered, [=]() 
		{
			CControlRadar::funGetInstance().funResetRadar(); 
			CControlRadar::funGetInstance().funRunRadar(); 
			ProControlSystem::funGetViewUi().radar->funStartTimer();

			myResumeRadar->setEnabled(false);
		});

	connect(myShowLineAction, &QAction::triggered, [=]()
		{
			funSetShowDataLine(!_isShowLine);

			if (_isShowLine)
			{
				myShowLineAction->setText("关闭激光线");
			}
			else
			{
				myShowLineAction->setText("显示激光线");
			}
		});
}

void CRadarScanView::funSetScanData()
{
	_scanData = CControlRadar::funGetInstance().funGetScanData();
}

void CRadarScanView::funSetShowDataLine(bool isShowLine)
{
	_isShowLine = isShowLine;
}

void CRadarScanView::paintEvent(QPaintEvent*)
{
	funInitPainter();

	funDrawRadarView();

	funDrawScanData();
}

void CRadarScanView::funInitPainter()
{
	_painter = new QPainter(this);
	_painter->begin(this);

	_painter->fillRect(this->rect(), _backgroundColor);

	QPen myPen(QColor(0, 255, 0));
	myPen.setWidth(1);

	_painter->setPen(myPen);
	_painter->setFont(QFont("微软雅黑"));
	_painter->setRenderHint(QPainter::Antialiasing);
	_painter->setRenderHint(QPainter::TextAntialiasing);

	auto myRadarViewWidth = size().width();
	auto myRadarViewHeight = size().height();
	auto myRad = std::min(myRadarViewHeight, myRadarViewWidth);

	_painter->setViewport(QRect((myRadarViewWidth - myRad) / 2, (myRadarViewHeight - myRad) / 2, myRad, myRad));
	_painter->setWindow(-300, -300, 600, 600);
}

void CRadarScanView::funDrawRadarView()
{
	_painter->save();
	QPen myPen(QColor(250, 250, 250));
	myPen.setWidth(2);
	_painter->setPen(myPen);
	_painter->drawEllipse(QPoint(0, 0), 210, 210);
	_painter->restore();

	_painter->save();
	myPen.setColor(QColor(150, 150, 150));
	myPen.setWidth(1);
	_painter->setPen(myPen);
	_painter->drawEllipse(QPoint(0, 0), 140, 140);
	_painter->drawEllipse(QPoint(0, 0), 90, 90);
	_painter->restore();

	_painter->save();
	myPen.setColor(QColor(0, 250, 250, 150));
	myPen.setWidth(1);
	myPen.setStyle(Qt::DotLine);
	_painter->setPen(myPen);
	_painter->drawArc(-190, -190, 380, 380, -210 * 16, -120 * 16);
	_painter->drawArc(-190, -190, 380, 380, -30 * 16, -120 * 16);
	_painter->restore();

	_painter->save();
	myPen.setColor(QColor(0, 230, 230, 100));
	myPen.setWidth(1);
	myPen.setStyle(Qt::SolidLine);
	_painter->setPen(myPen);
	_painter->drawEllipse(QPoint(0, 0), 5, 5);
	_painter->restore();

	_painter->save();
	myPen.setColor(QColor(255, 255, 255));
	myPen.setWidth(1);
	_painter->setPen(myPen);
	_painter->drawText(QPoint(-12, -145), QString::number(14) + "米");
	_painter->drawText(QPoint(-9, -95), QString::number(9) + "米");
	_painter->restore();

	for (int i = 0; i < 180; ++i)
	{
		_painter->save();

		_painter->rotate(i * 2);

		if (i % 5 == 0)
		{
			_painter->save();

			QPen myPen(QColor(0, 255, 0));
			myPen.setWidth(3);
			myPen.setCapStyle(Qt::RoundCap);
			_painter->setPen(myPen);

			_painter->drawLine(0, -225, 0, -240);

			_painter->restore();

			auto str = QString::number(i * 2);
			_painter->drawText(QPoint(-3 * str.size(), -245), str + "°");
		} // if (i % 5 == 0)
		else
		{
			_painter->drawLine(0, -230, 0, -240);
		}

		if (i % 45 == 0)
		{
			_painter->save();
			QString str[4] = { "前","右","后","左" };
			myPen.setColor(QColor(255, 255, 255));
			myPen.setWidth(1);
			_painter->setPen(myPen);
			_painter->drawText(QPoint(-6, -280), str[i / 45]);

			_painter->restore();
		}

		if (i == 25 || i == 65 || i == 115 || i == 155)
		{
			_painter->save();
			myPen.setColor(QColor(0, 100, 100, 100));
			myPen.setWidth(1);
			myPen.setStyle(Qt::DashDotDotLine);
			_painter->setPen(myPen);
			_painter->drawLine(0, 0, 0, -190);
			_painter->restore();
		}

		_painter->restore();
	}// for (int i = 0; i < 180; ++i)

	funDrawScanAnimation();

}

void CRadarScanView::funDrawScanAnimation()
{
	_painter->save();

	QConicalGradient myGradient;
	myGradient.setCenter(0, 0);
	myGradient.setAngle(-_drawScan + 180);
	myGradient.setColorAt(0.5, QColor(0, 255, 0, 200));
	myGradient.setColorAt(0.75, QColor(255, 255, 255, 0));
	_painter->setBrush(QBrush(myGradient));
	_painter->setPen(Qt::NoPen);
	_painter->drawPie(-210, -210, 420, 420, -_drawScan * 16, 90 * 16);

	_painter->restore();
}

void CRadarScanView::funDrawScanData()
{
	for (auto& myDot : _scanData)
	{
		_painter->save();
		_painter->rotate(myDot.angle);
		
		QPen myPen(QColor(255, 0, 0, 100));
		myPen.setWidth(1);
		_painter->setPen(myPen);

		auto myDist = -myDot.dist / 100;
		_painter->drawPoint(QPoint(0, myDist));
		if (_isShowLine)
		{
			_painter->drawLine(QPoint(0, 0), QPoint(0, myDist));
		}
		_painter->restore();
	}

	_painter->end();
}
