#pragma once

#include <opencv2/opencv.hpp>

class MatchPointPair
{
public:
    cv::Point2d srcPt; // 左片像点坐标
    cv::Point2d dstPt; // 右片像点坐标
    double dist;       // 相似性测度的计算值

};

