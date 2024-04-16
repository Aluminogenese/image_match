#pragma once
#include "base_matcher.h"
#include "match_point_pair.h"
class CorrelationMatcher :
    public BaseMatcher
{
public:
    /**
     * @brief ɨ��Ŀ��ͼ��ȫͼ����ͬ����ƥ��
     *
     * @param srcImg �ο�ͼ��
     * @param dstImg Ŀ��ͼ��
     * @param srcPts �ο�ͼ���ϵ�������
     * @param matches ���ƥ���ϵ�������
     */
    void match(const cv::Mat& srcImg,
        const cv::Mat& dstImg,
        const std::vector<cv::Point>& srcPts,
        std::vector<MatchPointPair>& matches);

    /**
     * @brief �ڲο�ͼ���Ŀ��ͼ����ͬʱ��ȡ���������ƥ��
     *
     * @param srcImg �ο�ͼ��
     * @param dstImg Ŀ��ͼ��
     * @param srcPts �ο�ͼ���ϵ�������
     * @param dstPts Ŀ��ͼ���ϵ�������
     * @param matches ���ƥ���ϵ�������
     */
    void matchImproved(const cv::Mat& srcImg,
        const cv::Mat& dstImg,
        const std::vector<cv::Point>& srcPts,
        const std::vector<cv::Point>& dstPts,
        std::vector<MatchPointPair>& matches);

};

