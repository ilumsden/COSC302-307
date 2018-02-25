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

const char ETX = 0x3;

typedef unsigned char uchar;

void ppm::read(string fname) 
{ 
    fstream fp;
    fp.open(fname.c_str(), ios::in);
    if (!fp.is_open())
    {
        fprintf(stderr, "%s could not be opened.\n", fname.c_str());
        fp.close();
        exit(-1);
    }
    string magicnum;
    fp >> magicnum;
    if (magicnum != "P6")
    {
        fprintf(stderr, "%s is not the correct file type.\n", fname.c_str());
        fp.close();
        exit(-2);
    }
    int nrows, ncols, maxpixval;
    fp >> nrows >> ncols >> maxpixval;
    if (maxpixval != 255)
    {
        fprintf(stderr, "%s does not have the correct maximum pixel value.\n", fname.c_str());
        fp.close();
        exit(-3);
    }
    char c = fp.get();
    assert(c == '\n');
    image.assign(nrows, ncols);
    fp.read((char *)(image.data()), 3*nrows*ncols); 
    if (fp.gcount() != 3 * nrows * ncols)
    {
        fprintf(stderr, "Number of elements in %s does not match the header data. Should be %i\n", fname.c_str(), 3*nrows*ncols);
        fp.close();
        exit(-4);
    }
    fp.close();
    return;
}

void ppm::write(string fname) 
{ 
    fname.erase(fname.length()-4);
    fname = fname + "_wmsg.ppm";
    fstream fp(fname.c_str(), ios::out | ios::binary);
    if (!fp.is_open())
    {
        fprintf(stderr, "%s could not be opened/created.", fname.c_str());
        exit(-4);
    }
    fp << "P6\n";
    fp << get_Nrows() << " " << get_Ncols() << "\n";
    fp << "255\n";
    char *buf = (char *)(image.data());
    int nrows = get_Nrows();
    int ncols = get_Ncols();
    fp.write(buf, 3*nrows*ncols);
    fp.close();
    return;
}

void rnumgen::pdf(const vector<int> &v)
{
    F.resize(v.size());
    partial_sum(v.begin(), v.end(), F.begin());
    transform(F.begin(), F.end(), F.begin(), bind2nd(divides<float>(), *(F.end() - 1)));
}

int rnumgen::rand() const
{
    const float randnorm = RAND_MAX+1.0f;
    const float p = (float)std::rand()/randnorm;
    return upper_bound(F.begin(), F.end(), p) - F.begin();
}
