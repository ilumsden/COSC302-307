#ifndef __SUPPORT_H__
#define __SUPPORT_H__

#include <string>
#include <vector>
#include <cstdlib>

/* support.h
 * Author: Ian Lumsden
 * Date: February 26, 2018
 *
 * This file contains the declarations of all the classes that are
 * used in the stenography performed by Crypto.cpp. Because the
 * `matrix` class is templated, all of its code is included in this
 * file. Additionally, the code for the RGB and pixel structs are also
 * included in this file because they are easily written in-line.
 */

/* `uchar` is simply used to avoid having to spell out 
 * `unsigned char`. ETX is a constant char that is used
 * to specify the end of the encoded message.
 */
typedef unsigned char uchar;
extern const char ETX;

/* The matrix class is used to store the actual data from the
 * ppm. It is initialized first as an array of T pointers. Then,
 * a contiguous array of T objects is instantiated. Each element of the
 * array of T pointers is linked so that they point to the correct element
 * of the contiguous array (i.e. buf[0] points to the contiguous array element
 * at index 0). Several operators and functions are also defined to allow for
 * easy access of the data.
 */
template <typename T>
class matrix 
{
    public:
        matrix();
        ~matrix();
        void assign(int n_Nrows, int n_Ncols);
        // This function simply returns the value of `Nrows`.
        int get_Nrows() const { return Nrows; }
        // This function simply returns the value of `Ncols`.
        int get_Ncols() const { return Ncols; }
        /* This function returns the value of one of the T pointer in
         * `buf` corresponding to the parameter i.
         */
        T* operator[] (int i) { return buf[i]; }
        /* This function returns a pointer to the begining of the
         * data stored in the matrix object. This allows other
         * functions to easily access the private data of the matrix
         * class.
         */
        T * data() const { return buf[0]; }
    private:
        // The number of rows and columns in the data (as read from the ppm).
        int Nrows, Ncols;
        // The internal data of the matrix
        T **buf;
};

/* The constructor of the matrix class.
 * It simply sets all members to their default values.
 */
template <typename T>
matrix<T>::matrix()
{
    Nrows = 0;
    Ncols = 0;
    buf = NULL;
}

/* The deconstructor of the matrix class.
 * First, it deletes the contiguous array of T objects.
 * Then, it deletes the array of T pointers.
 */
template <typename T>
matrix<T>::~matrix()
{
    delete [] buf[0];
    delete [] buf;
}

/* This function initializes the matrix using the parameters
 * passed.
 */
template <typename T>
void matrix<T>::assign(int n_Nrows, int n_Ncols)
{
    // The members Nrows and Ncols are set using the parameters.
    Nrows = n_Nrows;
    Ncols = n_Ncols;
    /* The array of T pointers and the array of T objects are both
     * dynamically created.
     */
    buf = new T *[Nrows];
    buf[0] = new T [Nrows*Ncols];
    /* This for loop links the T pointers with their corresponding
     * T objects in continuous memory.
     */
    for (int i = 1; i < Nrows; i++)
    {
        buf[i] = buf[i-1] + Ncols;
    }
}

/* This struct is used to easily specify a (row, column) ordered pair
 * with respect to a matrix object.
 */
struct pixel
{
    pixel(int n_row=0, int n_col=0) { row = n_row; col = n_col; }
    int row, col;
};

/* This struct is used to store the RGB values for a single pixel that
 * are obtained from the ppm.
 */
struct RGB
{
    RGB(char nR=0, char nG=0, char nB=0) { R = nR; G = nG; B = nB; }
    uchar R, G, B;
};

/* The ppm class to store the data read from a ppm image.
 * It stores the data in a matrix object of RGB items.
 */ 
class ppm
{
    public:
        // The dufault constructor for the ppm object.
        ppm() { ; }
        // The defualt deconstructor for the ppm obgect.
        ~ppm() { ; }
        // See support.cpp for read and write.
        void read(std::string fname);
        void write(std::string fname);
        // Returns the RGB pointer returned by a call to image[i] (from the matrix class)
        RGB * operator[] (int i) { return image[i]; }
        // Returns the number of rows of the ppm (obtained from the matrix object).
        int get_Nrows() { return image.get_Nrows(); }
        // Returns the number of columns of the ppm (obtained from the matrix object).
        int get_Ncols() { return image.get_Ncols(); }
    private:
        // An internal matrix of RGB structs used to store the data obtained from the ppm
        matrix<RGB> image;
};

/* This class is an implementation of a random number generator controlled by
 * the contents of a vector.
 */
class rnumgen
{
    public:
        // The rnumgen constructor. It simply seeds the random number generator
        // with the parameter.
        rnumgen(int seed=0) { srand(seed); }
        // See support.cpp for pdf and rand.
        void pdf(const std::vector<int> &);
        int rand() const;
    private:
        std::vector<float> F;
};

#endif
