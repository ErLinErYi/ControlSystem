#include "CDeviceDataView.h"
#include "qpainter.h"
#include "qmath.h"

CDeviceDataView::CDeviceDataView(QWidget* parent)
	: QOpenGLWidget(parent),
	_backgroundColor(QBrush(QColor(34, 34, 34))),
	_angle((qreal)270 / (180 - 1)),
	_speed(0),
	_status(0),
	_infomationText("左后轮转速值")
{
	QSurfaceFormat myFmt;
	myFmt.setSamples(8);
	setFormat(myFmt);
}

CDeviceDataView::~CDeviceDataView()
{
}

void CDeviceDataView::funDrawFrame()
{
	_painter->save();

	_painter->setBrush(QBrush(QColor(0, 255, 0, 255), Qt::SolidPattern));
	_painter->drawArc(-200, -200, 400, 400, -135 * 16, -270 * 16);

	_painter->restore();

	for (int i = 0; i < 180; ++i)
	{
		_painter->save();



		_painter->rotate(-225 + i * _angle);

		if (i % 10 == 0)
		{
			_painter->drawLine(180, 0, 200, 0);
		}
		else
		{
			_painter->drawLine(190, 0, 200, 0);
		}

		_painter->restore();
	}

	_painter->save();

	for (int i = 0; i < 9; i++)
	{
		int xTextPos = 180 * qCos((225 - i * 15) * M_PI / 180);
		int yTextPos = -180 * qSin((225 - i * 15) * M_PI / 180);
		_painter->drawText(xTextPos, yTextPos + 10, QString::number(i * 1500));
		_painter->drawText(-xTextPos - 25, yTextPos + 15, QString::number((18 - i) * 1500));
	}
	_painter->drawText(-10, -165, QString::number(9 * 1500));

	_painter->restore();
}

void CDeviceDataView::funDrawPointer()
{
	_painter->save();

	QLinearGradient myLinear;
	myLinear.setStart(-200, -200);
	myLinear.setFinalStop(200, 200);
	myLinear.setColorAt(0, QColor(0, 255, 255, 0));
	myLinear.setColorAt(1, QColor(0, 255, 255, 200));
	_painter->setPen(Qt::NoPen);
	_painter->setBrush(myLinear);
	_painter->drawPie(-200, -200, 400, 400, 225 * 16, -(_angle * _speed / 150) * 16);

	_painter->restore();

	_painter->save();

	_painter->setBrush(QBrush(QColor(0, 0, 0, 255), Qt::SolidPattern));
	_painter->rotate(-135 + _speed / 150 * _angle);
	_painter->drawPolygon(_point, 4);

	_painter->restore();
}

void CDeviceDataView::funDrawSpeed()
{
	_painter->save();

	_painter->setPen(QColor(200,200,200));

	//  绘制速度
	QFont myFont("微软雅黑", 10, QFont::Bold);
	myFont.setBold(true);
	myFont.setPixelSize(40);
	_painter->setFont(myFont);
	_painter->drawText(-60, 100, 120, 92, Qt::AlignCenter, QString::number(_speed));

	QPen pen;
	pen.setColor(QColor(0, 255, 255));
	myFont.setPixelSize(25);
	myFont.setBold(false);
	_painter->setFont(myFont);
	_painter->setPen(pen);
	_painter->drawText(-80, 140, 160, 92, Qt::AlignCenter, _infomationText);

	_painter->restore();

	_painter->end();
}

void CDeviceDataView::funInitPainter()
{
	_painter = new QPainter(this);
	_painter->begin(this);
	_painter->fillRect(this->rect(), _backgroundColor);

	QPen myPen;
	myPen.setColor(QColor(200, 200, 200));
	_painter->setPen(myPen);

	auto myRadarViewWidth = size().width();
	auto myRadarViewHeight = size().height();
	auto myRad = std::min(myRadarViewHeight, myRadarViewWidth);

	_painter->setRenderHint(QPainter::Antialiasing);
	_painter->setRenderHint(QPainter::TextAntialiasing);
	_painter->setViewport(QRect((myRadarViewWidth - myRad) / 2, (myRadarViewHeight - myRad) / 2, myRad, myRad));
	_painter->setWindow(-220, -220, 440, 440);
}