#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>

using namespace std;

#include "support.h"

void set_pixel_list(vector<pixel> &pixlist, ppm &img)
{
    int nrows = img.get_Nrows();
    int ncols = img.get_Ncols();
    for (int i = 0; i < nrows; i+=2)
    {
        for (int j = 0; j < ncols; j+=2)
        {
            pixlist.push_back(pixel(i, j));
        }
    }
    return;
}

void encode(ppm &img)
{
    vector<pixel> pixlist;
    set_pixel_list(pixlist, img);
    int row, col;
    int i = 0;
    int s = 0;
    int color = 0;
    while (1)
    {
        char c = cin.get();
        if (c == cin.eof())
        {            
            while (s < 8)
            {
                row = pixlist[i].row;
                col = pixlist[i].col;
                RGB *pix = img[row] + col;
                if (color == 0)
                {
                    pix->R &= 0xFE;
                    pix->R |= ((ETX >> s) & 0x1);
                }
                else if (color == 1)
                {
                    pix->G &= 0xFE;
                    pix->G |= ((ETX >> s) & 0x1);
                }
                else
                {
                    pix->B &= 0xFE;
                    pix->B |= ((ETX >> s) & 0x1);
                }
                i++;
                color++;
                if (color == 3)
                {
                    color = 0;
                }
                s++;
            }
            s = 0;
            break;
        }
        while (s < 8)
        {
            row = pixlist[i].row;
            col = pixlist[i].col;
            RGB *pix = img[row] + col;
            if (color == 0)
            {
                pix->R &= 0xFE;
                pix->R |= ((c >> s) & 0x1);
            }
            else if (color == 1)
            {
                pix->G &= 0xFE;
                pix->G |= ((c >> s) & 0x1);
            }
            else
            {
                pix->B &= 0xFE;
                pix->B |= ((c >> s) & 0x1);
            }
            i++;
            color++;
            if (color == 3)
            {
                color = 0;
            }
            s++;
        }
        s = 0;
    }
    return;
}

void decode(ppm &img) 
{
    vector<pixel> pixlist; 
    set_pixel_list(pixlist, img);
    int row, col;
    int i = 0;
    int s = 0;
    int color = 0;
    char c = 0x00;
    while (1)
    {
        while (s < 8)
        {
            row = pixlist[i].row;
            col = pixlist[i].col;
            RGB *pix = img[row] + col;
            if (color == 0)
            {
                c = (((c >> s) & pix->R) << s); 
            }
            else if (color == 1)
            {
                c = (((c >> s) & pix->G) << s); 
            }
            else
            {
                c = (((c >> s) & pix->B) << s); 
            }
            i++;
            color++;
            if (color == 3)
            {
                color = 0;
            }
            s++;
        }
        s = 0;
        if (c == ETX)
        {
            break;
        }
        else
        {
            cout.put(c);
        }
    }
    return;
}

int main(int argc, char *argv[]) 
{
    if (argc < 3)
    {
        fprintf(stderr, "Usage: ./Crypto -encode|decode image.ppm\n");
        return -5;
    }
    if (strcmp(argv[1], "-encode") != 0 && strcmp(argv[1], "-decode") != 0)
    {
        fprintf(stderr, "Usage: ./Crypto -encode|decode image.ppm\n");
        return -6;
    }
    string fname = argv[2];
    if (fname.substr(fname.length() - 4) != ".ppm")
    {
        fprintf(stderr, "Usage: ./Crypto -encode|decode image.ppm\n");
        return -7;
    }

    ppm img;
    img.read(fname);

    if (strcmp(argv[1], "-encode") == 0)
    {
        encode(img);
    }
    else
    {
        decode(img);
    }

    img.write(fname);
    return 0;
}
