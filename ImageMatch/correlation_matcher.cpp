#include "pch.h"
#include "correlation_matcher.h"
//x:col,y:row

void correlationmatch::determine_match_window(cv::Mat& matchWindow, const cv::Mat& image, const cv::Point& centralPnt, const int& matchWinSize) {
    matchWindow.create(matchWinSize, matchWinSize, CV_32FC1);
    int match_step = matchWinSize / 2;//边长的一半
    for (int i = 0; i < matchWinSize; i++) {
        const uchar* image_ptr = image.ptr<uchar>(centralPnt.y - match_step + i);
        float* match_window_ptr = matchWindow.ptr<float>(i);
        for (int j = 0; j < matchWinSize; j++) {
            match_window_ptr[j] = static_cast<float>(image_ptr[centralPnt.x - match_step + j]);
        }
    }
}

void correlationmatch::normalize_window(cv::Mat& matchWindow) {
    float grey_average = static_cast<float>(cv::mean(matchWindow)[0]);
    for (int i = 0; i < matchWindow.rows; i++) {
        float* match_window_ptr = matchWindow.ptr<float>(i);
        for (int j = 0; j < matchWindow.cols; j++) {
            match_window_ptr[j] -= grey_average;
        }
    }
}

void correlationmatch::calculte_coefficient(double& cofficent, const cv::Mat& leftWindow, const cv::Mat& rightWindow) {
    //开始计算相关系数
    float cofficent1 = 0;
    float cofficent2 = 0;
    float cofficent3 = 0;
    for (int i = 0; i < leftWindow.rows; i++)
    {
        const float* left_window_ptr = leftWindow.ptr<float>(i);
        const float* right_window_ptr = rightWindow.ptr<float>(i);
        for (int j = 0; j < leftWindow.cols; j++)
        {
            cofficent1 += left_window_ptr[j] * right_window_ptr[j];
            cofficent2 += left_window_ptr[j] * left_window_ptr[j];
            cofficent3 += right_window_ptr[j] * right_window_ptr[j];
        }
    }
    cofficent = cofficent1 / sqrt(cofficent2 * cofficent3);
}

void correlationmatch::match(std::vector<MatchPointPair>& match_points, const cv::Mat& leftImage, const cv::Mat& rightImage, const std::vector<cv::Point>& leftCorners, const int& matchWinSize, const float& threshold)
{
    cv::Mat left_image_grey = leftImage.clone();
    cv::Mat right_image_grey = rightImage.clone();
    if (left_image_grey.channels() != 1) {
        cv::cvtColor(left_image_grey, left_image_grey, cv::COLOR_BGR2GRAY);
    }
    if (right_image_grey.channels() != 1) {
        cv::cvtColor(right_image_grey, right_image_grey, cv::COLOR_BGR2GRAY);
    }

    int match_step = matchWinSize / 2;//边长的一半

//#pragma omp parallel for
    for (auto& left_corner : leftCorners)
    {
        if (left_corner.x < match_step + 1 || left_corner.y < match_step + 1 || (left_image_grey.cols - left_corner.x) < match_step + 1 || (left_image_grey.rows - left_corner.y) < match_step + 1) {
            continue;
        }
        cv::Mat match_left_window;
        determine_match_window(match_left_window, left_image_grey, left_corner, matchWinSize);
        normalize_window(match_left_window);
        double coefficient_max = 0.;
        cv::Point3f match_point;
        for (int i = match_step; i < right_image_grey.cols - match_step; i++) {
            if ((right_image_grey.rows - left_corner.y) < match_step + 1) {
                continue;
            }

            cv::Mat match_right_window;

            determine_match_window(match_right_window, right_image_grey, cv::Point(i, left_corner.y), matchWinSize);
            normalize_window(match_right_window);
            double coefficient = 0.;
            calculte_coefficient(coefficient, match_left_window, match_right_window);
            if (coefficient > coefficient_max) {
                match_point = cv::Point3f(i, left_corner.y, coefficient);
                coefficient_max = coefficient;
            }
        }
        //#pragma omp critical
        if (match_point.z > threshold) {
            match_points.push_back(MatchPointPair(left_corner, cv::Point2f(match_point.x, match_point.y), match_point.z));
        }
    }
}