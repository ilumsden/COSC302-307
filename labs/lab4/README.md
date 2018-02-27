# Lab 3

## Introduction

In this lab you will write a program for embedding ASCII text within a PPM image using a randomized algorithm for selecting where to place the characters. We will call the program `crypto` which makes it sound as if it is based on cryptography. In reality, it is a case of steganography, which, according to Wikipedia, "is the art or practice of concealing a file, message, image, or video within another file, message, image, or video." Either way, the program is actually pretty cool.

Before you get any ideas to turn it into an app that will make you rich, be forewarned that somebody may hold a patent on a similar idea. The goal of the exercise is to have you apply many of the things we have discussed since the begining of the semester. No attempt has been made to research the literature on the subjects of steganography and cryptography.

Your job is to complete writing the program and make it behave as described next.

## Support.h

This header file defines a template based 2D matrix class, a pixel index class, an RGB class, and a ppm image class.

The 2D matrix class can be either version 1 or __version 2__ from the matrix handout (lecture material), as version 3 will not work correctly with the implementation defined later on. The constructor should be changed to simply set the data pointer equal to NULL. A new public member function called `assign()` should be added that allocates the needed memory given two parameters, `Nrows` and `Ncols`. A new public member function called `data()` should be added that returns a pointer to the matrix buffer. This is needed by the `ppm::read()` and `ppm::write()` functions. With this class being template based, all code needs to be included in the header file. If you like, it can even be included in the class definition.

The pixel index class is merely a struct that stores a pair of row and column indices. The constructor should allow instantiation both with and without new values for these indices. The default constructor should set both indices to 0. With this class being so simple, all code can be included in the class definition.

The RGB class, which is also just a struct, defines a three char data structure with members R, G, and B. The default constructor sets the values of these to 0. With this class being so simple, all code can be included in the class definition.

The ppm image class must define all functionality needed to read, write, and store a P6 type ppm image. Public member functions include the constructor which initializes all member data, `read()` and `write()` functions, an overloaded `[]`-operator for index based lookup, and `get_Nrows()` and `get_Ncols()` functions for exporting these data variables. All member data must be private and the image must be stored as a matrix object. The `read()` and `write()` functions are descirbed below. The `[]`-operator returns a pointer to the image row corresponding to the integer index given to it. All but the `read()` and `write()` function code can be included in the class definition.

## Support.cpp

This file contains the implementation for the `ppm::read()` and `ppm::write()` functions which both take one argument, namely the name of the file.

The `ppm::read()` function opens the specified file, then reads the header information followed by the image data. Error checking must be included for successful opening of the file (hint: cf. `fstream::is_open()`), the proper P6 format, a max value of 255, and the correct number of bytes read.

The `ppm::write()` function creates file name "image_wmsg.ppm" (assuming the input file was "image.ppm") where "wmsg" is short for "with message" (in other words, an image which has text embedded within it), opens the file, then writes the header information followed by the image data. Error checking must be included for successful opening of the file (hint: cf. `ios::is_open()`).

## Crypto.cpp

This file implements the `main()` function, the `encode()` and `decode()` functions, and all other support code, which includes a `set_pixel_list()` function.

The `main()` function parses the command line arguments `-encode|decode image.ppm` where the pipe symbol means logical OR (standard unix). A meaningful error message is printed if the proper `-encode|decode` mode option is missing or not present, as well as if an unknown option is given. A ppm image object is instantiated. The specified ppm file is read and passed to the `encode()` or `decode()` function as appropriate.

_The different versions of the code for the lab mostly involve the progressive development of _`Crypto.cpp`_. These different versions are described below._

### Version 1 (COSC 302 and 307):

The solution executable `crypto1` creates the framework for this version. First, write the `set_pixel_list()` function which adds pixel index objects to a vector list. The function is given two arguments: a reference to a ppm image object and a reference to said vector list. For now, have the function add all even pixel index pairs (i.e. (0, 0), (0, 2), (0, 4), ..., (2, 0), (2, 2), ..., etc.). You change this in _version 2_.

The `encode()` function instantiates the vector list of pixel indices and calls the `set_pixel_list()` function to populate it. Then, text is read from standard input (`cin`) and encoded into the ppm image. The basic idea is to replace one RGB bit with one text bit in locations specified by the pixel index list. __Warning: You may have to read the how-to description several times before you fully understand the process.__

Read and encode the text one character at a time including all white space (hint: cf. `fstream::get()`). Furthermore, extract and encode one character bit at a time. The pixel index list tells you which pixels to target. Use one pixel per character bit. Alternate between the red, green, and blue color values. Use bit logic to extract the bits in order from LSB to MSB (hint: `x>>k` shifts the _k_th bit to the LSB position and `x & 0x1` sets all but the LSB to 0). Use bit logic to encode the extracted bit. That is, replace the LSB of the target pixel color with the extracted character bit (hint: `x &= 0xFE` zeros out the x-LSB while `x |= (y &0x1)` sets it to the y-LSB). When all the text has been processed, add an ETX (`0x3`) character, so you know when to stop when decoding an image. The ETX has been included in the provided support files for your convenience.

The `decode()` function reverses the process. Once you understand how the encoding works, this is relatively simple. Create the same pixel index list and use it to extract the LSB from eight pixels. Combine these to form a character that you print to standard out (hint: cf. `fstrea::put()`). Stop when the ETX character has been extracted. If you do it right, you see the same text as you encoded. If you don't you get garbage.

### Version 2 (COSC 302 and 307):

`Crypto2` adds randomness to the pixel index list. Without, somebody might figure out how to read your secret text by detecting patterns. As unlikely as that is, we want to make it nearly impossible. Specifically, we will distribute the eight bits of each text character across eight randomly chosen pixels. Instead of just using the standard random number generator (which we could seed with a secret number), we will use a data driven distribution to make it even more unique. For simplicity, we will use the ppm input image. A more sophisticated version would use another image but we will not go there.

Modify the `set_pixel_list()` function as follows. First, compute a histogram of the color values for all pixels. to keep the size of the histogram managable, extract the four most significant bits from each color byte and combine these half-bytes to obtain a 12-bit color integer. That is `color = (R>>4)<<8 | (G>>4)<<4 | B>>4`. Then instantiate a random number generator that can produce random numbers with probabilities that are proportional to these histogram counts. Use this random number generator to permute the pixel index list. In order to support the processing of more than `2^12 = 4K` pixels, combine two of these random numbers to form a 24-bit number that allows the processing of `2^24 = 16M` pixels. That is, `r24 = (r1_12<<12) | r2_12`. The heavy lifting can be done using the code in the rnumgen and randperm handouts (lecture material). Add the rnumgen class definition to `support.h`, and add the member function implementations to `support.cpp`. Use a seed value of 0. Incorporate the randperm idea into the `set_pixel_list()` function.

If you do the above exactly the same way as the solution code, one can decode what the other encodes. If you do even the slightest thing differently (including when and how many times you call the randome number generator), your code will not work with the encodings produced by the solution code, but you can still have fun as it may work with its own encodings.

### Version 3 (COSC 307 ONLY):

_`Crypto3` is required in order for CS307 students to get full credit but is optional and available for extra credit for CS302 students. That is, CS307 students will have up to 25 points deducted if the functionality described next is not included or doesn't work right. CS302 students will have up to 25 points added if they complete this successfully._

A simple, but effective encryption method consists of repeatedly applying XOR to the characters of th etext with characters in a secret key known only to the sender and the receiver. Say the text character is c and the key character is k, then it is easily seen that `c = XOR(XOR(c, k), k)`. We will apply this idea as follows. The `main()` function will accept `-key="text"` as an optional third argument. The `encode()` and `decode()` functions will be given this text as a second argument. The length of the key will be passed to the `set_pixel_list()` function which uses it to seed the random number generator. When a key is not specified, the key text is the empty string which has a length of 0 as used for _version 2_. The `encode()` and `decode()` functions apply XOR to each character before encoding and after decoding respecively (hint: `XOR(x, y) = x^y`). When the key text consists of multiple characters, use these in an alternating fashion.

## Provided Materials:

The following files were provided before starting the lab:
* Solution Executables `crypto1-3`
* A skeleton code outline in `support.h`
* A skeleton code outline in `support.cpp`
* A skeleton code outline in `Crypto.cpp`
* Test images `plane.ppm`, `rocket.ppm`, and `truck.ppm`
* A makefile
