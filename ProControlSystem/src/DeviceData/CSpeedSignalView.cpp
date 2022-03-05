#include "CSpeedSignalView.h"
#include "qpainter.h"
#include "qtimer.h"
#include "qmath.h"

CSpeedSignalView::CSpeedSignalView(QWidget* parent)
{
	_infomationText = "转速信号值";
}

CSpeedSignalView::~CSpeedSignalView()
{
}

void CSpeedSignalView::funSetSpeedValue(int value)
{
	_speed = value;
}

void CSpeedSignalView::paintEvent(QPaintEvent*)
{
	funInitPainter();

	funDrawFrame();

	funDrawPointer();

	funDrawSpeed();
}

void CSpeedSignalView::funSetTimer()
{
	_timer = new QTimer();
	_timer->start(100);

	connect(_timer, &QTimer::timeout, [=]()
		{
			/*if (_status == 0)
			{
				_speed += 5;
				if (_speed >= 180)
				{
					_status = 1;
				}
			}
			else
			{
				_speed -= 5;
				if (_speed <= 0)
				{
					_status = 0;
				}
			}*/

			this->update();
		});
}

void CSpeedSignalView::funDrawFrame()
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
	}//for (int i = 0; i < 180; i++)

	_painter->save();

	for (int i = 0; i < 9; ++i)
	{
		int xTextPos = 180 * qCos((225 - i * 15) * M_PI / 180);
		int yTextPos = -180 * qSin((225 - i * 15) * M_PI / 180);
		_painter->drawText(xTextPos, yTextPos + 10, QString::number(i*10));
		_painter->drawText(-xTextPos - 20, yTextPos + 15, QString::number(((18 - i) * 10)));
	}
	_painter->drawText(-10, -165, QString::number(90));

	_painter->restore();
}

void CSpeedSignalView::funDrawPointer()
{
	_painter->save();

	QLinearGradient myLinear;
	myLinear.setStart(-200, -200);
	myLinear.setFinalStop(200, 200);
	myLinear.setColorAt(0, QColor(255, 86, 8, 0));
	myLinear.setColorAt(1, QColor(255, 86, 8, 200));
	_painter->setPen(Qt::NoPen);
	_painter->setBrush(myLinear);
	_painter->drawPie(-200, -200, 400, 400, 225 * 16, -(_angle * _speed) * 16);

	_painter->restore();

	_painter->save();

	_painter->setBrush(QBrush(QColor(0, 0, 0, 255), Qt::SolidPattern));
	_painter->rotate(-135 + _speed * _angle);
	_painter->drawPolygon(_point, 4);

	_painter->restore();
}