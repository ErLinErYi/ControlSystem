#include "CLeftWheelSpeedView.h"
#include "qpainter.h"
#include "qtimer.h"
#include "qmath.h"

CLeftWheelSpeedView::CLeftWheelSpeedView(QWidget* parent):
	CDeviceDataView(parent),
	_timer(nullptr)
{
}

CLeftWheelSpeedView::~CLeftWheelSpeedView()
{
}

void CLeftWheelSpeedView::funSetSpeed(int value)
{
	_speed = value;
}

void CLeftWheelSpeedView::paintEvent(QPaintEvent*)
{
	funInitPainter();

	funDrawFrame();

	funDrawPointer();

	funDrawSpeed();
}

void CLeftWheelSpeedView::funStartTimer()
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

void CLeftWheelSpeedView::funSetTimer()
{
	_timer = new QTimer();
	_timer->start(100);

	connect(_timer, &QTimer::timeout, [=]()
		{
			/*if (_status == 0)
			{
				_speed += 150;
				if (_speed >= 27000)
				{
					_status = 1;
				}
			}
			else
			{
				_speed -= 150;
				if (_speed <= 0)
				{
					_status = 0;
				}
			}*/

			this->update();
		});
}

void CLeftWheelSpeedView::funCloseTimer()
{
	_speed = 0;
	_status = 0;
	_timer->stop();

	this->update();
}
