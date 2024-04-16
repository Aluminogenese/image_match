#include "pch.h"
#include "lsq_matcher.h"

bool LsqMatcher::subPixelMatch(const cv::Mat& srcImg, const cv::Mat& dstImg, MatchPointPair& match)
{
    cv::Mat srcImgCopy, dstImgCopy;
    if (srcImg.channels() != 1)
        cv::cvtColor(srcImg, srcImgCopy, cv::COLOR_BGR2GRAY);
    else
        srcImgCopy = srcImg.clone();

    if (dstImg.channels() != 1)
        cv::cvtColor(dstImg, dstImgCopy, cv::COLOR_BGR2GRAY);
    else
        dstImgCopy = dstImg.clone();

    // 这里处理时认为x表示行，y表示列
    double y1 = match.srcPt.x;
    double x1 = match.srcPt.y;
    double y2 = match.dstPt.x;
    double x2 = match.dstPt.y;

    if (windowSize % 2 == 0)
        this->windowSize += 1;

    int r = windowSize / 2;

    // 使用ROI从原图中切出一个窗口
    cv::Rect rectSrc, rectDst;
    cv::Mat windowSrc, windowDst;
    rectSrc = cv::Rect(y1 - r, x1 - r, windowSize, windowSize);
    windowSrc = srcImgCopy(rectSrc);
    windowDst.create(cv::Size(windowSize, windowSize), CV_8UC1);

    // 设定几何畸变初值
    a0 = x2 - x1;
    a1 = 1;
    a2 = 0;
    b0 = y2 - y1;
    b1 = 0;
    b2 = 1;

    // 设定灰度畸变初值
    h0 = 0;
    h1 = 1;

    double xs = 0.0, ys = 0.0;
    double currentCorrelationIdx, bestCorrelationIdx = 0.0;
    cv::Point2d bestPt;

    for (int iter = 0; iter < 50; iter++) // 设定最大迭代次数不超过50次
    {
        Eigen::MatrixXd A(windowSize * windowSize, 8), L(windowSize * windowSize, 1), x;

        int num = 0;
        double xNumerator = 0.0, yNumerator = 0.0, xDenominator = 0.0, yDenominator = 0.0;

        for (int i = x1 - r; i <= x1 + r; i++)
            for (int j = y1 - r; j <= y1 + r; j++)
            {
                // 几何变形改正
                double m = a0 + a1 * i + a2 * j;
                double n = b0 + b1 * i + b2 * j;

                int I = floor(m);
                int J = floor(n);

                // 如果当前的点在图像的边界附近，就舍弃当前点，因为后面求导会出现问题
                if (I < 1 || I > dstImgCopy.rows || J < 1 || J > dstImgCopy.cols)
                    continue;

                // 重采样：双线性内插
                double pixelValue = (J + 1 - n) * ((I + 1 - m) * dstImgCopy.at<uchar>(I, J) + (m - I) * dstImgCopy.at<uchar>(I + 1, J)) + (n - J) * ((I + 1 - m) * dstImgCopy.at<uchar>(I, J + 1) + (m - I) * dstImgCopy.at<uchar>(I + 1, J + 1));

                // 辐射畸变改正
                pixelValue = h0 + h1 * pixelValue;
                windowDst.at<uchar>(i - x1 + r, j - y1 + r) = pixelValue;

                // 构建误差方程
                double gxDst = 0.5 * (dstImgCopy.at<uchar>(I + 1, J) - dstImgCopy.at<uchar>(I - 1, J));
                double gyDst = 0.5 * (dstImgCopy.at<uchar>(I, J + 1) - dstImgCopy.at<uchar>(I, J - 1));
                A(num, 0) = 1;
                A(num, 1) = pixelValue;
                A(num, 2) = gxDst;
                A(num, 3) = m * gxDst;
                A(num, 4) = n * gxDst;
                A(num, 5) = gyDst;
                A(num, 6) = m * gyDst;
                A(num, 7) = n * gyDst;

                L(num, 0) = srcImgCopy.at<uchar>(i, j) - pixelValue;

                // 计算最佳匹配点位
                double gxSrc = 0.5 * (srcImgCopy.at<uchar>(i + 1, j) - srcImgCopy.at<uchar>(i - 1, j));
                double gySrc = 0.5 * (srcImgCopy.at<uchar>(i, j + 1) - srcImgCopy.at<uchar>(i, j - 1));

                xNumerator += i * gxSrc * gxSrc;
                xDenominator += gxSrc * gxSrc;
                yNumerator += j * gySrc * gySrc;
                yDenominator += gySrc * gySrc;

                num++;
            }
        if (num < 8) // 无法求解法方程
            return false;

        currentCorrelationIdx = computeCorrelationIdx(windowSrc, windowDst);

        // std::cout << "Iter time: " << iter << std::endl;
        // std::cout << "a0 = " << a0 << "\ta1 = " << a1 << "\ta2 = " << a2 << std::endl;
        // std::cout << "b0 = " << b0 << "\tb1 = " << b1 << "\tb2 = " << b2 << std::endl;
        // std::cout << "h0 = " << h0 << "\th1 = " << h1 << std::endl;
        // std::cout << "idx = " << currentCorrelationIdx << std::endl;

        // std::cout << "A: \n" << A << std::endl;
        // std::cout << "L: \n" << L << std::endl;

        // 计算变形参数
        x = (A.transpose() * A).inverse() * (A.transpose() * L);
        // std::cout << "x: \n" << x << std::endl;
        // std::cout << std::endl;


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

        if (currentCorrelationIdx > bestCorrelationIdx)
        {
            bestPt.x = ys;
            bestPt.y = xs;
            bestCorrelationIdx = currentCorrelationIdx;
        }

        if (bestCorrelationIdx > threshold)
        {
            match.dstPt.x = bestPt.x;
            match.dstPt.y = bestPt.y;
            match.dist = bestCorrelationIdx;
            return true;
        }
    }

    match.dstPt.x = bestPt.x;
    match.dstPt.y = bestPt.y;
    match.dist = bestCorrelationIdx;
    return true;
}
