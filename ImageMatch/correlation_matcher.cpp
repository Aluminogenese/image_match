#include "pch.h"
#include "correlation_matcher.h"
//���ϵ��ͼ��ƥ��
float Get_coefficient(cv::Mat matchLeftWindow, cv::Mat imageRight, int x, int y)
{
    //��������������ȷ�����������ڵĴ�С
    cv::Mat Rmatchwindow;
    Rmatchwindow.create(matchLeftWindow.rows, matchLeftWindow.cols, CV_32FC1);
    float aveRImg = 0;
    for (int m = 0; m < matchLeftWindow.rows; m++)
    {
        for (int n = 0; n < matchLeftWindow.cols; n++)
        {
            aveRImg += imageRight.at<uchar>(x + m, y + n);
            Rmatchwindow.at<float>(m, n) = imageRight.at<uchar>(x + m, y + n);
        }
    }
    aveRImg = aveRImg / (matchLeftWindow.rows * matchLeftWindow.cols);
    for (int m = 0; m < matchLeftWindow.rows; m++)
    {
        for (int n = 0; n < matchLeftWindow.cols; n++)
        {
            Rmatchwindow.at<float>(m, n) -= aveRImg;
        }
    }
    //��ʼ�������ϵ��
    float cofficent1 = 0;
    float cofficent2 = 0;
    float cofficent3 = 0;
    for (int m = 0; m < matchLeftWindow.rows; m++)
    {
        for (int n = 0; n < matchLeftWindow.cols; n++)
        {
            cofficent1 += matchLeftWindow.at<float>(m, n) * Rmatchwindow.at<float>(m, n);
            cofficent2 += Rmatchwindow.at<float>(m, n) * Rmatchwindow.at<float>(m, n);
            cofficent3 += matchLeftWindow.at<float>(m, n) * matchLeftWindow.at<float>(m, n);
        }
    }
    double cofficent = cofficent1 / sqrt(cofficent2 * cofficent3);
    return cofficent;
}
void vectorsort(std::vector <cv::Point3f>& Temp_sort)
{
    for (int i = 0; i < Temp_sort.size() - 1; i++) {
        float tem = 0;
        float temx = 0;
        float temy = 0;
        // �ڲ�forѭ���������ڵ�����Ԫ�ؽ��бȽ�
        for (int j = i + 1; j < Temp_sort.size(); j++) {
            if (Temp_sort.at(i).z < Temp_sort.at(j).z) {
                tem = Temp_sort.at(j).z;
                Temp_sort.at(j).z = Temp_sort.at(i).z;
                Temp_sort.at(i).z = tem;

                temx = Temp_sort.at(j).x;
                Temp_sort.at(j).x = Temp_sort.at(i).x;
                Temp_sort.at(i).x = temx;

                temy = Temp_sort.at(j).y;
                Temp_sort.at(j).y = Temp_sort.at(i).y;
                Temp_sort.at(i).y = temy;
            }
        }
    }
}
void lastview(cv::Mat imageLeftRGB, cv::Mat imageRightRGB, std::vector<cv::Point3f> featurePointLeft, std::vector<cv::Point3f> featurePointRight)
{
    cv::Mat bothview;//���ͼ��
    bothview.create(imageLeftRGB.rows, imageLeftRGB.cols + imageRightRGB.cols, imageLeftRGB.type());
    for (int i = 0; i < imageLeftRGB.rows; i++)
    {
        for (int j = 0; j < imageLeftRGB.cols; j++)
        {
            bothview.at<cv::Vec3b>(i, j) = imageLeftRGB.at<cv::Vec3b>(i, j);
        }
    }

    for (int i = 0; i < imageRightRGB.rows; i++)
    {
        for (int j = imageLeftRGB.cols; j < imageLeftRGB.cols + imageRightRGB.cols; j++)
        {
            bothview.at<cv::Vec3b>(i, j) = imageRightRGB.at<cv::Vec3b>(i, j - imageLeftRGB.cols);
        }
    }//����Ӱ��϶�Ϊһ	
    for (int i = 0; i < featurePointRight.size(); i++)
    {
        int a = (rand() % 200);
        int b = (rand() % 200 + 99);
        int c = (rand() % 200) - 50;
        if (a > 100 || a < 0)
        {
            a = 255;
        }
        if (b > 255 || b < 0)
        {
            b = 88;
        }
        if (c > 255 || c < 0)
        {
            c = 188;
        }
        int radius = 5;
        //��Ƭ
        int lm = int(featurePointLeft.at(i).y);
        int ln = int(featurePointLeft.at(i).x);

        cv::circle(bothview, cv::Point(lm, ln), radius, cv::Scalar(0, 255, 255), 1, 4, 0);
        cv::line(bothview, cv::Point(lm - radius - 2, ln), cv::Point(lm + radius + 2, ln), cv::Scalar(0, 255, 255), 1, 8, 0);
        cv::line(bothview, cv::Point(lm, ln - radius - 2), cv::Point(lm, ln + radius + 2), cv::Scalar(0, 255, 255), 1, 8, 0);

        //��Ƭ
        int rm = int(featurePointRight.at(i).y + imageLeftRGB.cols);
        int rn = int(featurePointRight.at(i).x);

        cv::circle(bothview, cv::Point(rm, rn), radius, cv::Scalar(0, 255, 255), 1, 4, 0);
        cv::line(bothview, cv::Point(rm - radius - 2, rn), cv::Point(rm + radius + 2, rn), cv::Scalar(0, 255, 255), 1, 8, 0);
        cv::line(bothview, cv::Point(rm, rn - radius - 2), cv::Point(rm, rn + radius + 2), cv::Scalar(0, 255, 255), 1, 8, 0);
        //����
        cv::line(bothview, cv::Point(lm, ln), cv::Point(rm, rn), cv::Scalar(a, b, c), 1, 8, 0);
    }

    cv::imshow("����ƬӰ��ͬ����չʾ", bothview);
    cv::waitKey(0);
}
int kyhMatchingImg(std::string pathLeft, std::string pathRight, std::vector<cv::Point3f> featurePointLeft)
{
    cv::Mat imageLeft, imageLeftRGB = cv::imread(pathLeft, cv::IMREAD_COLOR);
    cv::Mat imageRight, imageRightRGB = cv::imread(pathRight, cv::IMREAD_COLOR);
    if (imageLeftRGB.empty())
    {
        std::cout << "Fail to read the image:" << pathLeft << std::endl;
        return -1;
    }
    if (imageRightRGB.empty())
    {
        std::cout << "Fail to read the image:" << pathRight << std::endl;
        return -1;
    }
    cv::cvtColor(imageLeftRGB, imageLeft, cv::COLOR_BGR2GRAY);
    cv::cvtColor(imageRightRGB, imageRight, cv::COLOR_BGR2GRAY);

    int matchsize = 9;//���ϵ���������δ��ڵı߳�
    int half_matchsize = matchsize / 2;//�߳���һ��

    std::vector<cv::Point3f> featurePointRight;//��Ƭƥ�䵽������

    float lowst_door = 0.7; //���ϵ����ƥ�����ֵ
    int dist_width = 270;//����Ƭ������Ƭ����Ծ��룬������ͨ���ֶ��۲�

    //����f���ݵ�Ԥ���� ɾ�������Ϲ淶������
    for (size_t i = 0; i < featurePointLeft.size(); i++)
    {
        //����� 5 = half_matchsize + 1
        if ((featurePointLeft.at(i).y + dist_width < imageLeft.cols) || (imageLeft.cols - featurePointLeft.at(i).y < 5))
        {
            featurePointLeft.erase(featurePointLeft.begin() + i);
            i--;
            continue;
        }
        if ((featurePointLeft.at(i).x < 5) || (imageLeft.rows - featurePointLeft.at(i).x < 5))
        {
            featurePointLeft.erase(featurePointLeft.begin() + i);
            i--;
            continue;
        }

    }
    //�����󴰿ڵ�С����
    cv::Mat matchLeftWindow;
    matchLeftWindow.create(matchsize, matchsize, CV_32FC1);
    for (size_t i = 0; i < featurePointLeft.size(); i++)
    {
        float aveLImg = 0;
        for (int m = 0; m < matchsize; m++)
        {
            for (int n = 0; n < matchsize; n++)
            {
                aveLImg += imageLeft.at<uchar>(featurePointLeft.at(i).x - half_matchsize + m, featurePointLeft.at(i).y - half_matchsize + n);
                matchLeftWindow.at<float>(m, n) = imageLeft.at<uchar>(featurePointLeft.at(i).x - half_matchsize + m, featurePointLeft.at(i).y - half_matchsize + n);
            }
        }
        aveLImg = aveLImg / (matchsize * matchsize);//��ȡ�󴰿�ƽ��ֵ
        //����ĳ��ֵ
        for (int m = 0; m < matchsize; m++)
        {
            for (int n = 0; n < matchsize; n++)
            {
                matchLeftWindow.at<float>(m, n) = matchLeftWindow.at<float>(m, n) - aveLImg;
            }
        }
        //***************************���Ҵ��ڽ��м���
        //����Ԥ���Ҵ��ڵ�λ��
        int halflengthsize = 10; //�������İ뾶
        std::vector < cv::Point3f> tempfeatureRightPoint;
        //ȥ���ܵ�������ĵ�
        for (int ii = -halflengthsize; ii <= halflengthsize; ii++)
        {
            for (int jj = -halflengthsize; jj <= halflengthsize; jj++)
            {
                //Ϊ��ʡ�¡��� �ѱ�Ե���޵Ķ�����û��
                if ((featurePointLeft.at(i).x < (halflengthsize + 5)) || (imageRight.rows - featurePointLeft.at(i).x) < (halflengthsize + 5)
                    || (featurePointLeft.at(i).y + dist_width - imageLeft.cols) < (halflengthsize + 5))
                {
                    cv::Point3f temphalflengthsize;
                    temphalflengthsize.x = 0;
                    temphalflengthsize.y = 0;
                    temphalflengthsize.z = 0;
                    tempfeatureRightPoint.push_back(temphalflengthsize);
                }
                else
                {
                    cv::Point3f temphalflengthsize;
                    int x = featurePointLeft.at(i).x + ii - half_matchsize;
                    int y = featurePointLeft.at(i).y + dist_width - imageLeft.cols + jj - half_matchsize;
                    float  coffee = Get_coefficient(matchLeftWindow, imageRight, x, y);
                    temphalflengthsize.x = featurePointLeft.at(i).x + ii;
                    temphalflengthsize.y = featurePointLeft.at(i).y + dist_width - imageLeft.cols + jj;
                    temphalflengthsize.z = coffee;
                    tempfeatureRightPoint.push_back(temphalflengthsize);
                }

            }
        }
        vectorsort(tempfeatureRightPoint);
        //�޳����ϵ��С����ֵ�ĵ�
        if (tempfeatureRightPoint.at(0).z > lowst_door && tempfeatureRightPoint.at(0).z < 1)
        {
            cv::Point3f tempr;
            tempr.x = tempfeatureRightPoint.at(0).x;
            tempr.y = tempfeatureRightPoint.at(0).y;
            tempr.z = tempfeatureRightPoint.at(0).z;
            featurePointRight.push_back(tempr);
        }
        else
        {
            featurePointLeft.erase(featurePointLeft.begin() + i);
            i--;
            continue;
        }
    }
    //չʾ
    lastview(imageLeftRGB, imageRightRGB, featurePointLeft, featurePointRight);
    return 0;
}

void CorrelationMatcher::match(const cv::Mat& srcImg, const cv::Mat& dstImg, const std::vector<cv::Point>& srcPts, std::vector<MatchPointPair>& matches)
{
    matches.clear();

    cv::Mat srcImgCopy, dstImgCopy;
    if (srcImg.channels() != 1)
        cv::cvtColor(srcImg, srcImgCopy, cv::COLOR_BGR2GRAY);
    else
        srcImgCopy = srcImg.clone();

    if (dstImg.channels() != 1)
        cv::cvtColor(dstImg, dstImgCopy, cv::COLOR_BGR2GRAY);
    else
        dstImgCopy = dstImg.clone();

    double r = windowSize / 2;
    int num = 0;

    int total = srcPts.size();
    //ProgressBar progressBar(total, 70, '#', '-');

    for (cv::Point srcPt : srcPts)
    {
        //++progressBar;
        if (isVaildPoint(srcImgCopy, srcPt))
        {
            // ʹ��ROI��ԭͼ���г�һ������
            cv::Rect rectSrc, rectDst;
            cv::Mat windowSrc, windowDst;
            rectSrc = cv::Rect(srcPt.x - r, srcPt.y - r, windowSize, windowSize);
            windowSrc = srcImgCopy(rectSrc);

            // ����Ŀ��ͼ��Ѱ������������ͬ����
            double idx = 0.0, maxIdx = 0.0;
            int maxI, maxJ;
            for (int i = r; i < dstImgCopy.rows - r; i++)
                for (int j = r; j < dstImgCopy.cols - r; j++)
                {
                    rectDst = cv::Rect(j - r, i - r, windowSize, windowSize);
                    windowDst = dstImgCopy(rectDst);
                    idx = computeCorrelationIdx(windowSrc, windowDst);
                    if (idx > maxIdx)
                    {
                        maxIdx = idx;
                        maxI = i;
                        maxJ = j;
                    }
                }
            // �ж��������ϵ���Ƿ������趨��ֵ
            if (maxIdx > threshold)
            {
                MatchPointPair match;
                match.srcPt = srcPt;
                match.dstPt = cv::Point2d(maxJ, maxI);
                match.dist = maxIdx;
                matches.push_back(match);
            }
        }
        //if (num % 10 == 0)
        //{
        //    progressBar.display();
        //}

        num++;
    }
    //progressBar.done();

}

void CorrelationMatcher::matchImproved(const cv::Mat& srcImg, const cv::Mat& dstImg, const std::vector<cv::Point>& srcPts, const std::vector<cv::Point>& dstPts, std::vector<MatchPointPair>& matches)
{
    matches.clear();

    cv::Mat srcImgCopy, dstImgCopy;
    if (srcImg.channels() != 1)
        cv::cvtColor(srcImg, srcImgCopy, cv::COLOR_BGR2GRAY);
    else
        srcImgCopy = srcImg.clone();

    if (dstImg.channels() != 1)
        cv::cvtColor(dstImg, dstImgCopy, cv::COLOR_BGR2GRAY);
    else
        dstImgCopy = dstImg.clone();

    double r = windowSize / 2;
    int num = 0;

    int total = srcPts.size();
    //ProgressBar progressBar(total, 70, '#', '-');

    for (cv::Point srcPt : srcPts)
    {
        //++progressBar;
        if (isVaildPoint(srcImgCopy, srcPt))
        {
            // ʹ��ROI��ԭͼ���г�һ������
            cv::Rect rectSrc, rectDst;
            cv::Mat windowSrc, windowDst;
            rectSrc = cv::Rect(srcPt.x - r, srcPt.y - r, windowSize, windowSize);
            windowSrc = srcImgCopy(rectSrc);

            // ����Ŀ��ͼ��������㣬Ѱ������������ͬ����
            double idx = 0.0, maxIdx = 0.0;
            cv::Point maxPt;
            for (cv::Point dstPt : dstPts)
            {
                if (isVaildPoint(dstImgCopy, dstPt))
                {
                    rectDst = cv::Rect(dstPt.x - r, dstPt.y - r, windowSize, windowSize);
                    windowDst = dstImgCopy(rectDst);
                    idx = computeCorrelationIdx(windowSrc, windowDst);
                    if (idx > maxIdx)
                    {
                        maxIdx = idx;
                        maxPt = dstPt;
                    }
                }
            }
            // �ж��������ϵ���Ƿ������趨��ֵ
            if (maxIdx > threshold)
            {
                MatchPointPair match;
                match.srcPt = srcPt;
                match.dstPt = maxPt;
                match.dist = maxIdx;
                matches.push_back(match);
            }
        }

        //if (num % 10 == 0)
        //{
        //    progressBar.display();
        //}

        num++;
    }
    //progressBar.done();

}
