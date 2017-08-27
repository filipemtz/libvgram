
#include "read_mnist.h"

#include <math.h>
#include <iostream>
#include <fstream>
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;


int
MNIST_reverse_int(int i)
{
	unsigned char ch1, ch2, ch3, ch4;

	ch1 = i & 255;
	ch2 = (i >> 8) & 255;
	ch3 = (i >> 16) & 255;
	ch4 = (i >> 24) & 255;

	return((int) ch1 << 24) + ((int)ch2 << 16) + ((int)ch3 << 8) + ch4;
}


void
MNIST_read_images(string filename, vector<cv::Mat> &vec)
{
	vec.clear();
	ifstream file (filename.c_str(), ios::binary);

	Mat eroded, dilated;
	Mat morph_kernel = getStructuringElement(CV_SHAPE_RECT, Size(3, 3));

	if (file.is_open())
	{
		int magic_number = 0;
		int number_of_images = 0;
		int n_rows = 0;
		int n_cols = 0;

		file.read((char*) &magic_number, sizeof(magic_number));
		magic_number = MNIST_reverse_int(magic_number);

		file.read((char*) &number_of_images,sizeof(number_of_images));
		number_of_images = MNIST_reverse_int(number_of_images);

		file.read((char*) &n_rows, sizeof(n_rows));
		n_rows = MNIST_reverse_int(n_rows);

		file.read((char*) &n_cols, sizeof(n_cols));
		n_cols = MNIST_reverse_int(n_cols);

		for(int i = 0; i < number_of_images; ++i)
		{
			cv::Mat tp = Mat::zeros(n_rows, n_cols, CV_8UC1);

			for(int r = 0; r < n_rows; ++r)
			{
				for(int c = 0; c < n_cols; ++c)
				{
					char temp = 0;
					file.read(&temp, sizeof(temp));
					unsigned char bla = temp;
					//tp.at<uchar>(r, c) = (int) temp;
					if (bla > 50) tp.at<uchar>(r, c) = (unsigned char) 255;
					else tp.at<uchar>(r, c) = (unsigned char) 0;
				}
			}

			//cv::erode(tp, eroded, morph_kernel);
			////cv::dilate(eroded, dilated, morph_kernel);
			//dilated = eroded;

			//printf("%d\n", i);
			//imshow("tp", tp);
			//imshow("croped", dilated);
			//waitKey(-1);

			vec.push_back(tp.clone());
		}
	}
}


void
MNIST_read_labels(string filename, vector<double> &vec)
{
	vec.clear();
	ifstream file (filename.c_str(), ios::binary);

	if (file.is_open())
	{
		int magic_number = 0;
		int number_of_images = 0;

		file.read((char*) &magic_number, sizeof(magic_number));
		magic_number = MNIST_reverse_int(magic_number);

		file.read((char*) &number_of_images,sizeof(number_of_images));
		number_of_images = MNIST_reverse_int(number_of_images);

		for(int i = 0; i < number_of_images; ++i)
		{
			unsigned char temp = 0;
			file.read((char*) &temp, sizeof(temp));
			vec.push_back((double) temp);
		}
	}
}


Rect
MNIST_find_digit_area(Mat *input)
{
	Rect r;
	int i, j, left, right, bottom, top, found;

	left = right = -1;
	top = bottom = -1;

	// find left limit
	found = 0;
	for (j = 0; j < input->cols; j++)
	{
		for (i = 0; i < input->rows; i++)
		{
			uchar pixel = input->data[i * input->cols + j];
			uchar th = 0;
			if (pixel > th)
			{
				left = j;
				found = 1;
				break;
			}
		}

		if (found)
			break;
	}

	// find right limit
	found = 0;
	for (j = (input->cols - 1); j >= 0; j--)
	{
		for (i = 0; i < input->rows; i++)
		{
			uchar pixel = input->data[i * input->cols + j];
			uchar th = 0;
			if (pixel > th)
			{
				right = j;
				found = 1;
				break;
			}
		}

		if (found)
			break;
	}

	// find top limit
	found = 0;
	for (i = 0; i < input->rows; i++)
	{
		for (j = 0; j < input->cols; j++)
		{
			uchar pixel = input->data[i * input->cols + j];
			uchar th = 0;
			if (pixel > th)
			{
				top = i;
				found = 1;
				break;
			}
		}

		if (found)
			break;
	}

	// find bottom limit
	found = 0;
	for (i = (input->rows - 1); i >= 0; i--)
	{
		for (j = 0; j < input->cols; j++)
		{
			uchar pixel = input->data[i * input->cols + j];
			uchar th = 0;
			if (pixel > th)
			{
				bottom = i;
				found = 1;
				break;
			}
		}

		if (found)
			break;
	}

	r.x = left;
	r.width = (right - left) + 1;

	r.y = top;
	r.height = (bottom - top) + 1;

	return r;
}


void
MNIST_preprocess_images(vector<Mat> &images)
{
	for (int i = 0; i < images.size(); i++)
	{
		Mat img_float;

		//Rect area = MNIST_find_digit_area(&(images[i]));
		//Mat croped = images[i].clone();
		//resize(images[i](area), croped, croped.size(), 0, 0, INTER_LINEAR);

		images[i].convertTo(img_float, CV_32FC1);
		images[i] = img_float;
	}
}

/*
// TEST FUNCTION
int main()
{
    string filename = "../data/MNIST_t10k-images.idx3-ubyte";
    int number_of_images = 10000;
    int image_size = 28 * 28;

    //read MNIST iamge into OpenCV Mat vector
    vector<cv::Mat> vec;
    read_Mnist(filename, vec);
    cout<<vec.size()<<endl;

    filename = "../data/MNIST_t10k-labels.idx1-ubyte";

    //read MNIST label into double vector
    vector<double> labels(number_of_images);
    read_Mnist_Label(filename, labels);
    cout<<labels.size()<<endl;

    for (int i = 0; i < vec.size(); i++)
    {
    	cout<<labels[i]<<endl;
		imshow("1st", vec[i]);
		waitKey();
    }

    return 0;
}
 */
