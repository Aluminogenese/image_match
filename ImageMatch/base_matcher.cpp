#include "pch.h"
#include "base_matcher.h"

void basematcher::save_match_points(const std::string& saveFilename, const std::vector<MatchPointPair>& matchPoints) {
	std::ofstream ofs;
	ofs.open(saveFilename);
	ofs << "left_x(col) " << "left_y(row)\t" << "right_x(col) " << "right_y(row)\t" << "coefficient" << std::endl;
	for (auto& match_point : matchPoints)
	{
		ofs << match_point.leftPt.x << "\t"
			<< match_point.leftPt.y << "\t\t"
			<< match_point.rightPt.x << "\t"
			<< match_point.rightPt.y << "\t\t"
			<< match_point.dist << std::endl;
	}
	ofs.close();
}

void basematcher::draw_result_view(cv::Mat& outputImage, const cv::Mat& leftImage, const cv::Mat& rightImage, std::vector<MatchPointPair>& matchPoints) {
    outputImage.create(leftImage.rows, leftImage.cols + rightImage.cols, leftImage.type());
//#pragma omp parallel for
    for (int i = 0; i < leftImage.rows; i++) {
        const cv::Vec3b* leftPtr = leftImage.ptr<cv::Vec3b>(i);
        cv::Vec3b* outputPtr = outputImage.ptr<cv::Vec3b>(i);
        for (int j = 0; j < leftImage.cols; j++) {
            outputPtr[j] = leftPtr[j];
        }
    }
//#pragma omp parallel for
    for (int i = 0; i < rightImage.rows; i++) {
        const cv::Vec3b* rightPtr = rightImage.ptr<cv::Vec3b>(i);
        cv::Vec3b* outputPtr = outputImage.ptr<cv::Vec3b>(i);
        for (int j = leftImage.cols; j < leftImage.cols + rightImage.cols; j++)
        {
            outputPtr[j] = rightPtr[j];
        }
    }
//#pragma omp parallel for
    for (auto& match_point : matchPoints)
    {
        int a = (rand() % 200);
        int b = (rand() % 200 + 99);
        int c = (rand() % 200) - 50;
        if (a > 100 || a < 0) {
            a = 255;
        }
        if (b > 255 || b < 0) {
            b = 88;
        }
        if (c > 255 || c < 0) {
            c = 188;
        }
        int radius = 5;
        //左片
        int left_col = int(match_point.leftPt.x);
        int left_row = int(match_point.leftPt.y);

        cv::circle(outputImage, cv::Point(left_col, left_row), radius, cv::Scalar(0, 255, 255), 1, 4, 0);
        cv::line(outputImage, cv::Point(left_col - radius - 2, left_row), cv::Point(left_col + radius + 2, left_row), cv::Scalar(0, 255, 255), 1, 8, 0);
        cv::line(outputImage, cv::Point(left_col, left_row - radius - 2), cv::Point(left_col, left_row + radius + 2), cv::Scalar(0, 255, 255), 1, 8, 0);

        //右片
        int right_col = int(match_point.rightPt.x + leftImage.cols);
        int right_row = int(match_point.rightPt.y);

        cv::circle(outputImage, cv::Point(right_col, right_row), radius, cv::Scalar(0, 255, 255), 1, 4, 0);
        cv::line(outputImage, cv::Point(right_col - radius - 2, right_row), cv::Point(right_col + radius + 2, right_row), cv::Scalar(0, 255, 255), 1, 8, 0);
        cv::line(outputImage, cv::Point(right_col, right_row - radius - 2), cv::Point(right_col, right_row + radius + 2), cv::Scalar(0, 255, 255), 1, 8, 0);
        //连接
        cv::line(outputImage, cv::Point(left_col, left_row), cv::Point(right_col, right_row), cv::Scalar(a, b, c), 1, 8, 0);
    }

    cv::imshow("左右片影像同名点展示", outputImage);
    cv::waitKey(0);
}