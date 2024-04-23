#include "pch.h"
#include "base_matcher.h"
#include <random>

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
    outputImage.create(cv::Size(leftImage.cols + rightImage.cols, std::max(leftImage.rows, rightImage.rows)), CV_8UC3);

    leftImage.copyTo(outputImage(cv::Rect(0, 0, leftImage.cols, leftImage.rows)));
    rightImage.copyTo(outputImage(cv::Rect(leftImage.cols, 0, rightImage.cols, rightImage.rows)));

    cv::Point pt1, pt2;
    static std::default_random_engine e;
    static std::uniform_int_distribution<int> u(0, 255);

    for (auto& match : matchPoints)
    {
        cv::Scalar color(u(e), u(e), u(e));

        pt1 = match.leftPt;
        pt2 = cv::Point(match.rightPt.x + leftImage.cols, match.rightPt.y);

        cv::circle(outputImage, pt1, 5, color, 2);
        cv::circle(outputImage, pt2, 5, color, 2);
        cv::line(outputImage, pt1, pt2, color, 2);
    }
}