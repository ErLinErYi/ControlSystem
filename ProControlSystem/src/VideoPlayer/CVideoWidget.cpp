#include "CVideoWidget.h"
#include "qevent.h"

CVideoWidget::CVideoWidget(QWidget *parent)
	: QVideoWidget(parent)
{
}

CVideoWidget::~CVideoWidget()
{
}

void CVideoWidget::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Escape && isFullScreen()) 
    {
        setFullScreen(false);
        event->accept();
        emit funWidgetResized();
    }
    else if (event->key() == Qt::Key_Enter && event->modifiers() & Qt::Key_Alt) 
    {
        setFullScreen(!isFullScreen());
        event->accept();
        emit funWidgetResized();
    }
    else 
    {
        QVideoWidget::keyPressEvent(event);
    }
}

void CVideoWidget::mouseDoubleClickEvent(QMouseEvent* event)
{
    setFullScreen(!isFullScreen());
    event->accept();
    emit funWidgetResized();
}