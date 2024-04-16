#pragma once

#include <random>

#include <opencv2/opencv.hpp>

#include "match_point_pair.h"

class BaseMatcher
{
public:
    /**
     * @brief ����ƥ�䴰�ڴ�С
     *
     * @param windowSize ���ڴ�С���磺7��9��11��Ĭ��Ϊ15
     */

    void setWindowSize(int windowSize);

    /**
     * @brief ������ֵ
     *
     * @param threshold ���ϵ����ֵ��Ĭ��Ϊ0.8
     */
    void setThreshold(double threshold);

    /**
     * @brief
     *
     * @param srcImg �ο�ͼ��
     * @param dstImg Ŀ��ͼ��
     * @param outputImg ���ƥ������ͼ��
     * @param matches ƥ���ϵ�������
     */
    void drawMatches(const cv::Mat& srcImg,
        const cv::Mat& dstImg,
        cv::Mat& outputImg,
        std::vector<MatchPointPair>& matches);

protected:
    int windowSize = 15;    // ���ڴ�С
    double threshold = 0.8; // ��ֵ

    /**
     * @brief �����������ڵ����ϵ��
     *
     * @param srcWindow
     * @param dstWindow
     * @return double
     */
    double computeCorrelationIdx(const cv::Mat& srcWindow, const cv::Mat& dstWindow);

    /**
     * @brief �жϵ�ǰ���Ƿ��ڱ߽���
     *
     * @param srcImg
     * @param pt
     * @return true
     * @return false
     */
    bool isVaildPoint(const cv::Mat& srcImg, const cv::Point& pt);

};

