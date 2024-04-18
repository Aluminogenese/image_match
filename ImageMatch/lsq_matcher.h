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
     * @param srcImg �ο�ͼ��
     * @param dstImg Ŀ��ͼ��
     * @param match ����������ƥ���ֵ�����������ƥ����
     * @return true
     * @return false
     */
    bool subPixelMatch(MatchPointPair& match, const cv::Mat& leftImage, const cv::Mat& rightImage, const int& winSize, const float& threshold);
    void match(std::vector<MatchPointPair>& matchPnts, std::vector<MatchPointPair>& corrMatchPnts, cv::Mat& leftImage, const cv::Mat& rightImage, const int& winSize = 5, const float& threshold = 0.95);
};