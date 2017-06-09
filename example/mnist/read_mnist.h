
#ifndef __READ_MNIST_H__
#define __READ_MNIST_H__

#include <string>
#include <vector>
#include <opencv/cv.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

void MNIST_read_images(string filename, vector<Mat> &vec);
void MNIST_read_labels(string filename, vector<double> &vec);

Rect MNIST_find_digit_area(Mat *input);
void MNIST_preprocess_images(vector<Mat> &images);

#endif
