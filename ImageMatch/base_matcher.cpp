#include "pch.h"
#include "base_matcher.h"

void BaseMatcher::setWindowSize(int windowSize)
{
	this->windowSize = windowSize;
}

void BaseMatcher::setThreshold(double threshold)
{
	this->threshold = threshold;
}

void BaseMatcher::drawMatches(const cv::Mat& srcImg, const cv::Mat& dstImg, cv::Mat& outputImg, std::vector<MatchPointPair>& matches)
{
    outputImg.create(cv::Size(srcImg.cols + dstImg.cols, std::max(srcImg.rows, dstImg.rows)), CV_8UC3);

    srcImg.copyTo(outputImg(cv::Rect(0, 0, srcImg.cols, srcImg.rows)));
    dstImg.copyTo(outputImg(cv::Rect(srcImg.cols, 0, dstImg.cols, dstImg.rows)));

    cv::Point pt1, pt2;
    static std::default_random_engine e;
    static std::uniform_int_distribution<int> u(0, 255);

    for (MatchPointPair match : matches)
    {
        cv::Scalar color(u(e), u(e), u(e));

        pt1 = match.srcPt;
        pt2 = cv::Point(match.dstPt.x + srcImg.cols, match.dstPt.y);

        cv::circle(outputImg, pt1, 5, color, 2);
        cv::circle(outputImg, pt2, 5, color, 2);
        cv::line(outputImg, pt1, pt2, color, 2);
    }
}

double BaseMatcher::computeCorrelationIdx(const cv::Mat& srcWindow, const cv::Mat& dstWindow)
{
    if (srcWindow.size != dstWindow.size)
    {
        std::cerr << "窗口大小不匹配！" << std::endl;
        return 0;
    }

    // 总像元数量
    double totalNum = srcWindow.rows * srcWindow.cols;

    // 计算两张影像窗口中像素的平均值
    double gAverSrc = 0.0, gAverDst = 0.0;
    for (int i = 0; i < srcWindow.rows; i++)
        for (int j = 0; j < srcWindow.cols; j++)
        {
            gAverSrc += srcWindow.at<unsigned char>(i, j);
            gAverDst += dstWindow.at<unsigned char>(i, j);
        }
    gAverSrc /= totalNum;
    gAverDst /= totalNum;

    // 计算相关系数
    double numerator = 0.0;
    double denominatorSrc = 0.0;
    double denominatorDst = 0.0;

    for (int i = 0; i < srcWindow.rows; i++)
        for (int j = 0; j < srcWindow.cols; j++)
        {
            numerator += (srcWindow.at<unsigned char>(i, j) - gAverSrc) * (dstWindow.at<unsigned char>(i, j) - gAverDst);
            denominatorSrc += pow((srcWindow.at<unsigned char>(i, j) - gAverSrc), 2);
            denominatorDst += pow((dstWindow.at<unsigned char>(i, j) - gAverDst), 2);
        }

    double denominator = sqrt(denominatorSrc * denominatorDst);
    return abs(numerator / denominator);
}

bool BaseMatcher::isVaildPoint(const cv::Mat& srcImg, const cv::Point& pt)
{
    double r = windowSize / 2;
    if (pt.x < r || pt.y < r || pt.x >= srcImg.cols - r || pt.y >= srcImg.rows - r)
        return false;
    return true;
}
