#include "CRightWheelSpeedView.h"
#include "qtimer.h"

CRightWheelSpeedView::CRightWheelSpeedView(QWidget *parent)
	: CLeftWheelSpeedView(parent)
{
	_infomationText = "右后轮转速值";
}

CRightWheelSpeedView::~CRightWheelSpeedView()
{
}

void CRightWheelSpeedView::paintEvent(QPaintEvent*)
{
	funInitPainter();

	funDrawFrame();

	funDrawPointer();

	funDrawSpeed();
}

void CRightWheelSpeedView::funSetTimer()
{
	_timer = new QTimer();
	_timer->start(100);

	connect(_timer, &QTimer::timeout, [=]()
		{
			/*if (_status == 0)
			{
				_speed += 300;
				if (_speed >= 27000)
				{
					_status = 1;
				}
			}
			else
			{
				_speed -= 300;
				if (_speed <= 0)
				{
					_status = 0;
				}
			}*/

			this->update();
		});
}
