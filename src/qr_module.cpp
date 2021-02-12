/* Copyright (C) 2021 Gautam Gupta, Duncan Lowther
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

#include <cmath>
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

/* CHANGEABLE ACCORDING TO CAMERA SENSOR RESOLUTION AND FOV ANGLES */
#define L  0.035f   /* One-half the side length of the QR code in real life */
#define ZF 2571.2f /* Pixel distance between the image of the optical axis and the image of a line 45 degrees away.
                   * Can be caluclated as (linear resolution/(2*tan(FOV angle/2))) */
#define OX 1296    /* X-coordinate of the image of the optical axis */

static int find_perspective(int*,int*,int*,int,int,int,int,int,int,int,int,int,int);

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
		// QR code class initialisation
		//qr_Code qrcode;

		// Storing 
		qrcode.data = symbol->get_data();

		// Print type and data
		cout << "Data : " << qrcode.data << endl;

		// Storing the coordinates of the QR codes - LEGACY, dont need this anymore
		for (int i = 0; i < symbol->get_location_size(); i++){
			qrcode.location.push_back(Point(symbol->get_location_x(i), symbol->get_location_y(i)));
			//cout << "QR code corners -" << qrcode.location[i] << endl; //Can include this for debugging
		}
#define Q(i,j,xy) (symbol->get_location_##xy ((i<<1)+((i)^(j)))) /* indexing map 00=>0, 01=>1, 11=>2, 10=>3 */
#ifndef SIMPLE_PERSPECTIVE
		float a0 = -1/(float)(Q(0,0,y) - Q(0,1,y));
		float a1 = -1/(float)(Q(1,0,y) - Q(1,1,y));
 		qrcode.dx = L * ((Q(0,0,x) - OX) * a0 + (Q(1,0,x) - OX) * a1);
		qrcode.dy = L * ZF * (a0 + a1);
		qrcode.face = asinf(ZF * (a1 - a0));
		//qr_Codes.push_back(qrcode);
#else
#define Qdot(i0,j0,i1,j1) ((Q(i0,j0,x) - OX)*(Q(i1,j1,x) - OX) + (Q(i0,j0,y) - OY)*(Q(i1,j1,y) - OY) + ZF*ZF)
		double r00, r01, r10, r11;
		if(find_range_ratios(&r00, &r01, &r10, &r11, Qdot(0,0,0,0), Qdot(0,0,0,1), Qdot(0,0,1,0), Qdot(0,0,1,1),
			Qdot(1,0,1,0), Qdot(0,1,0,1), Qdot(0,1,1,0), Qdot(0,1,1,1), Qdot(1,1,1,0), Qdot(1,1,1,1))) {
			/* TODO ERROR */
		}
		qrcode.dx = (r00*(Q(0,0,x) - OX) + r11*(Q(1,1,x) - OX))/2;
		qrcode.dy = (r00 + r11)/2;
		qrcode.face = asinf((r11 - r00)/(2*L));
#endif
	}

	return 0;
}

/* This code is a huge hack based on calculations done during 3am mania. r00, etc., are the ranges such that Pij=rij*Qij
 * where Pij is the point in real space and Qij is the point (x,1,z) where x and z are the normalised x and y pixel
 * coordinates. Solves a cubic in s01 (the ratio r01/r00) and checks which positive, real root best fits a related but
 * independent quartic in s01. From this the ratios sij = rij/r00 can be computed, and then the absolute ranges can be
 * found by fixing |P01-P00| = 2L. The input parameters q0 to q9 are the dot products between pairs of Qij. This
 * algorithm takes into account all six possible degrees of freedom for the object but I'm still not sure how much we
 * should trust it. */
static int find_range_ratios(r00, r01, r10, r11, q0, q1, q2, q3, q4, q5, q6, q7, q8, q9) {
	double a[4], s01, s10, s11, d;
	struct {double dr,di;} z[3];
	int nc4 = q1*q8, k4 = q5*q9 - q7*q7;
	a[3] = k4 * (q3*q6*q5 + q7*q1*q1*q4 + q6*q1*q1*q8 + q6*q0*q5*q8 + q2*q1*q5*q8 - q6*q7*q1*q2)
	    + nc4 * (q7*q7*q1*q6 - 2*q6*q5*q1*q9 - q2*q5*q5*q9 - q7*q5*q1*q8 + q7*q7*q6*q1 + q7*q7*q2*q5 + q3*q6*q5*q7);
	a[2] = k4 * (-q3*q6*q1 - q3*q2*q5 - 2*q7*q0*q1*q4 - q3*q1*q1*q4 - q6*q0*q1*q8 - q2*q1*q1*q8 - q2*q0*q5*q8
	             + q6*q7*q0*q2 + q6*q3*q1*q2 + q2*q2*q7*q1)
	    + nc4 * (-q7*q7*q0*q6 - 2*q7*q3*q1*q6 + q6*q1*q1*q9 + 2*q2*q5*q1*q9 + q7*q5*q0*q8 + q7*q1*q1*q8
	             + q3*q5*q1*q8 - q7*q7*q2*q1 - q3*q6*q1*q7 - q3*q2*q5*q7);
	a[1] = k4 * (q2*q1*q3 + q7*q0*q0*q4 + 2*q3*q0*q1*q4 + q2*q0*q1*q8 - q6*q3*q0*q2 - q2*q2*q7*q0 - q2*q2*q3*q1)
	    + nc4 * (2*q7*q3*q0*q6 + q3*q3*q1*q6 - q2*q1*q1*q9 - q7*q1*q0*q8 - q3*q5*q0*q8 - q3*q1*q1*q8 + q3*q2*q1*q7);
	a[0] = k4 * (q2*q2*q3*q0 - q3*q0*q0*q4) + nc4 * (q3*q1*q0*q8 - q3*q3*q0*q6);
	__polynomialroots_MOD_cubicroots(&a, &z); /* For real roots, z0 < z1 < z2. Else, z0 real; z1, z2 complex */
	if(z[1].di != 0.0) { /* Only z[0] is real -- no need to check others */
	       	if(z[0].dr <= 0.0) return -1; /* But z[0] must be positive; if not, something has gone wrong. */
		s01 = z[0].dr; 
	} else { /* Three real roots: z[0] <= z[1] <= z[2] -- we need to find the "proper" solution. */
		if(z[2].dr <= 0.0) return -1; /* If z[2] is not positive, no root is; thus, something has gone wrong */
		if((z[1].dr <= 0.0)||((z[1].dr==z[2].dr)&&((z[0].dr<=0.0)||(z[0].dr==z[1].dr)))) {
			s01 = z[2].dr; /* Select the only positive root */
		} else { /* At least two distinct real and positive roots exist -- find the best fit of the quartic */
			double er, erc;
			s01 = z[2].dr;
			er = fabs((q2*q2*q3*q0 - q3*q0*q0*q4) + (q2*q1*q3 + q7*q0*q0*q4 + 2*q3*q0*q1*q4 + q2*q0*q1*q8
					- q6*q3*q0*q2 - q2*q2*q7*q0 - q2*q2*q3*q1) * s01
				+ (-q3*q6*q1 - q3*q2*q5 - 2*q7*q0*q1*q4 - q3*q1*q1*q4 - q6*q0*q1*q8 - q2*q1*q1*q8
						- q2*q0*q5*q8 + q6*q7*q0*q2 + q6*q3*q1*q2 + q2*q2*q7*q1) * s01*s01
				+ (q3*q6*q5 + q7*q1*q1*q4 + q6*q1*q1*q8 + q6*q0*q5*q8 + q2*q1*q5*q8 - q6*q7*q1*q2) * s01*s01*s01
				-q1*q5*q6*q8*s01*s01*s01*s01);
			erc = fabs((q2*q2*q3*q0 - q3*q0*q0*q4) + (q2*q1*q3 + q7*q0*q0*q4 + 2*q3*q0*q1*q4 + q2*q0*q1*q8
					- q6*q3*q0*q2 - q2*q2*q7*q0 - q2*q2*q3*q1) * z[1].dr
				+ (-q3*q6*q1 - q3*q2*q5 - 2*q7*q0*q1*q4 - q3*q1*q1*q4 - q6*q0*q1*q8 - q2*q1*q1*q8
						- q2*q0*q5*q8 + q6*q7*q0*q2 + q6*q3*q1*q2 + q2*q2*q7*q1)*z[1].dr*z[1].dr
				+ (q3*q6*q5 + q7*q1*q1*q4 + q6*q1*q1*q8 + q6*q0*q5*q8 + q2*q1*q5*q8 - q6*q7*q1*q2)*z[1].dr*z[1].dr*z[1].dr
				-q1*q5*q6*q8*z[1].dr*z[1].dr*z[1].dr*z[1].dr);
			if(erc < er) { er = erc; s01 = z[1].dr; }
			if(z[0] > 0.0) {
				erc = fabs((q2*q2*q3*q0 - q3*q0*q0*q4) + (q2*q1*q3 + q7*q0*q0*q4 + 2*q3*q0*q1*q4
							+ q2*q0*q1*q8 - q6*q3*q0*q2 - q2*q2*q7*q0 - q2*q2*q3*q1)
						* z[1].dr
					+ (-q3*q6*q1 - q3*q2*q5 - 2*q7*q0*q1*q4 - q3*q1*q1*q4 - q6*q0*q1*q8
						- q2*q1*q1*q8 - q2*q0*q5*q8 + q6*q7*q0*q2 + q6*q3*q1*q2 + q2*q2*q7*q1)*z[1].dr*z[1].dr
					+ (q3*q6*q5 + q7*q1*q1*q4 + q6*q1*q1*q8 + q6*q0*q5*q8 + q2*q1*q5*q8
						- q6*q7*q1*q2)*z[1].dr*z[1].dr*z[1].dr
					-q1*q5*q6*q8*z[1].dr*z[1].dr*z[1].dr*z[1].dr);
				if(erc < er) s01 = z[0].dr;
			}
		}
	}
	s10 = (s01*q1 - q0) / (s01*q6 - q2), s11 = s01 * (s01*q5 - q1) / (s01*q7 - q3); /* Distance ratios */
	d = (2*L) / (q0 - 2*q1*s01 + s01*s01*q5);
	*r00 = d, *r01 = d*s01, *r10 = d*s10, *r11 = d*s11;
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

