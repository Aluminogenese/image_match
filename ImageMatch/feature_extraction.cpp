#include "pch.h"
#include "feature_extraction.h"


void featurextraction::HarrisCornerDetect(const cv::Mat& image, std::vector<cv::Point>& corners, const int& threshold) {
    cv::Mat image_grey = image.clone();
    if (image_grey.channels() != 1) {
        cv::cvtColor(image_grey, image_grey, cv::COLOR_BGR2GRAY);
    }
    
    cv::Mat interest_img, interest_img_norm, interest_img_norm_uint8;
    interest_img = cv::Mat::zeros(image_grey.size(), CV_32FC1);

    int blockSize = 2;
    int apertureSize = 3;
    double k = 0.04;

    cv::cornerHarris(image_grey, interest_img, blockSize, apertureSize, k, cv::BORDER_DEFAULT);

    cv::normalize(interest_img, interest_img_norm, 0, 255, cv::NORM_MINMAX, CV_32FC1);

    cv::convertScaleAbs(interest_img_norm, interest_img_norm_uint8);

    int radius = 5;

    // 筛选响应值大于阈值的点作为角点
#pragma omp parallel for
    for (int i = 0; i < interest_img_norm.rows; i++) {
        for (int j = 0; j < interest_img_norm.cols; j++) {
#pragma omp critical
            if ((int)*interest_img_norm.ptr<float>(i, j) > threshold) {
                corners.push_back(cv::Point(j, i)); // i->y, j->x
            }
        }
    }
}

void featurextraction::MoravecCornerDetect(const cv::Mat& image, std::vector<cv::Point>& corners, const int& interestWinSize, const int& restrainWinSize, const int& threshold){
    cv::Mat image_grey = image.clone();
    if (image_grey.channels() != 1)
        cv::cvtColor(image_grey, image_grey, cv::COLOR_RGB2GRAY);

    cv::GaussianBlur(image_grey, image_grey, cv::Size(interestWinSize, interestWinSize), 0, 0);

    cv::Mat interest_img = cv::Mat::zeros(image_grey.size(), CV_32FC1);
    int insterest_step = interestWinSize / 2;
    /// 计算兴趣值
#pragma omp parallel for
    for (int i = insterest_step; i < image_grey.rows - insterest_step; i++) {
        for (int j = insterest_step; j < image_grey.cols - insterest_step; j++)
        {
            cv::v_int32x4 value = cv::v_setzero_s32();
            for (int k = -insterest_step; k < insterest_step; k++)
            {
                cv::v_int32x4 lvalue(*image_grey.ptr<uchar>(i + k, j), *image_grey.ptr<uchar>(i, j + k), *image_grey.ptr<uchar>(i + k, j + k), *image_grey.ptr<uchar>(i + k, j - k));
                cv::v_int32x4 rvalue(*image_grey.ptr<uchar>(i + k + 1, j), *image_grey.ptr<uchar>(i, j + k + 1), *image_grey.ptr<uchar>(i + k + 1, j + k + 1), *image_grey.ptr<uchar>(i + k + 1, j - k - 1));
                cv::v_int32x4 v_1 = (lvalue - rvalue) * (lvalue - rvalue);
                value += v_1;
            }
            *interest_img.ptr<float>(i, j) = static_cast<float>(cv::v_reduce_min(value));
        }
    }
    /// 选取候选点
    int restrain_step = restrainWinSize / 2;
    int maxValue;
    cv::Point point;
#pragma omp parallel for
    for (int i = restrain_step; i < image_grey.rows - restrain_step; i += restrain_step)
    {
        for (int j = restrain_step; j < image_grey.cols - restrain_step; j += restrain_step)
        {
            point.x = -1;
            point.y = -1;
            maxValue = 0;
            for (int m = -restrain_step; m < restrain_step; m++)
            {
                for (int n = -restrain_step; n < restrain_step; n++)
                {
                    if (*interest_img.ptr<float>(i + m, j + n) > maxValue)
                    {
                        maxValue = *interest_img.ptr<float>(i + m, j + n);
                        point.y = i + m;
                        point.x = j + n;
                    }
                }
            }
#pragma omp critical
            if (maxValue > threshold)
            {
                corners.push_back(point);
            }
        }
    }

}
void featurextraction::SIFTCornerDetect(const cv::Mat& srcImg, std::vector<cv::Point>& corners)
{
    cv::Ptr<cv::FeatureDetector> detector = cv::SIFT::create();
    std::vector<cv::KeyPoint> keypoints;
    detector->detect(srcImg, keypoints);

    for (const cv::KeyPoint& kpt : keypoints)
    {
        corners.push_back(cv::Point(kpt.pt));
    }
}


void featurextraction::DrawCorners(const cv::Mat& srcImg, cv::Mat& outputImg, const std::vector<cv::Point>& corners)
{
    outputImg = srcImg.clone();

    //用圆和十字丝标出特征点
    int radius = 4;
#pragma omp parallel for
    for (int i = 0; i < corners.size(); i++)
    {
        int xx = corners.at(i).x;
        int yy = corners.at(i).y;
        cv::circle(outputImg, cv::Point(xx, yy), radius, cv::Scalar(0, 255, 255), 1, cv::LINE_AA);
        //特征点标号
        putText(outputImg, std::to_string(i), cv::Point(xx + 2, yy - 2), cv::FONT_HERSHEY_SIMPLEX, 0.4, CV_RGB(255, 0, 0), 1.8, cv::LINE_AA);
        cv::line(outputImg, cv::Point(xx - radius - 1, yy), cv::Point(xx + radius + 1, yy), cv::Scalar(0, 255, 255), 1, cv::LINE_AA);
        cv::line(outputImg, cv::Point(xx, yy - radius - 1), cv::Point(xx, yy + radius + 1), cv::Scalar(0, 255, 255), 1, cv::LINE_AA);
    }
}
