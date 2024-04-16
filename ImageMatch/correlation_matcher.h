#pragma once
#include "base_matcher.h"
#include "match_point_pair.h"
class CorrelationMatcher :
    public BaseMatcher
{
public:
    /**
     * @brief 扫描目标图像全图进行同名点匹配
     *
     * @param srcImg 参考图像
     * @param dstImg 目标图像
     * @param srcPts 参考图像上的特征点
     * @param matches 输出匹配上的特征点
     */
    void match(const cv::Mat& srcImg,
        const cv::Mat& dstImg,
        const std::vector<cv::Point>& srcPts,
        std::vector<MatchPointPair>& matches);

    /**
     * @brief 在参考图像和目标图像上同时提取特征点进行匹配
     *
     * @param srcImg 参考图像
     * @param dstImg 目标图像
     * @param srcPts 参考图像上的特征点
     * @param dstPts 目标图像上的特征点
     * @param matches 输出匹配上的特征点
     */
    void matchImproved(const cv::Mat& srcImg,
        const cv::Mat& dstImg,
        const std::vector<cv::Point>& srcPts,
        const std::vector<cv::Point>& dstPts,
        std::vector<MatchPointPair>& matches);

};

