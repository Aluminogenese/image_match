#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/core/simd_intrinsics.hpp>

namespace featurextraction
{
    /**
     * @brief Harris算子角点检测
     *
     * @param image 检测图像
     * @param corners 检测得到的角点
     * @param threshold Harris响应值的阈值
     */
    void HarrisCornerDetect(const cv::Mat& image, std::vector<cv::Point>& corners, const int& threshold = 1000);

    /**
     * @brief Moravec角点检测
     *
     * @param image 检测图像
     * @param corners 检测得到的角点
     * @param windowSize 计算兴趣值的窗口大小，默认为9×9
     * @param restrainWinSize 抑制窗口大小，默认为21×21
     * @param threshold 阈值，默认为700
     */
    void MoravecCornerDetect(const cv::Mat& image, std::vector<cv::Point>& corners, const int& interestWinSize = 9, const int& restrainWinSize = 101, const int& threshold = 1000);

    /**
     * @brief SIFT角点检测
     *
     * @param image 检测图像
     * @param corners 检测得到的角点
     */
    void  SIFTCornerDetect(const cv::Mat& image, std::vector<cv::Point>& corners);

    /**
     * @brief 在图像上绘制角点
     *
     * @param srcImg 原图像
     * @param outputImg 绘制角点后图像
     * @param corners 角点
     */
    void DrawCorners(const cv::Mat& srcImg, cv::Mat& outputImg, const std::vector<cv::Point>& corners);
};

