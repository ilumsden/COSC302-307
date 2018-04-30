#include <algorithm>
#include <vector>

/*
 * Sptrsort.h
 * Author: Ian Lumsden
 * Date: 4/30/18
 *
 * This header file defines a smart pointer class and
 * and smart pointer-based sorting algorithm for a vector of pointers.
 * The contents of this file are used to sort the vector of
 * person pointers created in Data_processors.cpp.
 */

/* This class defines a basic, templated smart pointer
 * with basic less-than comparison and dereferencing overloads.
 * The contents of this class were obtained from "sorting_handout6"
 * from Canvas. Therefore, they will not be further commented.
 */
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

/* This function uses the smart pointer class above to
 * sort the vector of standard pointers referenced by the
 * iterator parameters.
 */
template <typename T>
void sptrsort(typename std::vector<T*>::iterator start, typename std::vector<T*>::iterator end) 
{
    /* Creates a vector of smart pointers for sorting,
     * and fills it by looping using the passed iterators.
     */
    std::vector< sptr<T> > sa;
    typename std::vector<T*>::iterator tmp = start;
    while (tmp != end)
    {
        sa.push_back(sptr<T>(*tmp));
        ++tmp;
    }
    // Sorts the smart pointer vector with the STL sort function.
    std::sort(sa.begin(), sa.end());
    /* Sorts the contents of the vector of normal pointers
     * (represented by the iterator parameters) using the contents
     * of the sorted vector of smart pointers.
     */
    typename std::vector< sptr<T> >::iterator ptr_start = sa.begin();
    while (start != end && ptr_start != sa.end())
    {
        *start = (*ptr_start);
        ++start;
        ++ptr_start;
    }
}
