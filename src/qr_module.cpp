/* Copyright (C) 2021 Gautam Gupta
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
//NOTE: MODIFY IT AS PER OPENCV INSTALLATION

#include <iostream>
#include <string>
#include "opencv2/core.hpp"  //Point LD_LIBRARY_PATH to opencv2 folder
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio/videoio_c.h"
#include "qr.hpp"
#include "zbar.h"


using namespace zbar;
using namespace cv;
using namespace std;

int decode(Mat &im, vector<qr_Code>&qr_Codes) {
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
		// QR code class initialisation
		qr_Code qrcode;

		// Storing 
		qrcode.data = symbol->get_data();

		// Print type and data
		cout << "Data : " << qr_Code.data << endl;

		// Storing the coordinates of the QR codes
		for (int i = 0; i < symbol->get_location_size(); i++){
			qrcode.location.push_back(Point(symbol->get_location_x(i), symbol->get_location_y(i)));
		}

		qr_Codes.push_back(qrcode);
	}

	return 0;
}

int main(int argv, char** argc){

	Mat frame; //To store the image
	vector<qr_Code> qr_Codes;
	VideoCapture cap(0);

	if (!cap.isOpened()){
		cerr<< "ERROR!Unable to open Camera\n";
		return -1;
	}
	cout << "Start grabbing"<<endl<<"Press any key to terminate"<<endl;

	for (;;){
	cap.read(frame);
	if (frame.empty()){
		cerr<<"ERROR!Blank frame grabbed\n";
		break;
		}
	imshow("Live", frame);
	decode(frame, qr_Codes); // Decodes the frames from the camera
	if (waitKey (5) >= 0){
		break;
		}
	}
	return 0;
}
