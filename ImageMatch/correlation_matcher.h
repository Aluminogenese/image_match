#pragma once
#include<opencv2/opencv.hpp>
#include<opencv2/core/simd_intrinsics.hpp>
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

    /**
     * @brief 归一化匹配窗口
     *
     * @param matchWindow 需进行操作的匹配窗口
     */
    void normalize_window(cv::Mat& matchWindow);

    /**
     * @brief 计算归一化相关系数NCC
     *
     * @param cofficent 计算得到的NCC
     * @param leftWindow 左匹配窗口
     * @param rightWindow 右匹配窗口
     */
    void calculte_coefficient(double& cofficent, const cv::Mat& leftWindow, const cv::Mat& rightWindow);

    /**
     * @brief 左右影像相关系数匹配
     *
     * @param matchPoints 匹配得到的同名点
     * @param leftImage 左影像
     * @param rightImage 右影像
     * @param leftCorners 左影像上提取的特征点
     * @param matchWinSize 匹配窗口大小，默认为9
     * @param threshold 相关系数匹配的阈值
     */
    void match(std::vector<MatchPointPair>& matchPoints, const cv::Mat& leftImage, const cv::Mat& rightImage, const std::vector<cv::Point>& leftCorners, const int& matchWinSize = 9, const int&searWinSize=101, const float& threshold = 0.7);
};

