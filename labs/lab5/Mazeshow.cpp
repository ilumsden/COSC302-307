#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>

using namespace std;

/* Mazeshow.cpp
 * Author: Ian Lumsden
 * Date: April 2, 2018
 *
 * This file generates a PPM image of a maze based on either
 * one or two TXT files:
 *    * Maze.txt: a TXT file stating where the walls are in the maze
 *    * Path.txt: an optional TXT file containing a list of cells through
 *          which the path that solves the maze passes.
 */

// Defines `uchar` as an alias of `unsigned char`.
typedef unsigned char uchar;

// The ppm class stores the image data that will be written to a PPM file.
class ppm
{
    // `pixel` stores the three color values for each PPM pixel.
    struct pixel
    {
        pixel() { R=0xFF; G=0xFF; B=0xFF; }
        // An alternate constructor that sets the values to a value other than 0xFF.
        pixel(uchar color) { R=color; G=color; B=color; }
        // These variables are `uchar` to allow them to be more easily written in binary.
        uchar R, G, B;
    };
    public:
        // Default constructor is used to set the value of `width` to 11.
        ppm() : width(11) { ; } 
        ~ppm();
        void make_maze(string);
        void make_maze(string, string);
        void print_maze();
    private:
        // `img` is a 2D pixel array that stores the PPM image data.
        pixel** img;
        // `width` is a CONST int that specifies the size (in pixels) of the width/height of a cell.
        const int width;
        // These variables store width (# rows) and height (# cols) of the image in PIXELS.
        int Nrows, Ncols;
};

// A destructor for ppm to prevent data corruption and seg faults.
ppm::~ppm()
{
    for (int i = 0; i < Nrows; i++)
    {
        delete [] img[i];
    }
    delete [] img;
}

/* This function generates the PPM data for a maze without a path
 * from the data in a MAZE TXT file.
 */
void ppm::make_maze(string mazename)
{
    // A file stream to the MAZE TXT file is opened in input mode.
    fstream fin;
    fin.open(mazename.c_str(), ios::in);
    // If the file couldn't be opened, an error message is printed, and the program exits.
    if (!fin.is_open())
    {
        fprintf(stderr, "Could not open %s\n", mazename.c_str());
        exit(-3);
    }
    // The header info from the MAZE file is read.
    string type;
    int nr, nc;
    fin >> type >> nr >> nc;
    // Prints an error message and exits if the incorrect file type was passed.
    if (type != "MAZE")
    {
        fprintf(stderr, "%s is not the correct file type (incorrect header).\n", mazename.c_str());
        exit(-4);
    }
    // Calculates the pixel-based width and height of the image
    Nrows = nr*width+1;
    Ncols = nc*width+1;
    // Initializes the `img` array using the calculated size.
    img = new pixel*[Nrows];
    for (int i = 0; i < Nrows; i++)
    {
        img[i] = new pixel[Ncols];
    }
    /* `cell` stores the cell number read from the TXT file.
     * `wall` stores the wall number read from the TXT file.
     * `row` stores the pixel-based row coordinate of the top-left corner of the cell.
     * `col` stores the pixel-based column coordinate of the top-left corner of the cell.
     * `rind` stores the 2D, non-pixel-based row coordinate of the cell.
     * `cind` stores the 2D, non-pixel-based column coordinate of the cell.
     */
    int cell, wall;
    int row, col;
    int rind, cind;
    // The loop ends when cell/wall pairs can no longer be read from the TXT file.
    while (fin >> cell >> wall)
    {
        // Calculates the values of `cind`, `rind`, `row`, and `col`
        cind = cell % nc;
        rind = (cell - cind)/nc;
        row = rind * width;
        col = cind * width;
        /* Do nothing if the bottom or right walls are being considered 
         * unless the wall is an exterior wall.
         */
        if ((wall == 2 && rind != nr-1) || (wall == 3 && cind != nc-1))
        {
            continue;
        }
        // This loop controls the drawing of each wall.
        for (int i = 0; i < width; i++)
        {
            // If the top wall is specified, the pixels along the top of the cell are set to black.
            if (wall == 0)
            {
                img[row][col+i] = pixel(0x00);
            }
            /* If the left wall is specified, the pixels along the left side
             * the of cell are set to black.
             */
            else if (wall == 1)
            {
                img[row+i][col] = pixel(0x00);
            }
            /* If the bottom wall is specified and the wall is exterior,
             * the pixels along the bottom of the cell are set to black. Otherwise,
             * an internal error occured, and the loop is broken to fix the error.
             */
            else if (wall == 2)
            {
                if (rind == nr-1)
                {
                    img[Nrows-1][col+i] = pixel(0x00);
                }
                else
                {
                    break;
                }
            }
            /* If the right wall is specified and the wall is exterior,
             * the pixels along the right side of the cell are set to black. Otherwise,
             * an internal error occured, and the loop is broken to fix the error.
             */
            else
            {
                if (cind == nc-1)
                {
                    img[row+i][Ncols-1] = pixel(0x00);
                }
                else
                {
                    break;
                }
            }
        }
    }
    // The file stream is closed.
    fin.close();
}

/* This function generates the PPM data for a maze with a path.
 * The maze data is obtained from the MAZE TXT file, and the path data
 * is obtained from the PATH TXT file.
 */
void ppm::make_maze(string mazename, string pathname)
{
    // A file stream to the MAZE TXT file is opened in input mode.
    fstream fin1;
    fin1.open(mazename.c_str(), ios::in);
    // If the file couldn't be opened, an error message is printed, and the program exits.
    if (!fin1.is_open())
    {
        fprintf(stderr, "Could not open %s\n", mazename.c_str());
        exit(-3);
    }
    // The header info from the MAZE file is read.
    string type;
    int nr, nc;
    fin1 >> type >> nr >> nc;
    // Prints an error message and exits if the incorrect file type was passed as the MAZE file.
    if (type != "MAZE")
    {
        fprintf(stderr, "%s is not the correct file type (incorrect header).\n", mazename.c_str());
        exit(-4);
    }
    // Temporarily stores `nr` and `nc` in `Nrows` and `Ncols` to prevent data loss.
    Nrows = nr;
    Ncols = nc;
    // A file stream to the PATH TXT file is opened in input mode.
    fstream fin2;
    fin2.open(pathname.c_str(), ios::in);
    // If the file couldn't be opened, an error message is printed, and the program exits.
    if (!fin2.is_open())
    {
        fprintf(stderr, "Could not open %s\n", pathname.c_str());
        exit(-3);
    }
    // The header info from the PATH file is read.
    fin2 >> type >> nr >> nc;
    // Prints an error message and exits if the incorrect file type was passed as the PATH file.
    if (type != "PATH") {
        fprintf(stderr, "%s is not the correct file type (incorrect header).\n", pathname.c_str());
        exit(-4);
    }
    /* If the number of rows and columns specified by the two files don't agree,
     * an error message is printed, and the program exits.
     */
    if (nr != Nrows || nc != Ncols)
    {
        fprintf(stderr, "The maze dimensions in %s and %s do not match.\n", mazename.c_str(), pathname.c_str());
        exit(-5);
    }
    // Calculates the pixel-based width and height of the image
    Nrows = nr*width+1;
    Ncols = nc*width+1;
    // Initializes the `img` array using the calculated size.
    img = new pixel*[Nrows];
    for (int i = 0; i < Nrows; i++)
    {
        img[i] = new pixel[Ncols];
    }
    /* `cellnum` stores the cell number read from the PATH file.
     * `row` stores the pixel-based row coordinate of the top-left corner of the cell.
     * `col` stores the pixel-based column coordinate of the top-left corner of the cell.
     * `rind` stores the 2D, non-pixel-based row coordinate of the cell.
     * `cind` stores the 2D, non-pixel-based column coordinate of the cell.
     */
    int cellnum;
    int row, col;
    int rind, cind;
    while(fin2 >> cellnum)
    {
        // Calculates the values of `cind`, `rind`, `row`, and `col`
        cind = cellnum % nc;
        rind = (cellnum - cind)/nc;
        row = rind * width;
        col = cind * width;
        // Sets the color of the pixels in the current PATH cell to yellow.
        for (int i = row; i < row+width; i++)
        {
            for (int j = col; j < col+width; j++)
            {
                img[i][j] = pixel(0xF0);
                img[i][j].B = 0x64;
            }
        }
    }
    // Closes the PATH file stream.
    fin2.close();
    /* `cell` stores the cell number read from the TXT file.
     * `wall` stores the wall number read from the TXT file.
     */
    int cell, wall;
    /* Everything in this loop is the same as the other `maze_make` function.
     * Refer to the comments in that function for detailed descriptions of the
     * components of this loop.
     */
    while (fin1 >> cell >> wall)
    {
        cind = cell % nc;
        rind = (cell - cind)/nc;
        row = rind * width;
        col = cind * width;
        if ((wall == 2 && rind != nr-1) || (wall == 3 && cind != nc-1))
        {
            continue;
        }
        for (int i = 0; i < width; i++)
        {
            if (wall == 0)
            {
                img[row][col+i] = pixel(0x00);
            }
            else if (wall == 1)
            {
                img[row+i][col] = pixel(0x00);
            }
            else if (wall == 2)
            {
                if (rind == nr-1)
                {
                    img[Nrows-1][col+i] = pixel(0x00);
                }
                else
                {
                    break;
                }
            }
            else
            {
                if (cind == nc-1)
                {
                    img[row+i][Ncols-1] = pixel(0x00);
                }
                else
                {
                    break;
                }
            }
        }
    }
    // Closes the MAZE file stream.
    fin1.close();
}

// This function prints the PPM image data to stdout as a P6 (binary) PPM.
void ppm::print_maze()
{
    /* Since cout cannot natively print binary data (besides putting it in
     * HEX notation), a file stream pointer is created and linked to a binary output
     * fstream. This file stream is then redirected to print to stdout using the
     * `basic_ios<char>::rdbuf()` function.
     */
    fstream *fout = new fstream();
    fout->open("tmp.txt", ios::out | ios::binary);
    streambuf *fout_buffer = fout->rdbuf();
    streambuf *cout_buffer = cout.rdbuf();
    fout->basic_ios<char>::rdbuf(cout_buffer);
    // Prints the non-binary header data to stdout using cout
    cout << "P6\n";
    cout << Ncols << " " << Nrows << "\n";
    cout << "255\n";
    // Prints the image pixel data in binary to stdout using fout
    for (int i = 0; i < Nrows; i++)
    {
        for (int j = 0; j < Ncols; j++)
        {
            *fout << img[i][j].R << img[i][j].G << img[i][j].B;
        }
    }
    // Redirects fout back to its original file before closing the file stream.
    fout->basic_ios<char>::rdbuf(fout_buffer);
    fout->close();
    /* Tries to delete the temporary file that was created when openning the
     * file stream using the C `remove` function. If the file cannot be deleted,
     * an error message is printed.
     */
    if (remove("tmp.txt") != 0)
    {
        fprintf(stderr, "Temporary file \"tmp.txt\" could not be deleted.\n");
    }
    // Since `fout` is created using the new keyword, it must be manually deleted.
    delete fout;
}

// The driver for Mazeshow
int main(int argc, char** argv)
{
    /* If the incorrect number of command-line arguments are passed,
     * an error message is printed, and the program exits.
     */
    if (argc < 2 || argc > 3)
    {
        fprintf(stderr, "Usage: ./Mazeshow maze.txt [path.txt]\n");
        return -7;
    }
    // Creates a ppm object and stores the MAZE file name in `mazename`.
    string mazename, pathname;
    ppm image;
    mazename = argv[1];
    /* If there is only 1 user-provided command-line argument, the one
     * parameter `make_maze` function is called.
     */
    if (argc == 2)
    {
        image.make_maze(mazename);
    }
    /* If there are 2 user-provided command-line arguments, the PATH file name
     * is stored in `pathname`, and the 2 parameter `maze_make` function is called.
     */
    else
    {
        pathname = argv[2];
        image.make_maze(mazename, pathname);
    }
    // The `print_maze` method is called to generate the PPM.
    image.print_maze();
}
