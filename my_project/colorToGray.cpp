void colorToGray(cv::Mat image)
{
	cv::cvtColor(image, image, CV_BGR2GRAY);
}