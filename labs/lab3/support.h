#ifndef __SUPPORT_H__
#define __SUPPORT_H__

#include <string>

typedef unsigned char uchar;
extern const char ETX;

template <typename T>
class matrix 
{
    public:
        matrix();
        ~matrix();
        void assign(int n_Nrows, int n_Ncols);
        int get_Nrows() const { return Nrows; }
        int get_Ncols() const { return Ncols; }
        T* operator[] (int i) { return buf[i]; }
        T * data() const { return buf[0]; }
    private:
        int Nrows, Ncols;
        T **buf;
};

template <typename T>
matrix<T>::matrix()
{
    Nrows = 0;
    Ncols = 0;
    buf = NULL;
}

template <typename T>
matrix<T>::~matrix()
{
    delete [] buf[0];
    delete [] buf;
}

template <typename T>
void matrix<T>::assign(int n_Nrows, int n_Ncols)
{
    Nrows = n_Nrows;
    Ncols = n_Ncols;
    buf = new T *[Nrows];
    buf[0] = new T [Nrows*Ncols];
    for (int i = 1; i < Nrows; i++)
    {
        buf[i] = buf[i-1] + Ncols;
    }
}

struct pixel
{
    pixel(int n_row=0, int n_col=0) { row = n_row; col = n_col; }
    int row, col;
};

struct RGB
{
    RGB(char nR=0, char nG=0, char nB=0) { R = nR; G = nG; B = nB; }
    uchar R, G, B;
};

class ppm
{
    public:
        ppm() { ; }
        ~ppm() { ; }
        void read(std::string fname);
        void write(std::string fname);
        RGB * operator[] (int i) { return image[i]; }
        int get_Nrows() { return image.get_Nrows(); }
        int get_Ncols() { return image.get_Ncols(); }
    private:
        matrix<RGB> image;
};

#endif
