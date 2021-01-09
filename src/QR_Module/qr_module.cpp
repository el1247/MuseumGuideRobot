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

