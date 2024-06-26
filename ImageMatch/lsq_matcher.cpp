#include "pch.h"
#include "lsq_matcher.h"

bool lsqmatch::subPixelMatch(MatchPointPair& matchPoint, const cv::Mat& leftImage, const cv::Mat& rightImage, const int& winSize, const float& threshold)
{
    double left_x = matchPoint.leftPt.y;//row
    double left_y = matchPoint.leftPt.x;//col
    double right_x = matchPoint.rightPt.y;
    double right_y = matchPoint.rightPt.x;

    int step = winSize / 2;

    // 确定目标窗口
    cv::Mat left_window = leftImage(cv::Rect(left_y - step, left_x - step, winSize, winSize));
    left_window.convertTo(left_window, CV_32FC1);
    cv::Mat right_window = cv::Mat::zeros(winSize, winSize, CV_32FC1);
    // 设定几何畸变初值
    double a0 = right_x - left_x;
    double a1 = 1;
    double a2 = 0;
    double b0 = right_y - left_y;
    double b1 = 0;
    double b2 = 1;
    // 设定灰度畸变初值
    double h0 = 0;
    double h1 = 1;

    double xs = 0.0, ys = 0.0;
    double current_correlation_index = 0.0, best_correlation_index = 0.0;
    cv::Point2f best_leftPt, best_rightPt;

    for (int iter = 0; iter < 50; iter++) // 设定最大迭代次数不超过50次
    {
        Eigen::MatrixXd A(winSize * winSize, 8), L(winSize * winSize, 1), x;

        int num = 0;
        double xNumerator = 0.0, yNumerator = 0.0, xDenominator = 0.0, yDenominator = 0.0;

        for (int i = left_x - step; i <= left_x + step; i++) {
            for (int j = left_y - step; j <= left_y + step; j++)
            {
                // 几何变形改正
                double m = a0 + a1 * i + a2 * j;//row
                double n = b0 + b1 * i + b2 * j;//col

                int I = floor(m);//row
                int J = floor(n);//col

                // 如果当前的点在图像的边界附近，就舍弃当前点，因为后面求导会出现问题
                if (I < 2 || I > rightImage.rows - 2 || J < 2 || J > rightImage.cols - 2) {
                    continue;
                }

                uchar value_11 = rightImage.at<uchar>(I, J);
                uchar value_12 = rightImage.at<uchar>(I + 1, J);
                uchar value_13 = rightImage.at<uchar>(I, J + 1);
                uchar value_14 = rightImage.at<uchar>(I + 1, J + 1);
                // 重采样：双线性内插
                double pixelValue = (J + 1 - n) * ((I + 1 - m) * value_11 + (m - I) * value_12) + (n - J) * ((I + 1 - m) * value_13 + (m - I) * value_14);

                // 辐射畸变改正
                pixelValue = h0 + h1 * pixelValue;
                right_window.at<float>(i - left_x + step, j - left_y + step) = pixelValue;

                // 构建误差方程
                double gxDst = 0.5 * (rightImage.at<uchar>(I + 1, J) - rightImage.at<uchar>(I - 1, J));
                double gyDst = 0.5 * (rightImage.at<uchar>(I, J + 1) - rightImage.at<uchar>(I, J - 1));
                A(num, 0) = 1;
                A(num, 1) = pixelValue;
                A(num, 2) = gxDst;
                A(num, 3) = m * gxDst;
                A(num, 4) = n * gxDst;
                A(num, 5) = gyDst;
                A(num, 6) = m * gyDst;
                A(num, 7) = n * gyDst;

                L(num, 0) = leftImage.at<uchar>(i, j) - pixelValue;

                // 计算最佳匹配点位
                double gxSrc = 0.5 * (leftImage.at<uchar>(i + 1, j) - leftImage.at<uchar>(i - 1, j));
                double gySrc = 0.5 * (leftImage.at<uchar>(i, j + 1) - leftImage.at<uchar>(i, j - 1));

                xNumerator += i * gxSrc * gxSrc;
                xDenominator += gxSrc * gxSrc;
                yNumerator += j * gySrc * gySrc;
                yDenominator += gySrc * gySrc;

                num++;
            }
        }
        if (num < 8) // 无法求解法方程
            return false;
        correlationmatch::normalize_window(left_window);
        correlationmatch::normalize_window(right_window);
        correlationmatch::calculte_coefficient(current_correlation_index, left_window, right_window);

        // 计算变形参数
        x = (A.transpose() * A).inverse() * (A.transpose() * L);

        double a0_old = a0;
        double a1_old = a1;
        double a2_old = a2;
        double b0_old = b0;
        double b1_old = b1;
        double b2_old = b2;
        double h0_old = h0;
        double h1_old = h1;

        a0 = a0_old + x(2, 0) + a0_old * x(3, 0) + b0_old * x(4, 0);
        a1 = a1_old + a1_old * x(3, 0) + b1_old * x(4, 0);
        a2 = a2_old + a2_old * x(3, 0) + b2_old * x(4, 0);
        b0 = b0_old + x(5, 0) + a0_old * x(6, 0) + b0_old * x(7, 0);
        b1 = b1_old + a1_old * x(6, 0) + b1_old * x(7, 0);
        b2 = b2_old + a2_old * x(6, 0) + b2_old * x(7, 0);
        h0 = h0_old + x(0, 0) + h0_old * x(1, 0);
        h1 = h1_old + h1_old * x(1, 0);

        // 计算最佳匹配点位
        double xt = xNumerator / xDenominator;
        double yt = yNumerator / yDenominator;

        xs = a0 + a1 * xt + a2 * yt;
        ys = b0 + b1 * xt + b2 * yt;

        if (current_correlation_index > best_correlation_index)
        {
            best_leftPt.x = yt;
            best_leftPt.y = xt;
            best_rightPt.x = ys;
            best_rightPt.y = xs;
            best_correlation_index = current_correlation_index;
        }
    }
    if (best_correlation_index > threshold)
    {
        matchPoint.leftPt.x = best_leftPt.x;
        matchPoint.leftPt.y = best_leftPt.y;
        matchPoint.rightPt.x = best_rightPt.x;
        matchPoint.rightPt.y = best_rightPt.y;
        matchPoint.dist = best_correlation_index;
        return true;
    }
    else {
        return false;
    }
}

void lsqmatch::match(std::vector<MatchPointPair>& matchPoints, std::vector<MatchPointPair>& corrMatchPnts, cv::Mat& leftImage, const cv::Mat& rightImage, const int& winSize, const float& threshold)
{
    cv::Mat left_image_grey = leftImage.clone();
    cv::Mat right_image_grey = rightImage.clone();
    if (left_image_grey.channels() != 1)
        cv::cvtColor(left_image_grey, left_image_grey, cv::COLOR_BGR2GRAY);
    if (right_image_grey.channels() != 1)
        cv::cvtColor(right_image_grey, right_image_grey, cv::COLOR_BGR2GRAY);

#pragma omp parallel for
    for (int i = 0; i < corrMatchPnts.size(); i++)
    {
        auto& match_point = corrMatchPnts[i];
        bool is_accepted = lsqmatch::subPixelMatch(match_point, left_image_grey, right_image_grey, winSize, threshold);
        if (is_accepted) {
#pragma omp critical
            matchPoints.push_back(match_point);
        }
    }
}
