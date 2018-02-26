#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <cassert>
#include <algorithm>
#include <functional>
#include <numeric>

using namespace std;

#include "support.h"

/* support.cpp
 * Author: Ian Lumsden
 * Date: February 26, 2018
 *
 * This file contains the implementation for the following functions from support.h:
 *    - ppm::read()
 *    - ppm::write()
 *    - rnumgen::pdf()
 *    - rnumgen::rand()
 */

const char ETX = 0x3;

typedef unsigned char uchar;

/* This ppm member function is used to parse the contents of the ppm
 * represented by fname into a ppm object.
 */
void ppm::read(string fname) 
{ 
    // The file is opened using an input file stream.
    fstream fp;
    fp.open(fname.c_str(), ios::in);
    /* If the stream was unable to open the file, an error message is printed to
     * the console. Then, the file stream is closed, and the program exits.
     */
    if (!fp.is_open())
    {
        fprintf(stderr, "%s could not be opened.\n", fname.c_str());
        fp.close();
        exit(-1);
    }
    /* The P6-ppm magic number is read using the file stream. If the magic number
     * is not P6, an error message is printed to the console. Then, the file stream
     * is closed, and the program exits.
     */
    string magicnum;
    fp >> magicnum;
    if (magicnum != "P6")
    {
        fprintf(stderr, "%s is not the correct file type.\n", fname.c_str());
        fp.close();
        exit(-2);
    }
    /* The number of rows, number of columns, and maximum pixel value are read
     * using the file stream. If the maximum pixel value is not 255, an error message
     * is printed to the console. Then, the file stream is closed, and the program
     * exits.
     */
    int nrows, ncols, maxpixval;
    fp >> nrows >> ncols >> maxpixval;
    if (maxpixval != 255)
    {
        fprintf(stderr, "%s does not have the correct maximum pixel value.\n", fname.c_str());
        fp.close();
        exit(-3);
    }
    // A new line is extracted from the file to move the file pointer to the correct position for the next read.
    char c = fp.get();
    assert(c == '\n');
    // The internal matrix of RGB structs is initialized using the matrix::assign function.
    image.assign(nrows, ncols);
    /* The file stream is used to read in the contents of the entire body of the ppm file.
     * If, for some reason, the read function fails to read all of the ppm file's body, an error message
     * is printed to the console. Then, the file stream is closed, and the program exits.
     */
    fp.read((char *)(image.data()), 3*nrows*ncols); 
    if (fp.gcount() != 3 * nrows * ncols)
    {
        fprintf(stderr, "Number of elements in %s does not match the header data. Should be %i\n", fname.c_str(), 3*nrows*ncols);
        fp.close();
        exit(-4);
    }
    // The file stream is closed to avoid memory leak.
    fp.close();
    return;
}

/* This ppm member function takes the internal data in the ppm object and creates
 * a new ppm file with it.
 */
void ppm::write(string fname) 
{ 
    /* The fname variable is changed to create a new file name called
     * "fname_wmsg.ppm".
     */
    fname.erase(fname.length()-4);
    fname = fname + "_wmsg.ppm";
    /* An output file stream is opened using the new file name.
     * The ios::out and ios::binary flags are set to ensure that
     * the program creates the binary output file if it doesn't exist.
     */
    fstream fp(fname.c_str(), ios::out | ios::binary);
    /* If the file stream cannot open/create the desired file, an error message is
     * printed to the console. Then, the file stream is closed, and the program exits.
     */
    if (!fp.is_open())
    {
        fprintf(stderr, "%s could not be opened/created.", fname.c_str());
        fp.close();
        exit(-4);
    }
    // The header information is printed to the file through the file stream.
    fp << "P6\n";
    fp << get_Nrows() << " " << get_Ncols() << "\n";
    fp << "255\n";
    /* Using the buffer (the matrix's contents casted to char*), the number of rows,
     * and the number of columns, the contents of the body of the ppm is printed to
     * the file through the file stream.
     */
    char *buf = (char *)(image.data());
    int nrows = get_Nrows();
    int ncols = get_Ncols();
    fp.write(buf, 3*nrows*ncols);
    // The file stream is closed to avoid memory leak.
    fp.close();
    return;
}

/* This function instantiates the random number generator.
 * It resizes the internal float vector using the size of the passed
 * vector. It then uses the STL `partial_sum`, `transform`, and `bind2d` functions
 * to turn the internal vector into a "histogram" that is used to control the
 * random number generation.
 */
void rnumgen::pdf(const vector<int> &v)
{
    F.resize(v.size());
    partial_sum(v.begin(), v.end(), F.begin());
    transform(F.begin(), F.end(), F.begin(), bind2nd(divides<float>(), *(F.end() - 1)));
}

// The actual random number generation function for the random number generator.
int rnumgen::rand() const
{
    const float randnorm = RAND_MAX+1.0f;
    const float p = (float)std::rand()/randnorm;
    return upper_bound(F.begin(), F.end(), p) - F.begin();
}
