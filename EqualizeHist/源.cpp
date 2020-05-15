
#include<opencv2\opencv.hpp>
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

void histogram_calculation(const Mat &Image, Mat &histoImage)
{

	int histSize = 255;

	float range[] = { 0, 256 };
	const float* histRange = { range };

	bool uniform = true;
	bool accumulate = false;

	Mat b_hist, g_hist, r_hist;

	vector<Mat>  bgr_planes;
	split(Image, bgr_planes);

	calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize,
		&histRange, uniform, accumulate);
	calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize,
		&histRange, uniform, accumulate);
	calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize,
		&histRange, uniform, accumulate);

	int hist_w = 512;
	int hist_h = 400;

	int bin_w = cvRound((float)hist_w / (float)histSize);

	// Create image with gray base
	Mat histImage(hist_h, hist_w, CV_8UC3, Scalar(0, 0, 0));

	// Normalize the histograms to height of image
	normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	for (int i = 1; i< histSize; i++) {
		line(histImage,
			Point(bin_w*(i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(b_hist.at<float>(i))),
			Scalar(255, 0, 0), 2, 8, 0
		);
		line(histImage,
			Point(bin_w*(i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(g_hist.at<float>(i))),
			Scalar(0, 255, 0), 2, 8, 0
		);
		line(histImage,
			Point(bin_w*(i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(r_hist.at<float>(i))),
			Scalar(0, 0, 255), 2, 8, 0
		);
	}


	histoImage = histImage;

}

int main()
{
	Mat src, imageq;
	Mat histImage;

	//get src image
	src = imread("D:\\timg.jpg");
	

	vector<Mat> bgr;
	split(src, bgr);

	Size src_s = src.size();

	imshow("Source image", src);

	histogram_calculation(src, histImage);

	//equalize image
	equalizeHist(bgr[0], bgr[0]);
	equalizeHist(bgr[1], bgr[1]);
	equalizeHist(bgr[2], bgr[2]);

	merge(bgr, imageq);
	imshow("Equalized image", imageq);

	waitKey(0);

}
