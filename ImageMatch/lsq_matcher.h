#pragma once

#include <eigen3/Eigen/Dense>
#include "base_matcher.h"
#include "match_point_pair.h"
#include "correlation_matcher.h"

namespace lsqmatch
{
    /**
     * @brief ������С����ƥ��
     *
     * @param matchPoint ������ƥ����
     * @param leftImage ��Ӱ��
     * @param rightImage ��Ӱ��
     * @param winSize ƥ�䴰�ڴ�С��Ĭ��Ϊ5
     * @return true
     * @return false
     */
    bool subPixelMatch(MatchPointPair& matchPoint, const cv::Mat& leftImage, const cv::Mat& rightImage, const int& winSize);

    /**
     * @brief ����Ӱ����С����ƥ��
     *
     * @param matchPoints ������ƥ����
     * @param corrMatchPnts ���ϵ��ƥ���
     * @param leftImage ��Ӱ��
     * @param rightImage ��Ӱ��
     * @param winSize ƥ�䴰�ڴ�С��Ĭ��Ϊ5
     */
    void match(std::vector<MatchPointPair>& matchPoints, std::vector<MatchPointPair>& corrMatchPnts, cv::Mat& leftImage, const cv::Mat& rightImage, const int& winSize = 5);
};