#pragma once
#include <fstream>
#include "match_point_pair.h"

namespace basematcher
{
	void save_match_points(const std::string& saveFilename, const std::vector<MatchPointPair>& matchPoints);
	void draw_result_view(cv::Mat& outputImage, const cv::Mat& leftImage, const cv::Mat& rightImage, std::vector<MatchPointPair>& matchPoints);
};

