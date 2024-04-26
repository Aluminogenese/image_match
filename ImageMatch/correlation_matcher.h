#pragma once
#include<opencv2/opencv.hpp>
#include<opencv2/core/simd_intrinsics.hpp>
#include "match_point_pair.h"
namespace correlationmatch {
    /**
     * @brief �����ĵ��봰�ڴ�Сȷ��ƥ��Ŀ�괰��
     *
     * @param matchWindow �����ȷ����Ŀ�괰��
     * @param image Ŀ��ͼ��
     * @param centralPnt Ŀ�괰�����ĵ�
     * @param matchWinSize Ŀ�괰�ڴ�С
     */
    void determine_match_window(cv::Mat& matchWindow, const cv::Mat& image, const cv::Point& centralPnt, const int& matchWinSize);

    /**
     * @brief ��һ��ƥ�䴰��
     *
     * @param matchWindow ����в�����ƥ�䴰��
     */
    void normalize_window(cv::Mat& matchWindow);

    /**
     * @brief �����һ�����ϵ��NCC
     *
     * @param cofficent ����õ���NCC
     * @param leftWindow ��ƥ�䴰��
     * @param rightWindow ��ƥ�䴰��
     */
    void calculte_coefficient(double& cofficent, const cv::Mat& leftWindow, const cv::Mat& rightWindow);

    /**
     * @brief ����Ӱ�����ϵ��ƥ��
     *
     * @param matchPoints ƥ��õ���ͬ����
     * @param leftImage ��Ӱ��
     * @param rightImage ��Ӱ��
     * @param leftCorners ��Ӱ������ȡ��������
     * @param matchWinSize ƥ�䴰�ڴ�С��Ĭ��Ϊ9
     * @param threshold ���ϵ��ƥ�����ֵ
     */
    void match(std::vector<MatchPointPair>& matchPoints, const cv::Mat& leftImage, const cv::Mat& rightImage, const std::vector<cv::Point>& leftCorners, const int& matchWinSize = 9, const int&searWinSize=101, const float& threshold = 0.7);
};

