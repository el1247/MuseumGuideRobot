/* Copyright (C) 2021 Gautam Gupta, Duncan Lowther, Satya Mallick
 *  
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* g++ -std=c++0x qr_module.cpp -oqr_module -lzbar -L/opt/opencv-4.1.0/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_videoio */
//Current linker command^
//make qr-standalone LDFLAGS=-L/opt/opencv-4.1.0/lib && ./qr-standalone
//Current make and run command^
//NOTE: MODIFY IT AS PER OPENCV INSTALLATION

#include <cmath>
#include <iostream>
#include <string>
#include "opencv2/core.hpp"  //Point LD_LIBRARY_PATH to opencv2 folder
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio/videoio_c.h"
#include "qr.hpp"
#include "mapstruct.h"
#include "zbar.h"


using namespace zbar;
using namespace cv;
using namespace std;

/* CHANGEABLE ACCORDING TO CAMERA SENSOR RESOLUTION AND FOV ANGLES */
#define L  0.0575f   /* One-half the side length of the QR code in real life */
#define ZF 2185.5f /* ORIGINAL - 2571.2 Pixel distance between the image of the optical axis and the image of a line 45 degrees away.
                   * Can be caluclated as (linear resolution/(2*tan(FOV angle/2))) */
#define OX 1296    /* X-coordinate of the image of the optical axis */

int decode(Mat &im, qr_Code &qrcode) {
	ImageScanner scanner; //creating qr scanner
	scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 0);
	scanner.set_config(ZBAR_QRCODE, ZBAR_CFG_ENABLE, 1); //configuring the scanner

	// Converting image to grayscale
	Mat imGray;
	cvtColor(im, imGray, COLOR_RGB2GRAY);

	// Convert image to zbar image
	Image image(im.cols, im.rows, "Y800", (uchar *)imGray.data, im.cols * im.rows);

	// Scan the image
	int n = scanner.scan(image);

	for (Image::SymbolIterator symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol){
		// Storing data
		qrcode.data = symbol->get_data();

		// Print type and data
		cout << "Data : " << qrcode.data << endl;

#define Q(i,j,xy) (symbol->get_location_##xy ((i<<1)+((i)^(j)))) /* indexing map 00=>0, 01=>1, 11=>2, 10=>3 */
		float a0 = -1/(float)(Q(0,0,y) - Q(0,1,y));
		float a1 = -1/(float)(Q(1,0,y) - Q(1,1,y));
 		qrcode.dx = L * ((Q(0,0,x) - OX) * a0 + (Q(1,0,x) - OX) * a1);
		qrcode.dy = L * ZF * (a0 + a1);
		qrcode.face = asinf(ZF * (a1 - a0));
	}

	return 0;
}

#ifdef QR_STANDALONE
int main(int argv, char** argc){ //TESTING MAIN FUNCTION, TO BE COMMENTED OUT

	Mat frame; //To store the frames from the video
	//vector<qr_Code> qr_Codes; // Vector to contain all the qr codes scanned
	qr_Code qrcode;
	VideoCapture cap(0); //Camera select, default picam is 0

	if (!cap.isOpened()){
		cerr<< "ERROR!Unable to open Camera\n";
		return -1;
	}

	cap.set(CV_CAP_PROP_FRAME_WIDTH, 2592);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 1944);
	cap.set(CV_CAP_PROP_FPS, 10);

	cout << "Start grabbing"<<endl<<"Press any key to terminate"<<endl;

	for (;;){ // Infinite loop, can be modified to stop the video whenever
		cap.read(frame);
		if (frame.empty()){
			cerr<<"ERROR!Blank frame grabbed\n"; //Error catching in case no camera
			break;
			}
		imshow("Live", frame); // This can be removed to disable video display
		decode(frame, qrcode); // Decodes the frames from the camera
		cout << "(x,y,\u03b8) = (" << qrcode.dx << ", " << qrcode.dy << ", " << qrcode.face << ")" << endl;
		if (waitKey (5) >= 0){
			break;
			}
	}
	return 0;
}
#endif

