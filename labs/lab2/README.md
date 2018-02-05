# Lab 2:

## Provided Materials:
To help you get started, run the Hydra script `/home/cs302/labs/lab2/copy` to obtain the following files: `Sort_sptr.cpp` (skeleton code), `sort_sptr` (Hydra solution executable), and a makefile. Your job is to write the missing source code which must behave as described next. Use the data files from Lab 1.

## Instructions:

Get a copy of the `Quicksort.cpp` file you submitted for Lab 1. Update `Sort_sptr.cpp` to include the definition of class `person_t` including the associated input, output, and less-than operators. Also copy and add the no-argument command-line check as well as the template based `printlist` function. Write the missing `slist::node::constructor` that takes a data argument; this functionality is needed by `slist::push_back()` when it creates a new node. Include header files and do whatever else it takes for the code to compile (shouldn't be much), then test and make sure you can read and write data from the lists provided.

Next, add an overloaded less-than comparison operator to the `node` subclass. Embed the smart pointer code as a private `slist` subclass; copy the code verbatim from the `smartpointer_handout` (lecture material), then modify it to explicitly work for `slist::node` data.

Update the `slist::sort()` function to sort the single linked list data using the `std::sort()` function. Determine the number of elements stored in the list. Allocate a vector of smart pointers of that size called `Ap`. Initialize the individual smart pointers to point to the list nodes. For example, if `p = head`, then `Ap[0] = p->next`, etc. Apply `std::sort` and use the sorted `Ap` vector to relink the list nodes. Do not simply repeat the array based rearrangement covered in class which used pointers to the data itself. Here, the smart pointers point to nodes that hold the data. Also, keep in mind that `operator[]` is not available. You will need to use list node pointers. When the sorting is done, step through the smart pointer vector while creating the linked list. For example, if `p = head`, then `p->next = Ap[0]`, etc. Simple as that (once you figure it out). You may want to draw a picture of what's going on to help visualize the process.

## Example Output:

```
unix> cat list1.txt

CANDACE WITT        	250-939-5404
THEODORE PERKINS    	723-668-3397
THAD HULL           	708-807-6757
STEPHAN SALAZAR     	415-413-5058
ISRAEL WILKINS      	938-701-1455
BRUCE PERRY         	540-916-2956
VALENTIN RIVERS     	726-204-2377
WILFRED JOHNSTON    	582-126-8861
JORDAN WILKINS      	938-701-1455
LEVI SPENCE         	985-365-7415
KAYLA NGUYEN        	484-322-1527
```

```
unix> ./sort_slist < list1.txt

HULL THAD               708-807-6757
JOHNSTON WILFRED        582-126-8861
NGUYEN KAYLA            484-322-1527
PERKINS THEODORE        723-668-3397
PERRY BRUCE             540-916-2956
RIVERS VALENTIN         726-204-2377
SALAZAR STEPHAN         415-413-5058
SPENCE LEVI             985-365-7415
WILKINS ISRAEL          938-701-1455
WILKINS JORDAN          938-701-1455
WITT CANDACE            250-939-5404
```
