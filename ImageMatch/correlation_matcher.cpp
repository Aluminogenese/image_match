#include "pch.h"
#include "correlation_matcher.h"
//x:col,y:row
void correlationmatch::determine_match_window(cv::Mat& matchWindow, const cv::Mat& image, const cv::Point& centralPnt, const int& matchWinSize) {
    matchWindow.create(matchWinSize, matchWinSize, CV_8UC1);
    int match_step = matchWinSize / 2;//边长的一半
    int step = cv::v_uint8x16::nlanes;
    for (int i = 0; i < matchWinSize; i++) {
        const uchar* image_ptr = image.ptr<uchar>(centralPnt.y - match_step + i);
        uchar* match_window_ptr = matchWindow.ptr<uchar>(i);
        int j = 0;
        for (; j < matchWinSize - step; j += step) {
            cv::v_uint8x16 vec = cv::v_load(image_ptr + centralPnt.x - match_step + j);
            cv::v_store(match_window_ptr + j, vec);
        }
        for (; j < matchWinSize; j++) {
            match_window_ptr[j] = image_ptr[centralPnt.x - match_step + j];
        }
    }
    matchWindow.convertTo(matchWindow, CV_32FC1);
}

void correlationmatch::normalize_window(cv::Mat& matchWindow) {
    float grey_average = static_cast<float>(cv::mean(matchWindow)[0]);
    cv::v_float32x4 avg = cv::v_setall_f32(grey_average);
    int step = cv::v_float32x4::nlanes;
    for (int i = 0; i < matchWindow.rows; i++) {
        float* match_window_ptr = matchWindow.ptr<float>(i);
        int j = 0;
        for (; j < matchWindow.cols - step; j += step) {
            cv::v_float32x4 vec = cv::v_load(match_window_ptr + j);
            vec -= avg;
            cv::v_store(match_window_ptr + j, vec);
        }
        for (; j < matchWindow.cols; j++) {
            match_window_ptr[j] -= grey_average;
        }
    }
}

//void correlationmatch::determine_match_window(cv::Mat& matchWindow, const cv::Mat& image, const cv::Point& centralPnt, const int& matchWinSize) {
//    matchWindow.create(matchWinSize, matchWinSize, CV_32FC1);
//    int match_step = matchWinSize / 2;//边长的一半
//    for (int i = 0; i < matchWinSize; i++) {
//        const uchar* image_ptr = image.ptr<uchar>(centralPnt.y - match_step + i);
//        float* match_window_ptr = matchWindow.ptr<float>(i);
//        for (int j = 0; j < matchWinSize; j++) {
//            match_window_ptr[j] = static_cast<float>(image_ptr[centralPnt.x - match_step + j]);
//        }
//    }
//}

//void correlationmatch::normalize_window(cv::Mat& matchWindow) {
//    float grey_average = static_cast<float>(cv::mean(matchWindow)[0]);
//    for (int i = 0; i < matchWindow.rows; i++) {
//        float* match_window_ptr = matchWindow.ptr<float>(i);
//        for (int j = 0; j < matchWindow.cols; j++) {
//            match_window_ptr[j] -= grey_average;
//        }
//    }
//}

void correlationmatch::calculte_coefficient(double& cofficent, const cv::Mat& leftWindow, const cv::Mat& rightWindow) {
    //开始计算相关系数
    float cofficent1 = 0;
    float cofficent2 = 0;
    float cofficent3 = 0;

    cv::v_float32x4 cofficent1_vec = cv::v_setzero_f32();
    cv::v_float32x4 cofficent2_vec = cv::v_setzero_f32();
    cv::v_float32x4 cofficent3_vec = cv::v_setzero_f32();

    int step = cv::v_float32x4::nlanes;

    for (int i = 0; i < leftWindow.rows; i++)
    {
        const float* left_window_ptr = leftWindow.ptr<float>(i);
        const float* right_window_ptr = rightWindow.ptr<float>(i);
        int j = 0;
        for (; j < leftWindow.cols - step; j += step)
        {
            cv::v_float32x4 left_vec = cv::v_load(left_window_ptr + j);
            cv::v_float32x4 right_vec = cv::v_load(right_window_ptr + j);
            cofficent1_vec += left_vec * right_vec;
            cofficent2_vec += left_vec * left_vec;
            cofficent3_vec += right_vec * right_vec;
            //cofficent1 += left_window_ptr[j] * right_window_ptr[j];
            //cofficent2 += left_window_ptr[j] * left_window_ptr[j];
            //cofficent3 += right_window_ptr[j] * right_window_ptr[j];
        }
        for (; j < leftWindow.cols; j++) {
            cofficent1 += left_window_ptr[j] * right_window_ptr[j];
            cofficent2 += left_window_ptr[j] * left_window_ptr[j];
            cofficent3 += right_window_ptr[j] * right_window_ptr[j];
        }
    }
    cofficent1 += cv::v_reduce_sum(cofficent1_vec);
    cofficent2 += cv::v_reduce_sum(cofficent2_vec);
    cofficent3 += cv::v_reduce_sum(cofficent3_vec);

    cofficent = cofficent1 / sqrt(cofficent2 * cofficent3);
}

//void correlationmatch::calculte_coefficient(double& cofficent, const cv::Mat& leftWindow, const cv::Mat& rightWindow) {
//    //开始计算相关系数
//    float cofficent1 = 0;
//    float cofficent2 = 0;
//    float cofficent3 = 0;
//    for (int i = 0; i < leftWindow.rows; i++)
//    {
//        const float* left_window_ptr = leftWindow.ptr<float>(i);
//        const float* right_window_ptr = rightWindow.ptr<float>(i);
//        for (int j = 0; j < leftWindow.cols; j++)
//        {
//            cofficent1 += left_window_ptr[j] * right_window_ptr[j];
//            cofficent2 += left_window_ptr[j] * left_window_ptr[j];
//            cofficent3 += right_window_ptr[j] * right_window_ptr[j];
//        }
//    }
//    cofficent = cofficent1 / sqrt(cofficent2 * cofficent3);
//}

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
    //cv::GaussianBlur(left_image_grey, left_image_grey, cv::Size(matchWinSize, matchWinSize), 0, 0);
    //cv::GaussianBlur(right_image_grey, right_image_grey, cv::Size(matchWinSize, matchWinSize), 0, 0);

    int match_step = matchWinSize / 2;//边长的一半
    int searchWinSize = 101;
    int search_step = searchWinSize / 2;
#pragma omp parallel for
    for (int k = 0; k < leftCorners.size(); k++)
    {
        auto& left_corner = leftCorners[k];
        if (left_corner.x < match_step + 1 || left_corner.y < match_step + 1 || (left_image_grey.cols - left_corner.x) < match_step + 1 || (left_image_grey.rows - left_corner.y) < match_step + 1) {
            continue;
        }
        cv::Mat match_left_window;
        determine_match_window(match_left_window, left_image_grey, left_corner, matchWinSize);
        normalize_window(match_left_window);
        double coefficient_max = 0.;
        cv::Point3f match_point;
        for (int i = 0; i < searchWinSize - matchWinSize; i++) {
            if (left_corner.y < search_step + 1 || (right_image_grey.rows - left_corner.y) < search_step + 1) {
                continue;
            }
            int right_point_y = left_corner.y - (search_step - match_step) + i;
            for (int j = match_step; j < right_image_grey.cols - match_step; j++) {
                cv::Mat match_right_window;
                determine_match_window(match_right_window, right_image_grey, cv::Point(j, right_point_y), matchWinSize);
                normalize_window(match_right_window);
                double coefficient = 0.;
                calculte_coefficient(coefficient, match_left_window, match_right_window);
                if (coefficient > coefficient_max) {
                    match_point = cv::Point3f(j, right_point_y, coefficient);
                    coefficient_max = coefficient;
                }
            }
        }

        if (match_point.z > threshold) {
#pragma omp critical
            match_points.push_back(MatchPointPair(left_corner, cv::Point2f(match_point.x, match_point.y), match_point.z));
        }
    }
}