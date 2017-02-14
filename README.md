# libwnn 

A [weightless neural networks (WNN)](http://ieeexplore.ieee.org/document/593179/) library. 

The networks currently available are:
* Virtual Generalized RAM (VG-RAM).

# Requirements

There are not requirements to compile the library core.

To compile the mnist example, however, OpenCV is required.

The library was only tested in Ubuntu 12 and 14 so far.

# Building the lib

```
mkdir build
cd build
cmake ..
make -j 4
sudo make install
```

# Testing your installation

* Example vgram_xor

```
./vgram_xor
```

* Example vgram_mnist

Download the mnist dataset files from http://yann.lecun.com/exdb/mnist/ and then run the program using the mnist files as argument:
  
```
./vgram_mnist train-images-idx3-ubyte train-labels-idx1-ubyte t10k-images-idx3-ubyte t10k-labels-idx1-ubyte
```

It is expected an accuracy between 93% and 95%.


# VG-RAM

A VG-RAM neuron stores its knowledge as a set of (_key_, _value_) relations, where _key_ is a bit pattern, and _value_ is an associated output (for example, a label). We refer to this set as the neuron memory. The procedure to train a new sample is quite simple, and consists of adding a new relation in the set. To retrive an output given an input, it is necessary to look in the neuron memory for the pattern with smaller Hamming distance in relation to the input, and return its associated value. 

It is possible to limit the number of patterns in the neuron memory. When the memory is fully occupied, and a new pattern needs to be trained, several replacement strategies can be used. Our lib provide two: replacement of the nearest pattern (slower, but tends to retain information), and replacement of a random pattern (faster, but may lead to information loss).

When testing a pattern, more than one pattern from the neural memory can have minimal distance in relation to the input. In this case, a summarization strategy (chosen and implemented by the user) is necessary to define which value will be returned. Common strategies are select the most voted output, and select a random a random sample from the nearest patterns, and return its value.

It can be seen that the vgram testing algorithm is basically a K-nearest neighbor, with K being the number of patterns with the smallest distance to the input.

Although binarization can be used to transform an image in a bit pattern, in our experiments the [Minchinton transform](https://www.researchgate.net/publication/233758476_The_Minchinton_Cell_-Analogue_input_to_the_n-Tuple_Net) usually produces better results. An example using this transform is yet to come.

