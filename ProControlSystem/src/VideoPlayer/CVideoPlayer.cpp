#include "CVideoPlayer.h"
#include "qmediaplayer.h"
#include "qmediaplaylist.h"
#include "qvideowidget.h"
#include "qcameraimagecapture.h"
#include "qcamera.h"
#include "qcamerainfo.h"
#include "qcameraviewfinder.h"
#include "qaction.h"
#include "qfiledialog.h"
#include "qdesktopwidget.h"
#include "qpainter.h"
#include "CLogMessage.h"
#include "CVideoWidget.h"
#include "ProControlSystem.h"
#include "ReadFile.h"

CVideoPlayer::CVideoPlayer(QWidget* parent) :
	_mediaPlayer(nullptr),
	_medaiPlayList(nullptr),
	_cameraPlayer(nullptr),
	_videoWidget(new CVideoWidget(this)),
	_isCameraClose(true),
	QWidget(parent)
{
	funAddMenuAction();
}

CVideoPlayer::~CVideoPlayer()
{
}

void CVideoPlayer::funReceiveOpencvMatData(const cv::Mat& dst)
{
	_dstImage = dst;
	this->update();
}

void CVideoPlayer::resizeEvent(QResizeEvent* e)
{
	_videoWidget->resize(this->size());
}

void CVideoPlayer::keyPressEvent(QKeyEvent* event)
{
	if (_mediaPlayer && event->key() == Qt::Key_Right)
	{
		_mediaPlayer->setPosition(_mediaPlayer->position() + 5000);
	}
	else if (_mediaPlayer && event->key() == Qt::Key_Left)
	{
		_mediaPlayer->setPosition(_mediaPlayer->position() - 5000);
	}
	else
	{
		QWidget::keyPressEvent(event);
	}
}

void CVideoPlayer::paintEvent(QPaintEvent*)
{
	QPainter painter(this);
	painter.begin(this);
	painter.setRenderHint(QPainter::Antialiasing);

	auto myRadarViewWidth = size().width();
	auto myRadarViewHeight = size().height();
	auto myRad = std::min(myRadarViewHeight, myRadarViewWidth);

	painter.setViewport(QRect((myRadarViewWidth - myRad) / 2, (myRadarViewHeight - myRad) / 2, myRad, myRad));
	painter.setWindow(-200, -200, 400, 400);

	if (_isCameraClose)
	{
		painter.fillRect(this->rect(), QBrush(QColor(34, 34, 34)));
	}
	else
	{
		QImage image = QImage((uchar*)(_dstImage.data), _dstImage.cols, _dstImage.rows, QImage::Format_BGR888);
		painter.drawImage(-QPoint(image.width() / 2, image.height() / 2), image);
	}

	painter.end();
}

void CVideoPlayer::funVideoInit()
{
	if (!_mediaPlayer)_mediaPlayer = new QMediaPlayer(this);
	if (!_medaiPlayList)_medaiPlayList = new QMediaPlaylist(this);
}

void CVideoPlayer::funVideoPlay()
{
#ifndef USEOPENCV
	_mediaPlayer->play();
	_mediaPlayer->setVolume(0);
	_videoWidget->show();
	_videoWidget->resize(this->size());
	CLogMessage::funLogMessage("视频文件开始播放！");
#endif // !USEOPENCV
}

void CVideoPlayer::funVideoClose()
{
#ifndef USEOPENCV
	_mediaPlayer->stop();
	_medaiPlayList->clear();
	_videoWidget->close();
#else
	emit funCloseCamera();
	funSetIsCameraClose(true);
#endif // !USEOPENCV
	ProControlSystem::funGetViewUi().videoPlayerLabel->show();
	CLogMessage::funLogMessage("视频文件已关闭！");
}

void CVideoPlayer::funVideoStop()
{
#ifndef USEOPENCV
	_mediaPlayer->pause();

	CLogMessage::funLogMessage("视频文件已暂停！");
#endif
}

void CVideoPlayer::funVideoResume()
{
#ifndef USEOPENCV
	_mediaPlayer->play();

	CLogMessage::funLogMessage("视频文件恢复播放！");
#endif
}

void CVideoPlayer::funSetMediaFilesName(QString& name)
{
	CLogMessage::funLogMessage(name + " 视频文件加载成功！");
	ProControlSystem::funGetViewUi().videoPlayerLabel->hide();
#ifndef USEOPENCV
	funVideoInit();

	_medaiPlayList->clear();
	_medaiPlayList->addMedia(QUrl(name));
	_medaiPlayList->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
	_mediaPlayer->setPlaylist(_medaiPlayList);
	_mediaPlayer->setVideoOutput(_videoWidget);
#else
	funSetIsCameraClose(false);
	ReadFiles file;
	auto right = name.lastIndexOf('.');
	auto left = name.lastIndexOf('/');
	file.readImage(name.replace(left + 1, right - left - 1, "*").toStdString());
#endif
}

void CVideoPlayer::funCheckCamera()
{
	QList<QCameraInfo> myCameras = QCameraInfo::availableCameras();
	if (myCameras.size() > 0)
	{
		CLogMessage::funLogMessage(QString::asprintf("检测到 %d 个可用的摄像头！", myCameras.size()));

		funCameraInit();
	}
	else
	{
		CLogMessage::funLogMessage("没有检测到可用的摄像头！", QColor(255, 0, 0));
	}
}

void CVideoPlayer::funCameraInit()
{
#ifndef USEOPENCV
	auto myCameraInfo = QCameraInfo::defaultCamera();
	if (!_cameraPlayer) _cameraPlayer = new QCamera(myCameraInfo, this);
	_cameraPlayer->setViewfinder(_videoWidget);
#endif // USEOPENCV
	ProControlSystem::funGetViewUi().videoPlayerLabel->hide();
	CLogMessage::funLogMessage("默认摄像头已打开！");
}

void CVideoPlayer::funCameraPlay()
{
	CLogMessage::funLogMessage("摄像头画面已显示！");
#ifdef USEOPENCV
	funSetIsCameraClose(false);
	ReadFiles file;
	file.readCamera();
#else
	_videoWidget->show();
	_videoWidget->resize(this->size());
	_cameraPlayer->start();
#endif
}

void CVideoPlayer::funCameraStop()
{
#ifdef USEOPENCV
	emit funCloseCamera();
	funSetIsCameraClose(true);
#else
	_cameraPlayer->stop();
	_videoWidget->close();
#endif // USEOPENCV
	ProControlSystem::funGetViewUi().videoPlayerLabel->show();

	CLogMessage::funLogMessage("摄像头已关闭！");
}

void CVideoPlayer::funCameraImageCapture()
{
	auto myImageCapture = new QCameraImageCapture(_cameraPlayer, this);
	myImageCapture->setBufferFormat(QVideoFrame::Format_Jpeg);
	myImageCapture->setCaptureDestination(QCameraImageCapture::CaptureToFile);
}

void CVideoPlayer::funAddMenuAction()
{
	auto myOpenVideo = new QAction("打开视频", this);
	myOpenVideo->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
	addAction(myOpenVideo);

	auto myCloseVideo = new QAction("关闭视频", this);
	myCloseVideo->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_D));
	myCloseVideo->setEnabled(false);
	addAction(myCloseVideo);

	auto myPlayVideo = new QAction("播放视频", this);
	myPlayVideo->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_R));
	myPlayVideo->setEnabled(false);
	addAction(myPlayVideo);

	auto myStopVideo = new QAction("暂停视频", this);
	myStopVideo->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_P));
	myStopVideo->setEnabled(false);
	addAction(myStopVideo);

	auto myFullScreen = new QAction("视频全屏", this);
	myFullScreen->setEnabled(false);
	addAction(myFullScreen);

	auto mySeparator = new QAction(this);
	mySeparator->setSeparator(true);
	addAction(mySeparator);

	auto myOpenCamera = new QAction("打开摄像头", this);
	//myOpenCamera->setEnabled(false);
	addAction(myOpenCamera);

	auto myCloseCamera = new QAction("关闭摄像头", this);
	myCloseCamera->setEnabled(false);
	addAction(myCloseCamera);

	setContextMenuPolicy(Qt::ActionsContextMenu);

	/// @brief 打开视频
	connect(myOpenVideo, &QAction::triggered, [=]()
		{
			auto myFile = QFileDialog::getOpenFileName(this, "标题", QString(), "MP4(*.mp4);;所有文件(*.*)");
			
			if (!myFile.isEmpty())
			{
				myCloseVideo->setEnabled(true);
				myStopVideo->setEnabled(true);
				myOpenVideo->setEnabled(false);
				myFullScreen->setEnabled(true);

				CLogMessage::funLogMessage(myFile + " 文件打开成功！");

				funSetMediaFilesName(myFile);
				funVideoPlay();
			}
			else
			{
				CLogMessage::funLogMessage("文件打开失败！", QColor(255, 0, 0));
			}
		});

	/// @brief 关闭视频
	connect(myCloseVideo, &QAction::triggered, [=]()
		{
			funVideoClose();

			myOpenVideo->setEnabled(true);
			myCloseVideo->setEnabled(false);
			myStopVideo->setEnabled(false);
			myPlayVideo->setEnabled(false);
			myFullScreen->setEnabled(false);
		});

	/// @brief 播放视频
	connect(myPlayVideo, &QAction::triggered, [=]()
		{
			funVideoResume();

			myPlayVideo->setEnabled(false);
			myStopVideo->setEnabled(true);
			myOpenVideo->setEnabled(false);
			myCloseVideo->setEnabled(true);
		});

	/// @brief 暂停视频
	connect(myStopVideo, &QAction::triggered, [=]()
		{
			funVideoStop();

			myOpenVideo->setEnabled(false);
			myStopVideo->setEnabled(false);
			myPlayVideo->setEnabled(true);
			myCloseVideo->setEnabled(true);
		});

	/// @brief 视频全屏
	connect(myFullScreen, &QAction::triggered, [=]()
		{
			_videoWidget->setFullScreen(!_videoWidget->isFullScreen());
			funResizeWidget();
		});

	/// @brief 打开摄像头
	connect(myOpenCamera, &QAction::triggered, [=]()
		{
			myOpenCamera->setEnabled(false);
			myCloseCamera->setEnabled(true);

			funCheckCamera();
			funCameraPlay();
		});

	/// @brief 关闭摄像头
	connect(myCloseCamera, &QAction::triggered, [=]()
		{
			myOpenCamera->setEnabled(true);
			myCloseCamera->setEnabled(false);

			funCameraStop();
		});

	connect(_videoWidget, &CVideoWidget::funWidgetResized, [=]()
		{
			funResizeWidget();
		});
}

void CVideoPlayer::funResizeWidget()
{
#ifndef USEOPENCV
	if (!_videoWidget->isFullScreen())
	{
		_videoWidget->resize(this->size());
	}
#endif
}

void CVideoPlayer::funSetIsCameraClose(bool close)
{
	_isCameraClose = close;
}
