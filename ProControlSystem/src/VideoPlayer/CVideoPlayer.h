#pragma once
#include <opencv.hpp>
#include "qwidget.h"

#define USEOPENCV

class QCamera;
class QMediaPlayer;
class QMediaPlaylist;
class CVideoWidget;
class CVideoPlayer : public QWidget
{
	Q_OBJECT
signals:
	void funCloseCamera();

public:
	CVideoPlayer(QWidget* parent = nullptr);
	~CVideoPlayer();

	/// @brief 播放视频函数
	void funVideoPlay();

	/// @brief 关闭视频函数
	void funVideoClose();

	/// @brief 暂停视频函数
	void funVideoStop();

	/// @brief 恢复视频播放函数
	void funVideoResume();

	/// @brief 设置媒体文件名称
	/// @param name 所要播放的媒体文件
	void funSetMediaFilesName(QString& name);

	/// @brief 检测摄像头图像
	void funCheckCamera();

	/// @brief 播放摄像头函数
	void funCameraPlay();

	/// @brief 暂停摄像头视频播放函数
	void funCameraStop();

	/// @brief 抓取摄像头图像函数
	void funCameraImageCapture();

	/// @brief 接收opencvMat数据
	/// @param dst 
	void funReceiveOpencvMatData(const cv::Mat& dst);

protected:
	void resizeEvent(QResizeEvent* e) override;
	void keyPressEvent(QKeyEvent* event) override;
	void paintEvent(QPaintEvent*) override;

	/// @brief 视频播放初始化函数
	void funVideoInit();

	/// @brief 摄像头播放初始化函数
	void funCameraInit();

	/// @brief 添加菜单动作
	void funAddMenuAction();

	/// @brief 调整控件大小函数
	void funResizeWidget();

private:
	void funSetIsCameraClose(bool close = false);

private:
	QCamera* _cameraPlayer;          ///< 摄像头播放器
	QMediaPlayer* _mediaPlayer;      ///< 媒体播放器
	QMediaPlaylist* _medaiPlayList;  ///< 播放列表
	CVideoWidget* _videoWidget;      ///< 媒体播放窗口
	cv::Mat _dstImage;               ///< opencv处理后返回的图像
	bool _isCameraClose;             ///< 视像头是否关闭
};