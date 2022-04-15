#pragma once
#include <opencv2/opencv.hpp>
#include "qobject.h"
#include "LaneLineDetector.h"

class ReadFiles:public QObject
{
	Q_OBJECT

public:
	ReadFiles();
	~ReadFiles();

	bool readVideo(const cv::String& path);
	bool readImage(const cv::String& path);
	bool readCamera();
	void closeFile();

private:
	cv::Mat handleFile(cv::Mat& frame);

private:
	bool _isClose;
};