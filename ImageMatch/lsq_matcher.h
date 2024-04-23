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
     * @param matchPoint 亚像素匹配结果
     * @param leftImage 左影像
     * @param rightImage 右影像
     * @param winSize 匹配窗口大小，默认为5
     * @return true
     * @return false
     */
    bool subPixelMatch(MatchPointPair& matchPoint, const cv::Mat& leftImage, const cv::Mat& rightImage, const int& winSize);

    /**
     * @brief 左右影像最小二乘匹配
     *
     * @param matchPoints 亚像素匹配结果
     * @param corrMatchPnts 相关系数匹配点
     * @param leftImage 左影像
     * @param rightImage 右影像
     * @param winSize 匹配窗口大小，默认为5
     */
    void match(std::vector<MatchPointPair>& matchPoints, std::vector<MatchPointPair>& corrMatchPnts, cv::Mat& leftImage, const cv::Mat& rightImage, const int& winSize = 5);
};