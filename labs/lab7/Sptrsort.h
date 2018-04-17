#include <algorithm>
#include <vector>

template <typename T>
class sptr 
{
    public:
        sptr(T *_ptr=NULL) { ptr = _ptr; }
        bool operator<(const sptr& rhs) const { return *ptr < *rhs.ptr; }
        operator T*() const { return ptr; }
    private:
        T* ptr;
};

template <typename T>
void sptrsort(std::vector<T *> &A) 
{
    std::vector< sptr<T> > sa;
    for (int i = 0; i < (int)(A.size()); i++)
    {
        sa.push_back(sptr(A[i]));
    }
    std::sort(sa.begin(), sa.end());
    for (int i = 0; i < (int)(sa.size()); i++)
    {
        A[i] = *sa[i];
    }
}
