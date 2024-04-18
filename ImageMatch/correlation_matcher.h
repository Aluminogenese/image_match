#pragma once
#include<opencv2/opencv.hpp>
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

    void normalize_window(cv::Mat& matchWindow);

    void calculte_coefficient(double& cofficent, const cv::Mat& leftWindow, const cv::Mat& rightWindow);

    void match(std::vector<MatchPointPair>& match_points, const cv::Mat& leftImage, const cv::Mat& rightImage, const std::vector<cv::Point>& leftCorners, const int& matchWinSize = 9, const float& threshold = 0.7);


    /**
     * @brief �ڲο�ͼ���Ŀ��ͼ����ͬʱ��ȡ���������ƥ��
     *
     * @param srcImg �ο�ͼ��
     * @param dstImg Ŀ��ͼ��
     * @param srcPts �ο�ͼ���ϵ�������
     * @param dstPts Ŀ��ͼ���ϵ�������
     * @param matches ���ƥ���ϵ�������
     */


};

