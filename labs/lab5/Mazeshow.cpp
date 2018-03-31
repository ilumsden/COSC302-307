#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

typedef unsigned char uchar;

class ppm
{
    struct pixel
    {
        pixel() { R=0xFF; G=0xFF; B=0xFF; }
        pixel(uchar color) { R=color; G=color; B=color; }
        uchar R, G, B;
    };
    public:
        ppm() : width(11) { ; } 
        ~ppm();
        void make_maze(string);
        void make_maze(string, string);
        void print_maze();
    private:
        pixel** img;
        const int width;
        int Nrows, Ncols;
};

ppm::~ppm()
{
    for (int i = 0; i < Nrows; i++)
    {
        delete [] img[i];
    }
    delete [] img;
}

void ppm::make_maze(string mazename)
{
    fstream fin;
    fin.open(mazename.c_str(), ios::in);
    if (!fin.is_open())
    {
        fprintf(stderr, "Could not open %s\n", mazename.c_str());
        exit(-3);
    }
    string type;
    int nr, nc;
    fin >> type >> nr >> nc;
    if (type != "MAZE")
    {
        fprintf(stderr, "%s is not the correct file type (incorrect header).\n", mazename.c_str());
        exit(-4);
    }
    Nrows = nr*width+1;
    Ncols = nc*width+1;
    img = new pixel*[Nrows];
    for (int i = 0; i < Nrows; i++)
    {
        img[i] = new pixel[Ncols];
    }
    int cell, wall;
    int row, col;
    while (fin >> cell >> wall)
    {
        col = cell % nc;
        row = (cell - col)/nc;
        row *= width;
        col *= width;
        if (wall == 2 || wall == 3)
        {
            continue;
        }
        for (int i = 0; i < width; i++)
        {
            if (wall == 0)
            {
                img[row][col+i] = pixel(0x00);
            }
            else
            {
                img[row+i][col] = pixel(0x00);
            }
        }
    }
}

void ppm::make_maze(string mazename, string pathname)
{
    fstream fin1;
    fin1.open(mazename.c_str(), ios::in);
    if (!fin1.is_open())
    {
        fprintf(stderr, "Could not open %s\n", mazename.c_str());
        exit(-3);
    }
    string type;
    int nr, nc;
    fin1 >> type >> nr >> nc;
    if (type != "MAZE")
    {
        fprintf(stderr, "%s is not the correct file type (incorrect header).\n", mazename.c_str());
        exit(-4);
    }
    Nrows = nr;
    Ncols = nc;
    fstream fin2;
    fin2.open(mazename.c_str(), ios::in);
    if (!fin2.is_open())
    {
        fprintf(stderr, "Could not open %s\n", mazename.c_str());
        exit(-3);
    }
    fin2 >> type >> nr >> nc;
    if (type != "PATH") {
        fprintf(stderr, "%s is not the correct file type (incorrect header).\n", mazename.c_str());
        exit(-4);
    }
    if (nr != Nrows || nc != Ncols)
    {
        fprintf(stderr, "The maze dimensions in %s and %s do not match.\n", mazename.c_str(), pathname.c_str());
        exit(-5);
    }
    Nrows = nr*width+1;
    Ncols = nc*width+1;
    img = new pixel*[Nrows];
    for (int i = 0; i < Nrows; i++)
    {
        img[i] = new pixel[Ncols];
    }
    int cellnum;
    int row, col;
    while(fin2 >> cellnum)
    {
        col = cellnum % nc;
        row = (cellnum - col)/nc;
        row *= width;
        col *= width;
        for (int i = row; i < row+width; i++)
        {
            for (int j = col; j < col+width; j++)
            {
                img[i][j] = pixel(0xF0);
                img[i][j].B = 0x64;
            }
        }
    }
    int cell, wall;
    while (fin1 >> cell >> wall)
    {
        col = cell % Ncols;
        row = (cell - col)/Ncols;
        row *= width;
        col *= width;
        if (wall == 2 || wall == 3)
        {
            continue;
        }
        for (int i = 0; i < width; i++)
        {
            if (wall == 0)
            {
                img[row][col+i] = pixel(0x00);
            }
            else
            {
                img[row+i][col] = pixel(0x00);
            }
        }
    }
}

void ppm::print_maze()
{
    cout << "P6\n";
    cout << Ncols << " " << Nrows << "\n";
    cout << "255\n";
    for (int i = 0; i < Nrows; i++)
    {
        for (int j = 0; j < Ncols; j++)
        {
            cout << hex << img[i][j].R << " " << img[i][j].G << " " << img[i][j].B << " ";
        }
    }
}

int main(int argc, char** argv)
{
    if (argc < 2 || argc > 3)
    {
        fprintf(stderr, "Usage: ./Mazeshow maze.txt [path.txt]\n");
        return -7;
    }
    string mazename, pathname;
    ppm image;
    mazename = argv[1];
    if (argc == 2)
    {
        image.make_maze(mazename);
    }
    else
    {
        pathname = argv[2];
        image.make_maze(mazename, pathname);
    }
    image.print_maze();
}
