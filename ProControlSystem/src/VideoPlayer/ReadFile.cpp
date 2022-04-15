#include "ReadFile.h"
#include <vector>
#include "CVideoPlayer.h"
#include "ProControlSystem.h"

ReadFiles::ReadFiles():_isClose(false)
{
	QObject::connect(ProControlSystem::funGetViewUi().videoWidget, &CVideoPlayer::funCloseCamera, this, &ReadFiles::closeFile);
}

ReadFiles::~ReadFiles()
{
}

bool ReadFiles::readVideo(const cv::String& path)
{
	cv::VideoCapture cap(path);
	if (!cap.isOpened()) return false;

#if CAVE
	VideoWriter writer("video/1-.mp4", cap.get(CAP_PROP_FOCUS), cap.get(CAP_PROP_FPS), 
		Size(cap.get(CAP_PROP_FRAME_WIDTH), cap.get(CAP_PROP_FRAME_HEIGHT)), true);
#endif

	//auto conut = cap.get(cv::CAP_PROP_FRAME_COUNT);

	cv::Mat frame;
	while (!_isClose) {
		if (!cap.read(frame)) break;

		ProControlSystem::funGetViewUi().videoWidget->funReceiveOpencvMatData(handleFile(frame));

		cv::waitKey(16);
#if CAVE
		writer.write(frame);
#endif
	}

#if CAVE
	writer.release();
#endif
	cap.release();           //释放相机捕获对象
	cv::destroyAllWindows(); //关闭所有窗口
	return true;
}

bool ReadFiles::readImage(const cv::String& path)
{
	std::vector<cv::String> filenames;
	cv::Mat srcImg;
	cv::glob(path, filenames);//glob 将路径里的图片读取到vector中

	while (!_isClose)
	{
		for (auto& i : filenames)
		{
			srcImg = cv::imread(i);

			ProControlSystem::funGetViewUi().videoWidget->funReceiveOpencvMatData(handleFile(srcImg));

			cv::waitKey(16);

			if (_isClose)
			{
				break;
			}
		}
	}

	cv::destroyAllWindows(); //关闭所有窗口
	return true;
}

bool ReadFiles::readCamera()
{
	cv::VideoCapture cap(0);
	if (!cap.isOpened()) return false;

	cv::Mat frame;
	while (!_isClose) {
		if (!cap.read(frame)) break;

		ProControlSystem::funGetViewUi().videoWidget->funReceiveOpencvMatData(handleFile(frame));

		cv::waitKey(16);
	}
	cap.release();           //释放相机捕获对象
	cv::destroyAllWindows(); //关闭所有窗口
	return true;
}

void ReadFiles::closeFile()
{
	_isClose = true;
}

cv::Mat ReadFiles::handleFile(cv::Mat& frame)
{
	LaneLineDetector laneLineDetector; 

	// 分配各个通道权重
	auto img_weight = laneLineDetector.assignColorChannelWeights(frame);

	// 检测图像中的边缘
	auto img_edges = laneLineDetector.getImageEdge(img_weight);

#ifdef MEETING
	// 使用高斯滤波器对图像进行去噪
	auto img_denoise = laneLineDetector.getImageDeNoise(img_edges);

	// 车道线检测
	laneLineDetector.detectLaneLine(img_denoise);

	// 绘制车道线
	laneLineDetector.drawLaneLine(img_denoise);
#else
	// 遮罩图像
	auto img_mask = laneLineDetector.getMaskImage(img_edges);

	// 使用高斯滤波器对图像进行去噪
	auto img_denoise = laneLineDetector.getImageDeNoise(img_mask);

	// 透视图变换
	auto img = laneLineDetector.perspectiveTransformation(img_denoise);

	// 车道线检测
	laneLineDetector.detectLaneLine(img);

	// 绘制车道线
	laneLineDetector.drawLaneLine(img);
#endif // MEETING

	// 绘制识别区域
	return laneLineDetector.drawIdentificationArea(frame);
}
