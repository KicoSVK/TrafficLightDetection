#include "opencv2/opencv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

vector<Vec3f> circles;

int getIndexOfColorFromMat(vector<vector<vector<Point>>> contours) {

	vector<int> largestContourAreaForRGBOMat;

	for (size_t i = 0; i < contours.size(); i++)
	{
		int largestContourArea = 0;
		for (size_t j = 0; j < contours.at(i).size(); j++)
		{
			//drawContours(drawing, contours, (int)i, Scalar(0, 255, 0), 2, LINE_8, hierarchy, 0);
			double area = contourArea(contours.at(i)[j]);

			if (area > largestContourArea)
			{
				largestContourArea = area;
			}
		}
		largestContourAreaForRGBOMat.push_back(largestContourArea);
	}

	int largestAreaOfRGBOMat = *max_element(largestContourAreaForRGBOMat.begin(), largestContourAreaForRGBOMat.end());

	for (size_t i = 0; i < largestContourAreaForRGBOMat.size(); i++)
	{
		if (largestAreaOfRGBOMat > 5 && largestAreaOfRGBOMat == largestContourAreaForRGBOMat.at(i))
			return i;
	}
	return 99;
}

int getTrafficLightIndexColorFromMat(Mat redMat, Mat greenMat, Mat blueMat, Mat orangeMat) {

	vector<vector<Point>> contoursInRedMat, contoursInGreenMat, contoursInBlueMat, contoursInOrangeMat;
	vector<Vec4i> hierarchy;

	findContours(redMat, contoursInRedMat, hierarchy, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	findContours(greenMat, contoursInGreenMat, hierarchy, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	findContours(blueMat, contoursInBlueMat, hierarchy, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	findContours(orangeMat, contoursInOrangeMat, hierarchy, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	vector<vector<vector<Point>>> contours;
	contours.push_back(contoursInRedMat);
	contours.push_back(contoursInGreenMat);
	contours.push_back(contoursInBlueMat);
	contours.push_back(contoursInOrangeMat);

	return getIndexOfColorFromMat(contours);
}

void main() {

	for (int i = 1; i < 29; i++)
	{
		Mat frame, output, gray;
		Mat sobelX, sobelY, sobelSUM;

		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;

		//string path = "Testing dataset\\" + std::to_string(i) + ".png";
		string path = "Testing dataset\\ds01\\" + std::to_string(i) + ".png";

		frame = imread(path);

		/*cvtColor(frame, gray, COLOR_BGR2GRAY);
		//Sobel(gray, sobelX, CV_64F, 1, 0, 3);
		//Sobel(gray, sobelY, CV_64F, 0, 1, 3);
		//add(sobelX, sobelY, sobelSUM);
		//normalize(sobelSUM, sobelSUM, 0, 255, CV_MINMAX);
		Laplacian(gray, sobelSUM, CV_64F, 3);
		//normalize(sobelSUM, sobelSUM, 0, 1, CV_MINMAX);
		convertScaleAbs(sobelSUM, sobelSUM);
		threshold(sobelSUM, sobelSUM, 50, 255, THRESH_BINARY);


		findContours(sobelSUM, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_NONE);
		drawContours(frame, contours, 0, Scalar(0,0,255), 2, 8, hierarchy, 0);*/

		//rectangle(frame, Point((frame.cols / 10 * 1), frame.cols / 10 * 1), Point(frame.cols - (frame.cols / 10 * 1), frame.rows - (frame.cols / 10 * 1)), Scalar(0, 0, 255), 4);

		//rectangle(frame, Point(0, frame.rows / 10 * 3), Point(frame.cols, frame.rows - frame.rows / 10 * 3), Scalar(255, 0, 255), 4);

		cvtColor(frame, output, COLOR_BGR2HSV);
		//resize(output, output, INTER_C);
		medianBlur(output, output, 9);

		Mat redMat1, redMat2, redMat3, redMat4, redMat5, redMat, blueMat, blueMat1, blueMat2, lowerBlueMat, upperBlueMat, orangeMat, orangeMat1, orangeMat2, orangeMat3, orangeMat4, orangeMat5, orangeMat6, greenMat, greenMat1, greenMat2;

		cv::inRange(output, cv::Scalar(50, 40, 110), cv::Scalar(85, 60, 180), redMat1); //
		cv::inRange(output, cv::Scalar(0, 180, 230), cv::Scalar(20, 255, 255), redMat2); //
		cv::inRange(output, cv::Scalar(170, 140, 100), cv::Scalar(175, 235, 255), redMat3); //
		cv::inRange(output, cv::Scalar(0, 215, 140), cv::Scalar(5, 255, 215), redMat4); //
		cv::inRange(output, cv::Scalar(170, 175, 230), cv::Scalar(185, 210, 255), redMat5); //
		redMat = redMat1 + redMat2 + redMat3 + redMat4 + redMat5;

		cv::inRange(output, cv::Scalar(75, 90, 40), cv::Scalar(100, 235, 255), greenMat1);
		cv::inRange(output, cv::Scalar(235, 235, 10), cv::Scalar(255, 255, 60), greenMat2);
		greenMat = greenMat1 + greenMat2;

		cv::inRange(output, cv::Scalar(0, 0, 180), cv::Scalar(20, 30, 255), blueMat1);
		cv::inRange(output, cv::Scalar(215, 220, 200), cv::Scalar(255, 255, 255), blueMat2);
		blueMat = blueMat1 + blueMat2;


		cv::inRange(output, cv::Scalar(10, 210, 240), cv::Scalar(45, 240, 255), orangeMat1);
		cv::inRange(output, cv::Scalar(0, 65, 65), cv::Scalar(25, 100, 140), orangeMat2);
		cv::inRange(output, cv::Scalar(0, 245, 245), cv::Scalar(35, 255, 255), orangeMat3);
		cv::inRange(output, cv::Scalar(30, 0, 250), cv::Scalar(130, 10, 255), orangeMat4);
		cv::inRange(output, cv::Scalar(130, 0, 250), cv::Scalar(150, 5, 255), orangeMat5);
		cv::inRange(output, cv::Scalar(5, 130, 110), cv::Scalar(15, 200, 180), orangeMat6);
		orangeMat = orangeMat1 + orangeMat1 + orangeMat3 + orangeMat4 + orangeMat5 + orangeMat6;

		int morphType = cv::MORPH_CROSS;
		int erosionSize = 2;
		int dilateSize = 2;

		Mat elementForErode = getStructuringElement(morphType,
			Size(2 * erosionSize + 1, 2 * erosionSize + 1),
			Point(erosionSize, erosionSize));

		Mat elementForDilate = getStructuringElement(MORPH_ELLIPSE,
			Size(2 * dilateSize + 1, 2 * dilateSize + 1),
			Point(dilateSize, dilateSize));

		cv::erode(redMat, redMat, elementForErode);
		cv::dilate(redMat, redMat, elementForDilate);

		cv::erode(greenMat, greenMat, elementForErode);
		cv::dilate(greenMat, greenMat, elementForDilate);

		cv::erode(blueMat, blueMat, elementForErode);
		cv::dilate(blueMat, blueMat, elementForDilate);

		cv::erode(orangeMat, orangeMat, elementForErode);
		cv::dilate(orangeMat, orangeMat, elementForDilate);

		system("CLS");
		switch (getTrafficLightIndexColorFromMat(redMat, greenMat, blueMat, orangeMat))
		{
		case 0:
			cout << "CERVENA";
			break;
		case 1:
			cout << "ZELENA";
			break;
		case 2:
			cout << "BLUE";
			break;
		case 3:
			cout << "ORANZOVA";
			break;

		default:
			cout << "ZIADNA";
			break;
		}


		destroyWindow("Frame");
		//destroyWindow("red");
		//destroyWindow("green");
		//destroyWindow("blue");
		//destroyWindow("orange");
		imshow("Frame", frame);
		//imshow("red", redMat);
		//imshow("green", greenMat);
		//imshow("blue", blueMat);
		//imshow("orange", orangeMat);

		waitKey();
	}
}