#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <typeinfo>
#include <libexplain/fread.h>

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
        fclose(fp);
        exit(-1);
    }
    char magicnum[2];
    fscanf(fp, "%s", magicnum);
    if (strcmp(magicnum, "P6") != 0)
    {
        fprintf(stderr, "%s is not the correct file type.\n", fname.c_str());
        fclose(fp);
        exit(-2);
    }
    int nrows, ncols, maxpixval;
    fscanf(fp, "%i %i", &nrows, &ncols);
    fprintf(stdout, "%i %i", nrows, ncols);
    fscanf(fp, "%i\n", &maxpixval);
    if (maxpixval != 255)
    {
        fprintf(stderr, "%s does not have the correct maximum pixel value.\n", fname.c_str());
        fclose(fp);
        exit(-3);
    }
    image.assign(nrows, ncols);
    int n = 3;
    int nread;
    int nread_total = 0;
    int i = 0;
    int j = 0;
    //while (1)
    //{
        nread = fread((char *)(image.data()), 1, 3*nrows*ncols, fp); 
        fprintf(stdout, "nread = %i\n",  nread);
        if (ferror(fp))
        {
            fprintf(stdout, "There was a read error.\n");
            fprintf(stdout, "Explanation: %s", explain_fread((char *)(image.data()), 1, 3*nrows*ncols, fp));
        }
        nread_total += nread;
        /*if (nread_total > 3 * nrows * ncols)
        {
            fprintf(stderr, "Number of elements in %s does not match the header data.\n", fname.c_str());
            fclose(fp);
            exit(-3);
        }*/
        /*if (nread == 0 && feof(fp))
        {
            break;
        }*/
        /*fprintf(stdout, "buffer[0] = %c\n", buffer[0]);
        cout << "buffer[0] is a uchar: " << (typeid(buffer[0]) == typeid(uchar)) << endl;
        fprintf(stdout, "buffer[1] = %c\n", buffer[1]);
        cout << "buffer[1] is a uchar: " << (typeid(buffer[1]) == typeid(uchar)) << endl;
        fprintf(stdout, "buffer[2] = %c\n", buffer[2]);
        cout << "buffer[2] is a uchar: " << (typeid(buffer[2]) == typeid(uchar)) << endl;
        fprintf(stdout, "Pre-set for test.\n");
        RGB *test = image[i];
        fprintf(stdout, "Post-set for test.\n");
        cout << "test is " << (typeid((test + j)) == typeid(RGB *)) << endl;
        (image[i] + j)->R = a;
        fprintf(stdout, "Post-set for R.\n");
        (image[i] + j)->G = b;
        fprintf(stdout, "Post-set for G.\n");
        (image[i] + j)->B = c;
        fprintf(stdout, "Post-set for B.\n");*/
        /*j++;
        if (j == ncols)
        {
            j = 0;
            i++;
        }
    }*/ 
    if (nread != 3 * nrows * ncols)
    {
        fprintf(stderr, "Number of elements in %s does not match the header data. Should be %i\n", fname.c_str(), 3*nrows*ncols);
        fclose(fp);
        exit(-3);
    }
    fclose(fp);
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
    fwrite(buf, sizeof(RGB), sizeof(buf), fp);
    /*for (int i = 0; i < nrows * ncols; i++)
    {
        fprintf(stdout, "Pre-data-write.\n");
        fprintf(fp, "%d %d %d ", (*buf + i)->R, (*buf + i)->G, (*buf + i)->B);
        if (i != 0 && i % ncols == 0)
        {
            fprintf(fp, "\n");
        }
    }*/
    return;
}
