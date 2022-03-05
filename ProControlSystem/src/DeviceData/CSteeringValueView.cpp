#include "CSteeringValueView.h"
#include "qpainter.h"
#include "qmath.h"
#include "qtimer.h"

CSteeringValueView::CSteeringValueView(QWidget *parent): 
	CDeviceDataView(parent),
	_timer(nullptr)
{
	_infomationText = "转向编码器值";
}

CSteeringValueView::~CSteeringValueView()
{
}

void CSteeringValueView::funSetSteeringValue(int value)
{
	_speed = value;
}

void CSteeringValueView::paintEvent(QPaintEvent*)
{
	funInitPainter();

	funDrawFrame();

	funDrawPointer();

	funDrawSpeed();
}

void CSteeringValueView::funStartTimer()
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

void CSteeringValueView::funSetTimer()
{
	_timer = new QTimer();
	_timer->start(100);

	connect(_timer, &QTimer::timeout, [=]()
		{
			/*if (_status == 0)
			{
				_speed += 21;
				if (_speed >= 1260)
				{
					_status = 1;
				}
			}
			else
			{
				_speed -= 21;
				if (_speed <= 0)
				{
					_status = 0;
				}
			}*/

			this->update();
		});
}

void CSteeringValueView::funCloseTimer()
{
	_speed = 0;
	_status = 0;

	_timer->stop();
	this->update();
}

void CSteeringValueView::funDrawFrame()
{
	_painter->save();

	_painter->setBrush(QBrush(QColor(0, 255, 0, 255), Qt::SolidPattern));
	_painter->drawArc(-200, -200, 400, 400, -135 * 16, -270 * 16);

	_painter->restore();

	for (int i = 0; i < 180; i++)
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
		_painter->drawText(xTextPos, yTextPos + 10, QString::number(i * 70));
		_painter->drawText(-xTextPos - 20, yTextPos + 15, QString::number((18 - i) * 70));
	}
	_painter->drawText(-10, -165, QString::number(9 * 70));

	_painter->restore();
}

void CSteeringValueView::funDrawPointer()
{
	_painter->save();

	QLinearGradient myLinear;
	myLinear.setStart(-200, -200);
	myLinear.setFinalStop(200, 200);
	myLinear.setColorAt(0, QColor(255, 86, 8, 0));
	myLinear.setColorAt(1, QColor(255, 86, 8, 200));
	_painter->setPen(Qt::NoPen);
	_painter->setBrush(myLinear);
	_painter->drawPie(-200, -200, 400, 400, 225 * 16, -(_angle * _speed / 7) * 16);

	_painter->restore();

	_painter->save();

	_painter->setBrush(QBrush(QColor(0, 0, 0, 255), Qt::SolidPattern));
	_painter->rotate(-135 + _speed / 7 * _angle);
	_painter->drawPolygon(_point, 4);

	_painter->restore();
}

void CSteeringValueView::funDrawSpeed()
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
	pen.setColor(QColor(255, 86, 8));
	myFont.setPixelSize(25);
	myFont.setBold(false);
	_painter->setFont(myFont);
	_painter->setPen(pen);
	_painter->drawText(-80, 140, 160, 92, Qt::AlignCenter, _infomationText);

	_painter->restore();
}
