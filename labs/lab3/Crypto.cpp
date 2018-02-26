#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>
#include <utility>
#include <sstream>

using namespace std;

#include "support.h"

void set_pixel_list(vector<pixel> &pixlist, ppm &img, int seed)
{
    int nrows = img.get_Nrows();
    int ncols = img.get_Ncols();
    vector<int> hist;
    int color = 0;
    int i, j;
    for (i = 0; i < nrows; i++)
    {
        for (j = 0; j < ncols; j++)
        {
            color = ((img[i][j].R >> 4) << 8) | ((img[i][j].G >> 4) << 4) | (img[i][j].B >> 4);
            hist.push_back(color);
            if (i % 2 == 0 && j % 2 == 0)
            {
                pixlist.push_back(pixel(i, j));
            }
        }
    }
    rnumgen RNG(seed);
    RNG.pdf(hist);
    int r1_12 = RNG.rand();
    int r2_12 = RNG.rand();
    int r24 = (r1_12 << 12) | r2_12;
    for (i = (int)pixlist.size() - 1; i > 0; --i)
    {
        swap(pixlist[i], pixlist[r24 % (i+1)]);
    }
    return;
}

void encode(ppm &img, string key)
{
    vector<pixel> pixlist;
    set_pixel_list(pixlist, img, (int)key.length());
    int row, col;
    int i = 0;
    int s;
    int color = 0;
    char c;
    char k = 0x00;
    stringstream stream(key); 
    while (1)
    {
        c = cin.get();
        if (cin.eof())
        {
            c = ETX;
        }
        if (key != "")
        {
            k = stream.get();
            if (stream.eof())
            {
                stream.clear();
                stream.str(key);
                k = stream.get();
            }
        }
        c = c ^ k;
        for (s = 0; s < 8; s++)
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
        }
        if (cin.eof())
        {
            break;
        }
    }
    return;
}

void decode(ppm &img, string key) 
{
    vector<pixel> pixlist; 
    set_pixel_list(pixlist, img, (int)key.length());
    int row, col;
    int i = 0;
    int s;
    int color = 0;
    char c;
    char k = 0x00;
    stringstream stream(key);
    while (1)
    {
        c = 0x00;
        for (s = 0; s < 8; s++)
        {
            row = pixlist[i].row;
            col = pixlist[i].col;
            RGB *pix = img[row] + col;
            if (color == 0)
            {
                c |= ((pix->R & 0x1) << s);
            }
            else if (color == 1)
            {
                c |= ((pix->G & 0x1) << s);
            }
            else
            {
                c |= ((pix->B & 0x1) << s);
            }
            i++;
            color++;
            if (color == 3)
            {
                color = 0;
            }
        }
        if (key != "")
        {
            k = stream.get();
            if (stream.eof())
            {
                stream.clear();
                stream.str(key);
                k = stream.get();
            }
        }
        c = c ^ k;
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
    if (argc < 3 || argc > 4)
    {
        fprintf(stderr, "Usage: ./Crypto -encode|decode [-key=\"text\"] image.ppm\n");
        return -5;
    }
    if (strcmp(argv[1], "-encode") != 0 && strcmp(argv[1], "-decode") != 0)
    {
        fprintf(stderr, "Usage: ./Crypto -encode|decode [-key=\"text\"] image.ppm\n");
        return -6;
    }
    string fname, key;
    if (argc == 3)
    {
        string fname = argv[2];
        if (fname.substr(fname.length() - 4) != ".ppm")
        {
            fprintf(stderr, "Usage: ./Crypto -encode|decode [-key=\"text\"] image.ppm\n");
            return -7;
        }
        string key = "";
    }
    else
    {
        fname = argv[3];
        if (fname.substr(fname.length() - 4) != ".ppm")
        {
            fprintf(stderr, "Usage: ./Crypto -encode|decode [-key=\"text\"] image.ppm\n");
            return -7;
        }
        key = argv[2];
        size_t sub_start = key.find('=');
        key = key.substr(sub_start+1);
    }

    ppm img;
    img.read(fname);

    if (strcmp(argv[1], "-encode") == 0)
    {
        encode(img, key);
        img.write(fname);
        return 0;
    }
    else
    {
        decode(img, key);
        return 0;
    }
}
