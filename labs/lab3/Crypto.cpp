#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>
#include <utility>
#include <sstream>

using namespace std;

#include "support.h"

/*
 * Crypto.cpp
 * Author: Ian Lumsden
 * Date: February 26, 2018
 *
 * This code is the driver for performing stenography on ppm images.
 * First, it parses the command line arguments to determine what mode
 * to run in (encode or decode), the file that the stenography will be
 * performed on, and (optionally) the key to use in the stenography (default
 * is no key). Then, based on the command line arguments, it parses the data
 * from the specified ppm image into a ppm object (see support.h and support.cpp).
 * Using this object, if the program is running in encode mode, it encodes the
 * individual bits of each character into the LSBs of a color components of pixels 
 * specified by the `set_pixel_list` function. The color value used rotates per pixel
 * between red, green, and blue. If the program is running in decode mode, it reverses
 * this process to get the individual characters of the message. For a more secure encoding,
 * the pixels that are used are chosen using a random number generator (see rnumgen in
 * support.h and support.cpp). Additionally, if a key was provided, each character of the
 * message is XORed with a character from the key before encoding and after decoding. The
 * key character used in the XOR operation is determined by simply passing through the 
 * key character-by-character. After an encode operation, the contents of the ppm object
 * are written back out to a new ppm file.
 */

/* This function generates a randomized vector of pixel objects that
 * will be used to control the encoding and decoding.
 */
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
            /* Creates a histogram of color values for each pixel.
             * This will be used to setup the random number generator.
             */
            color = ((img[i][j].R >> 4) << 8) | ((img[i][j].G >> 4) << 4) | (img[i][j].B >> 4);
            hist.push_back(color);
            /* This sets the initial contents of the vector of pixel
             * objects.
             */
            if (i % 2 == 0 && j % 2 == 0)
            {
                pixlist.push_back(pixel(i, j));
            }
        }
    }
    // A random number generator seeded with `seed` (the length of the key)
    rnumgen RNG(seed);
    // Initializes the random number generator
    RNG.pdf(hist);
    /* Using the random number generator, two 12-bit numbers are
     * generated and combined to form a 24-bit number. This 24-bit number
     * is used to randomize the contents of the vector of pixels.
     */
    int r1_12 = RNG.rand();
    int r2_12 = RNG.rand();
    int r24 = (r1_12 << 12) | r2_12;
    /* The randomization of the vector of pixels.
     * Pixel elements are swapped within the vector using
     * the 24-bit random number as a guide.
     */
    for (i = (int)pixlist.size() - 1; i > 0; --i)
    {
        swap(pixlist[i], pixlist[r24 % (i+1)]);
    }
    return;
}

/* This function encodes the message read from stdin into the
 * ppm object that is passed.
 */
void encode(ppm &img, string key)
{
    vector<pixel> pixlist;
    /* The `set_pixel_list` function is called to generate
     * the vector of pixels that will be used to control which
     * bytes from the ppm object are used in the encoding.
     */
    set_pixel_list(pixlist, img, (int)key.length());
    int row, col;
    // i, s, and color are simply integers to control the encoding.
    int i = 0;
    int s;
    int color = 0;
    // c is used to store the single characters extracted from stdin.
    char c;
    /* k is used to store the single characters extracted from the key.
     * It is given a default value of 0x00 so that it will do nothing
     * if a key wasn't provided.
     */
    char k = 0x00;
    stringstream stream(key); 
    while (1)
    {
        /* The next character of the message is extracted from stdin.
         * If the character is the EOF character, c will be reset to
         * ETX (the character specifying end-of-message in the encoding.
         */
        c = cin.get();
        if (cin.eof())
        {
            c = ETX;
        }
        /* If a key was provided, the next character of the key is extracted.
         * However, if that character is the EOF, the stringstream is reset, and
         * k is set to the first character of the key. If a key was not provided,
         * the value of k remains 0x00.
         */
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
        // The message character and key character are XORed to provide extra protection.
        c = c ^ k;
        /* This for loop encodes each bit of c into the correct color of the
         * correct pixel. The variable `i` is combined with the vector of pixels
         * to produce the ppm row and column of the pixel to be used. The `color`
         * variable is used to dictate which of the three color chars will be used
         * in the encoding. Bit-shifting and boolean algebra are used to make the
         * desired bit the LSB of the correct color.
         */
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
        /* If the ETX character was just encoded, the message is fully
         * encoded, and the code breaks out of the while loop.
         */
        if (cin.eof())
        {
            break;
        }
    }
    return;
}

/* This function decodes the message from the
 * ppm object that is passed.
 */
void decode(ppm &img, string key) 
{
    vector<pixel> pixlist; 
    /* The `set_pixel_list` function is called to generate
     * the vector of pixels that will be used to control which
     * bytes from the ppm object are used in the decoding.
     */
    set_pixel_list(pixlist, img, (int)key.length());
    int row, col;
    // i, s, and color are simply integers to control the encoding.
    int i = 0;
    int s;
    int color = 0;
    // c is used to store the single characters extracted from the ppm object.
    char c;
    /* k is used to store the single characters extracted from the key.
     * It is given a default value of 0x00 so that it will do nothing
     * if a key wasn't provided.
     */
    char k = 0x00;
    stringstream stream(key);
    while (1)
    {
        /* The message character is set to 0x00 so that its
         * unextracted contents will not interfere with the
         * extraction.
         */
        c = 0x00;
        /* This for loop decodes each bit of c from the correct color of the
         * correct pixel. The variable `i` is combined with the vector of pixels
         * to produce the ppm row and column of the pixel to be used. The `color`
         * variable is used to dictate which of the three color chars will be used
         * in the encoding. Bit-shifting and boolean algebra are used to extract the
         * correct bit of the message character.
         */
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
        /* If a key was provided, the next character of the key is extracted.
         * However, if that character is the EOF, the stringstream is reset, and
         * k is set to the first character of the key. If a key was not provided,
         * the value of k remains 0x00.
         */
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
        // The message and key characters are XORed to reverse the XOR in `encode`.
        c = c ^ k;
        /* If the character extracted was ETX, the extraction is complete,
         * and the while loop is broken.
         * Otherwise, the extracted character is printed to the command line.
         */
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
    /* If there are not 2 or 3 user-provided command line arguments,
     * an error message is printed, and the program exits.
     */
    if (argc < 3 || argc > 4)
    {
        fprintf(stderr, "Usage: ./Crypto -encode|decode [-key=\"text\"] image.ppm\n");
        return -5;
    }
    /* If the first user-provided command line argument is not "-encode" or
     * "-decode", an error message is printed, and the program exits.
     */
    if (strcmp(argv[1], "-encode") != 0 && strcmp(argv[1], "-decode") != 0)
    {
        fprintf(stderr, "Usage: ./Crypto -encode|decode [-key=\"text\"] image.ppm\n");
        return -6;
    }
    string fname, key;
    /* If there are only 2 user-provided command line arguments,
     * the name of the file to be used in encoding or decoding is obtained
     * from argv[2], and the key is set to be an empty string.
     */
    if (argc == 3)
    {
        fname = argv[2];
        /* If the file is not a ppm file, an error message
         * is printed, and the program exits.
         */
        if (fname.substr(fname.length() - 4) != ".ppm")
        {
            fprintf(stderr, "Usage: ./Crypto -encode|decode [-key=\"text\"] image.ppm\n");
            return -7;
        }
        string key = "";
    }
    /* If there are 3 user-provided command line arguments,
     * the name of the file to be used in encoding or decoding is obtained
     * from argv[3], and the key is obtained by generating a subtring from
     * argv[2].
     */
    else
    {
        fname = argv[3];
        /* If the file is not a ppm file, an error message
         * is printed, and the program exits.
         */
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
    // This call parses the contents of the ppm file into the ppm object.
    img.read(fname);

    /* If the program is running in encode mode, the encode function is called,
     * followed by the ppm object's write function.
     * Otherwise, if the program is running in decode mode, the decode function
     * is called.
     */
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
