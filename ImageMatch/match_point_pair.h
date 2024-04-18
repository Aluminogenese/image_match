#pragma once

#include <opencv2/opencv.hpp>

class MatchPointPair
{
public:
    MatchPointPair() {
        leftPt = cv::Point2f(0., 0.);
        rightPt = cv::Point2f(0., 0.);
        dist = 0.;
    }
    MatchPointPair(const cv::Point2f& left_point, const cv::Point2f& right_point, const float& distance) {
        leftPt = left_point;
        rightPt = right_point;
        dist = distance;
    };
public:
    cv::Point2f leftPt; // 左片像点坐标
    cv::Point2f rightPt; // 右片像点坐标
    float dist;       // 相似性测度的计算值
};

