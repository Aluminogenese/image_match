#pragma once

#include <random>

#include <opencv2/opencv.hpp>

#include "match_point_pair.h"

class BaseMatcher
{
public:
    /**
     * @brief 设置匹配窗口大小
     *
     * @param windowSize 窗口大小，如：7、9、11，默认为15
     */

    void setWindowSize(int windowSize);

    /**
     * @brief 设置阈值
     *
     * @param threshold 相关系数阈值，默认为0.8
     */
    void setThreshold(double threshold);

    /**
     * @brief
     *
     * @param srcImg 参考图像
     * @param dstImg 目标图像
     * @param outputImg 输出匹配结果的图像
     * @param matches 匹配上的特征点
     */
    void drawMatches(const cv::Mat& srcImg,
        const cv::Mat& dstImg,
        cv::Mat& outputImg,
        std::vector<MatchPointPair>& matches);

protected:
    int windowSize = 15;    // 窗口大小
    double threshold = 0.8; // 阈值

    /**
     * @brief 计算两个窗口的相关系数
     *
     * @param srcWindow
     * @param dstWindow
     * @return double
     */
    double computeCorrelationIdx(const cv::Mat& srcWindow, const cv::Mat& dstWindow);

    /**
     * @brief 判断当前点是否在边界上
     *
     * @param srcImg
     * @param pt
     * @return true
     * @return false
     */
    bool isVaildPoint(const cv::Mat& srcImg, const cv::Point& pt);

};

