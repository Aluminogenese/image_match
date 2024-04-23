#pragma once

#include <opencv2/opencv.hpp>
#include <opencv2/core/simd_intrinsics.hpp>

namespace featurextraction
{
    /**
     * @brief Harris���ӽǵ���
     *
     * @param image ���ͼ��
     * @param corners ���õ��Ľǵ�
     * @param threshold Harris��Ӧֵ����ֵ
     */
    void HarrisCornerDetect(const cv::Mat& image, std::vector<cv::Point>& corners, const int& threshold = 1000);

    /**
     * @brief Moravec�ǵ���
     *
     * @param image ���ͼ��
     * @param corners ���õ��Ľǵ�
     * @param windowSize ������Ȥֵ�Ĵ��ڴ�С��Ĭ��Ϊ9��9
     * @param restrainWinSize ���ƴ��ڴ�С��Ĭ��Ϊ21��21
     * @param threshold ��ֵ��Ĭ��Ϊ700
     */
    void MoravecCornerDetect(const cv::Mat& image, std::vector<cv::Point>& corners, const int& interestWinSize = 9, const int& restrainWinSize = 101, const int& threshold = 1000);

    /**
     * @brief SIFT�ǵ���
     *
     * @param image ���ͼ��
     * @param corners ���õ��Ľǵ�
     */
    void  SIFTCornerDetect(const cv::Mat& image, std::vector<cv::Point>& corners);

    /**
     * @brief ��ͼ���ϻ��ƽǵ�
     *
     * @param srcImg ԭͼ��
     * @param outputImg ���ƽǵ��ͼ��
     * @param corners �ǵ�
     */
    void DrawCorners(const cv::Mat& srcImg, cv::Mat& outputImg, const std::vector<cv::Point>& corners);
};

