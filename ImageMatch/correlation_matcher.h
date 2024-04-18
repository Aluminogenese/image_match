#pragma once
#include<opencv2/opencv.hpp>
#include "match_point_pair.h"
namespace correlationmatch {
    /**
     * @brief 由中心点与窗口大小确定匹配目标窗口
     *
     * @param matchWindow 需进行确定的目标窗口
     * @param image 目标图像
     * @param centralPnt 目标窗口中心点
     * @param matchWinSize 目标窗口大小
     */
    void determine_match_window(cv::Mat& matchWindow, const cv::Mat& image, const cv::Point& centralPnt, const int& matchWinSize);

    void normalize_window(cv::Mat& matchWindow);

    void calculte_coefficient(double& cofficent, const cv::Mat& leftWindow, const cv::Mat& rightWindow);

    void match(std::vector<MatchPointPair>& match_points, const cv::Mat& leftImage, const cv::Mat& rightImage, const std::vector<cv::Point>& leftCorners, const int& matchWinSize = 9, const float& threshold = 0.7);


    /**
     * @brief 在参考图像和目标图像上同时提取特征点进行匹配
     *
     * @param srcImg 参考图像
     * @param dstImg 目标图像
     * @param srcPts 参考图像上的特征点
     * @param dstPts 目标图像上的特征点
     * @param matches 输出匹配上的特征点
     */


};

