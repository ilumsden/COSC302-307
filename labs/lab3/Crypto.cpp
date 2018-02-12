#include <cstdio>
#include <cstring>

using namespace std;

#include "support.h"

void encode(...) 
{
  write this
}

void decode(...) 
{
  write this
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
        encode(fname, img);
    }
    else
    {
        decode(fname, img);
    }

    img.write(fname);
    return 0;
}
