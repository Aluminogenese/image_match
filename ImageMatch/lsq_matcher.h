#pragma once

//#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Dense>
#include "base_matcher.h"
#include "match_point_pair.h"
class LsqMatcher :
    public BaseMatcher
{
public:
    /**
     * @brief 单点最小二乘匹配
     *
     * @param srcImg 参考图像
     * @param dstImg 目标图像
     * @param match 输入整像素匹配初值，输出亚像素匹配结果
     * @return true
     * @return false
     */
    bool subPixelMatch(const cv::Mat& srcImg, const cv::Mat& dstImg, MatchPointPair& match);

private:
    double a0 = 0, a1 = 1, a2 = 0;
    double b0 = 0, b1 = 0, b2 = 0;
    double h0 = 0, h1 = 1;

};

