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
void sptrsort(typename std::vector<T*>::iterator start, typename std::vector<T*>::iterator end) 
{
    std::vector< sptr<T> > sa;
    typename std::vector<T*>::iterator tmp = start;
    while (tmp != end)
    {
        sa.push_back(sptr<T>(*tmp));
        ++tmp;
    }
    std::sort(sa.begin(), sa.end());
    typename std::vector< sptr<T> >::iterator ptr_start = sa.begin();
    while (start != end && ptr_start != sa.end())
    {
        *start = (*ptr_start);
        ++start;
        ++ptr_start;
    }
}
