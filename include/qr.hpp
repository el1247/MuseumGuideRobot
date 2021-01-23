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

#include <string>
#include <vector>
#include "opencv2/core.hpp"  //These need to be modified as per ZBAR and OPENCV's installation in the system
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "zbar.h"

#ifndef QRMODULE_H
#define QRMODULE_H

int decode(cv::Mat &im);

typedef struct{
	std::string data;
	std::vector <cv::Point> location;
} qr_Code;

#endif
