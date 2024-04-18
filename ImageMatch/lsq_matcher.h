#pragma once

#include <eigen3/Eigen/Dense>
#include "base_matcher.h"
#include "match_point_pair.h"
#include "correlation_matcher.h"

namespace lsqmatch
{
    /**
     * @brief 单点最小二乘匹配
     *
     * @param srcImg 参考图像
     * @param dstImg 目标图像
     * @param match 输入整像素匹配初值，输出亚像素匹配结果
     * @return true
     * @return false
     */
    bool subPixelMatch(MatchPointPair& match, const cv::Mat& leftImage, const cv::Mat& rightImage, const int& winSize, const float& threshold);
    void match(std::vector<MatchPointPair>& matchPnts, std::vector<MatchPointPair>& corrMatchPnts, cv::Mat& leftImage, const cv::Mat& rightImage, const int& winSize = 5, const float& threshold = 0.95);
};