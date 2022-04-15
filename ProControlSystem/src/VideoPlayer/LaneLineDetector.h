#pragma once
#include <opencv.hpp>

//#define SHOW
//#define MEETING

class LaneLineDetector
{
public:
	LaneLineDetector();
	~LaneLineDetector();

public:
	cv::Mat assignColorChannelWeights(cv::Mat& inputImage); // 重新分配三个通道权重
	cv::Mat perspectiveTransformation(cv::Mat& inputImage); // 将透视图像转换为鸟瞰图
	cv::Mat getImageDeNoise(cv::Mat& inputImage);           // 对输入图像应用高斯模糊去噪
	cv::Mat getImageEdge(cv::Mat& inputImage);              // 获取边缘图像
	cv::Mat getMaskImage(cv::Mat& inputImage);              // 遮罩图像边缘
	cv::Mat drawIdentificationArea(cv::Mat& inputImage);    // 绘制识别区域

	void detectLaneLine(cv::Mat& inputImage);               // 车道线检测
	void drawLaneLine(cv::Mat& inputImage);                 // 绘制识别到的车道线

private:
	void drawPixelDistribution(cv::Mat& inputImage);        // 绘制像素分布图

	std::vector<cv::Point> polyval(const cv::Mat& input, 
		const std::vector<cv::Point>& src, int n);          // 计算多项式值
	cv::Mat polyfit(std::vector<cv::Point>& inPoint, int n);// 曲线拟合
	cv::Mat convertPerspective(bool flag = true);           // 透视图转换（默认true：透视->鸟瞰,false:鸟瞰->透视）

private:
	std::vector<cv::Point> _leftLaneLine;  //左侧车道线
	std::vector<cv::Point> _rightLaneLine; //右侧车道线
	double _distanceFromCenter;            //偏移量
};