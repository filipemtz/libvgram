
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <wnn/VgRamNeuron.h>
#include "read_mnist.h"


using namespace std;
using namespace cv;


class MnistOutput : public VgRamNeuronOutput
{
	public:
		int mnist_class;
		MnistOutput(int mnist_class_p) { mnist_class = mnist_class_p; }
};


BitPattern*
bit_pattern_from_image(Mat &m)
{
	int image_size = m.rows * m.cols * m.channels();
	BitPattern *b = new BitPattern(image_size);

	for (int i = 0; i < image_size; i++)
	{
		if (m.type() == CV_32FC1)
		{
			if (m.data[i] > 0)
				b->set(i);
		}
		else if (m.type() == CV_8UC1)
		{
			if (m.data[i] > 0)
				b->set(i);
		}
		else
			printf("Error: Invalid type of image\n");
	}

	return b;
}


int
summarize(VgRamNeuronResult &r)
{
	vector<int> classes;
	vector<long> frequency;

	for (int i = 0; i < r.values.size(); i++)
	{
		int found = 0;

		for (int j = 0; j < classes.size(); j++)
		{
			if (classes[j] == ((MnistOutput*) r.values[i])->mnist_class)
			{
				frequency[j] += 1;
				found = 1;
				break;
			}
		}

		if (!found)
		{
			classes.push_back(((MnistOutput*) r.values[i])->mnist_class);
			frequency.push_back(1);
		}
	}

	double max_frequency = -DBL_MAX;
	vector<int> max_freq_ids;

	for (int j = 0; j < classes.size(); j++)
	{
		if (frequency[j] > max_frequency)
		{
			max_frequency = frequency[j];
			max_freq_ids.clear();
			max_freq_ids.push_back(j);
		}
		else if (frequency[j] == max_frequency)
		{
			max_freq_ids.push_back(j);
		}
	}

	if (max_freq_ids.size() == 0)
		return 0;

	return classes[max_freq_ids[rand() % max_freq_ids.size()]];
}


int
main(int argc, char **argv)
{
	if (argc < 5)
		exit(printf("Use %s <train-images-file> <train-labels-file> <test-images-file> <test-labels-file>\n", argv[0]));

	int i, n, random_sample;
	vector<Mat> train_images, test_images;
	vector<double> train_labels, test_labels;

	MNIST_read_images(argv[1], train_images);
	MNIST_read_labels(argv[2], train_labels);
	MNIST_read_images(argv[3], test_images);
	MNIST_read_labels(argv[4], test_labels);

	srand(time(NULL));

	VgRamNeuronConfig config;

	config.memory_size = 6000;
	config.replacement = ReplacementStrategyNearest;

	VgRamNeuron *neuron = new VgRamNeuron(config);

	n = 6000;

	for (i = 0; i < n; i++)
	{
		if (i % 500 == 0)
			printf("Training %d of %d\n", i, n);

		random_sample = rand() % train_images.size();

		BitPattern* pattern = bit_pattern_from_image(train_images[random_sample]);
		MnistOutput* label = new MnistOutput(train_labels[random_sample]);

		neuron->train(pattern, label);
	}

	int num_hits = 0;

	n = 1000;

	for (i = 0; i < n; i++)
	{
		random_sample = rand() % test_images.size();

		BitPattern *b = bit_pattern_from_image(test_images[random_sample]);
		VgRamNeuronResult r = neuron->nearests(b);

		int c = summarize(r);

		if (c == test_labels[random_sample]) 
			num_hits++;

		if (i % 100 == 0)
		{
			printf("Testing %d of %d | Expected: %.0lf \t Predicted: %d \t Hit: %d Hit rate: %.2lf %%\n",
					i, n, test_labels[i], c, (c == test_labels[i]),
					100.0 * ((double) num_hits / (double) (i + 1)));
		}

		delete(b);
	}

	printf("Final Hit rate: %.2lf %%\n", 100.0 * ((double) num_hits / (double) n));

	return 0;
}
