#include <cstdio>
#include <cstdlib>

using namespace std;

#include "support.h"

const char ETX = 0x3;

typedef unsigned char uchar;

void ppm::read(string fname) 
{ 
    FILE *fp = fopen(fname.c_str(), "r");
    if (fp == NULL)
    {
        fprintf(stderr, "%s could not be opened.\n", fname.c_str());
        exit(-1);
    }
    char magicnum[2];
    fscanf(fp, "%s", magicnum);
    if ("P6" != magicnum)
    {
        fprintf(stderr, "%s is not the correct file type.\n", fname.c_str());
        exit(-2);
    }
    int nrows, ncols, maxpixval;
    fscanf(fp, "%i %i", nrows, ncols);
    fscanf(fp, "%i", maxpixval);
    if (maxpixval != 255)
    {
        fprintf(stderr, "%s does not have the correct maximum pixel value.\n", fname.c_str());
        exit(-3);
    }
    image.assign(nrows, ncols);
    int n = 3;
    uchar buffer[n];
    int nread_total, nread;
    int j = 0;
    RGB **imgdata = image.data();
    while (1)
    {
        nread = fread(buffer, 1, n, fp);
        nread_total += nread;
        if (nread_total > 3 * nrows * ncols)
        {
            fprintf(stderr, "Number of elements in %s does not match the header data.\n", fname.c_str());
            exit(-3);
        }
        if (nread == 0 && feof(fp))
        {
            break;
        }
        (*imgdata + j)->R = buffer[0];
        (*imgdata + j)->G = buffer[1];
        (*imgdata + j)->B = buffer[2];
        j++;
    } 
    if (nread_total != 3 * nrows * ncols)
    {
        fprintf(stderr, "Number of elements in %s does not match the header data.\n", fname.c_str());
        exit(-3);
    }
    return;
}

void ppm::write(string fname) 
{ 
    fname.erase(fname.length()-4);
    fname = fname + "_wmsg.ppm";
    FILE *fp = fopen(fname.c_str(), "w+");
    if (fp == NULL)
    {
        fprintf(stderr, "%s could not be opened/created.", fname.c_str());
        exit(-4);
    }
    fprintf(fp, "P6\n");
    fprintf(fp, "%i %i\n", get_Nrows(), get_Ncols());
    fprintf(fp, "255\n");
    RGB **buf = image.data();
    int nrows = get_Nrows();
    int ncols = get_Ncols();
    for (int i = 0; i < nrows * ncols; i++)
    {
        fprintf(fp, "%c %c %c  ", (*buf + i)->R, (*buf + i)->G, (*buf + i)->B);
        if (i != 0 && i % ncols == 0)
        {
            fprintf(fp, "\n");
        }
    }
    return;
}
