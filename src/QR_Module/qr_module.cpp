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

#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "ZBar/include/zbar.h"

using namespace cv;
using namespace zbar;

int decode(Mat &im) {
	ImageScanner scanner; //creating qr scanner
	scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 0);
	scanner.set_config(ZBAR_QRCODE, ZBAR_CFG_ENABLE, 1); //configuring the scanner
	
	// Converting image to grayscale
	Mat imGray;
	cvtColor(im, imGray, CV_BGR2GRAY);

	// Convert image to zbar image
	Image image(im.cols, im.rows, "Y800", (uchar *)imGray.data, im.cols * im.rows);

	// Scan the image
	int n = scanner.scan(image);

	for (Image::SymbolIterator symbol = image.symbol_begin(); symbol != image.symbol_end(); ++symbol){
		string data = symbol->get_data();

		// Print type and data
		std::cout << "Data : " << data << std::endl;
		//decodedObjects.push_back(obj);
	}

	return n;
}

