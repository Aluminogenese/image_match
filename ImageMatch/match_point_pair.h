#pragma once

#include <opencv2/opencv.hpp>

class MatchPointPair
{
public:
    cv::Point2d srcPt; // ��Ƭ�������
    cv::Point2d dstPt; // ��Ƭ�������
    double dist;       // �����Բ�ȵļ���ֵ

};

