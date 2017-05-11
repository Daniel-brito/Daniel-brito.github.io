#include <iostream>
#include <opencv2/opencv.hpp>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <numeric>
#include <ctime>
#include <cstdlib>

using namespace std;
using namespace cv;

int top_slider = 10;
int top_slider_max = 200;
int JITTER = 3;
int JITTER_max = 5;
int RAIO = 3;
int RAIO_max = 8;
int STEP = 5;
int STEP_max = 8;

Mat image, border,cannyponto, points2, points;

void on_trackbar_canny(int, void*){
	int width, height, gray;
	Mat image1, frame;
	vector<int> yrange;
	vector<int> xrange;
	int x, y;
	width = image.size().width;
	height = image.size().height;
	if (STEP < 1){
		STEP = 1;
	}
	if (JITTER < 1){
		JITTER = 1;
	}
	if (RAIO < 1){
		RAIO = 1;
	}
	xrange.resize(height / STEP);
	yrange.resize(width / STEP);
	iota(xrange.begin(), xrange.end(), 0);
	iota(yrange.begin(), yrange.end(), 0);
	for (uint i = 0; i < xrange.size(); i++){
		xrange[i] = xrange[i] * STEP + STEP / 2;
	}

	for (uint i = 0; i < yrange.size(); i++){
		yrange[i] = yrange[i] * STEP + STEP / 2;
	}
	points = Mat(height, width, CV_8U, Scalar(255));
	random_shuffle(xrange.begin(), xrange.end());
	for (auto i : xrange){
		random_shuffle(yrange.begin(), yrange.end());
		for (auto j : yrange){
			x = i + rand() % (2 * JITTER) - JITTER + 1;
			y = j + rand() % (2 * JITTER) - JITTER + 1;
			gray = image.at<uchar>(x, y);
			circle(points,
				cv::Point(y, x),
				RAIO,
				CV_RGB(gray, gray, gray),
				-1,
				CV_AA);
		}
	}

	points2 = points.clone();
	Canny(image, border, top_slider, 3 * top_slider);
	cannyponto = 0.8 * points2 + 0.2 * border;
	imshow("canny", cannyponto);
}

int main(int argc, char**argv){
	
	image = imread("image1.png", CV_LOAD_IMAGE_GRAYSCALE);
	srand(time(0));

	if (!image.data){
		cout << "nao abriu" << endl;
		exit(0);
	}
	while (true){
			namedWindow("canny", 1);
			createTrackbar("Threshold", "canny",
				&top_slider,
				top_slider_max,
				on_trackbar_canny);
			createTrackbar("Jitter", "canny",
				&JITTER,
				JITTER_max,
				on_trackbar_canny);
			createTrackbar("Raio", "canny",
				&RAIO,
				RAIO_max,
				on_trackbar_canny);
			createTrackbar("Step", "canny",
				&STEP,
				STEP_max,
				on_trackbar_canny);

			on_trackbar_canny(top_slider, 0);
			break;
	}
	
	waitKey();
	return 0;
}