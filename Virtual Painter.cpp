#include<iostream>
#include<opencv2/imgcodecs.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/objdetect.hpp>
#include<opencv2/highgui.hpp>

using namespace std;
using namespace cv;

vector<vector<int>> myColors{ {72,179,108,179,250,174} };// {72, 102, 75, 116, 255, 75} }; //{99,128,75,113,255,105} };
vector<Scalar> myColorValues{ {139,0,0} };
vector<vector<int>> newPoints;
Point myPoints ( 0,0 );

Mat img; 
VideoCapture cap(0);

Point getContours(Mat imgDil) {
	vector<vector<Point>> contours;
	vector < Vec4i>hierarchy;

	findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	for (int i = 0; i < contours.size(); i++) {
		
		int area = contourArea(contours[i]);

		vector<vector<Point>> conPoly(contours.size());   // approx values will be stored in this.
		vector<Rect> boundRect(contours.size());

		if (area > 1000) {
			float peri = arcLength(contours[i], true);
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);

			cout << conPoly[i].size() << endl;
			boundRect[i] = boundingRect(conPoly[i]);
			myPoints.x = boundRect[i].x + boundRect[i].width / 2;
			myPoints.y = boundRect[i].y;

			//drawContours(img, conPoly, i, Scalar(255, 0, 255), 2);
			rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0));


		}


	}
	return myPoints;
}

void drawOnCanvas(vector<vector<int>> newPoints, vector<Scalar> mycolorValues) {

	for (int i = 0; i < newPoints.size(); i++) {
		circle(img, Point(newPoints[i][0], newPoints[i][1]), 10, myColorValues[newPoints[i][2]], FILLED);

	}
 }
void  findColor(Mat img) {

	Mat imgHSV;
	cvtColor(img, imgHSV, COLOR_BGR2HSV);
	for (int i = 0; i < myColors.size(); i++) {
		Scalar lower(myColors[i][0], myColors[i][1], myColors[i][2]);
		Scalar upper(myColors[i][3], myColors[i][4], myColors[i][5]);
		Mat mask;
		inRange(imgHSV, lower, upper, mask);
		//cout << 1;
		imshow(to_string(i), mask);
		imshow("hi", mask);
		Point myPoints=getContours(mask);
		if (myPoints.x != 0 && myPoints.y != 0) {
			newPoints.push_back({ myPoints.x, myPoints.y ,i });
		}
	}
	return ;

}

int main() {


	while (true) {
		cap.read(img);
		findColor(img);
		drawOnCanvas(newPoints, myColorValues);
		imshow("Image", img);
		waitKey(1);

	}
	return 0;

}
