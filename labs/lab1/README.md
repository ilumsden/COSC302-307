# Lab 1:

The code for Lab 1 was developed in stages as described below.

## Provided Materials:
To help you get started, run the Hydra script `/home/cs302/labs/lab1/copy` to obtain the following files: `Quicksort.cpp` (skeleton code), `quicksort` (Hydra solution executable), `list1.txt`-`list3.txt` (data files), and a makefile. Your job is to write the missing source code which must behave as described next. 

## Part A:

### Instructions:
* **Part A Vers. 1:** Use your favorite editor to modify `Quicksort.cpp` as follows. Update the `quicksort()` function to accept the arguments indicated in the `main()` function, namely, a vector reference and two integers. Leave the function body empty. For now, `quicksort()` is merely a stub. Focus instead on adding the code for the input and output operators associated with the `person_t` class which has three private data members: `firstname`, `lastname`, and `phonenumber`. The input operator simply reads each of these every time it is called. The output operator prints each `person_t` object using the format: "lastname firstname phonenumber". Part of the assignment is to figure out how to maintain a fixed width if the name field so that all phonenumbers are left justified to the same column position. Finally, add the `printlist()` function from the `arraylist_handout` (lecture material) modified to print to `stdout` instead of a named file, and do whatever else is needed to make the program compile such as adding missing header files. Then test and make sure it reads and writes the `person_t` data as required. 
* **Part A Vers. 2:** Add an overloaded `operator<` for the `person_t` class so you can impose an order on such objects. Test that it works by having the `main()` function use `std::sort` before writing the list content to `stdout`. When you are satisfied that this works, replace `std::sort` with a modified quicksort algorithm. That is, instead of the median-of-three partition algorithm you saw in class and used in HW2, randomly pick an index in the left-to-right range and use it to partition the data. This new version forces you to think about how the median-of-three code works as it must explicitly check and prevent going out-bounds during the search for data pairs to swap. To make you think even more about the algorithm, merge all the functionality into the `quicksort()` function. That is, do not have an external function that carries out the partitioning. Keep it all in one function that gets called from the `main()` function. Test and compare the output with what you obtained using `std::sort`. The two output should be identical since they use the same comparison operator.

## Part B:

### Instructions:
* **Part B Vers. 1:** In preparation for the quickselect based sublist sorting, add command-line argument checks and processing. The basic version takes no arguments and executes the code from Part A. However, if two integers are provided, they specify the range of the array to be sorted, say `k1` to `k2`. You must check that the program is being executed in one of these two modes. For the second mode, you must convert the two command-line arguments into integers. Hint: the `atoi()` function from the C standard library (`cstdlib`) can do the job.
* **Part B Vers. 2:** Copy the `quickselect()` function from the sorting3 handout (lecture material). Embed the random pivot based partition code that you added to `quicksort()` in Part A. The function will take four arguments, e.g. `quickselect(A, left, k, right)` where `A` is a data array, `left` and `right` designate the first and the last indices to consider (the range), and `k` the element that needs to be selected (found and put in its proper place). Make whatever other modifications are needed to get the code to compile. Test the program to make sure you the selection is correct. Use `quickselect(A, 0, k0, N-1)` to put the `k0`-th element in its proper location and partition the data in `A` accordingly; Then use `quickselect(A, k0, k1, N-1)` to put the `k1`-th element in its proper location and partition the data in `A` accordingly. Note that data indexed below `k0` is ignored at this point. When this works, use the `quicksort()` function to sort the data in the `k0`-`k1` range.

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
unix> ./quicksort < list1.txt 
HULL THAD               708-807-6757       0
JOHNSTON WILFRED        582-126-8861       1
NGUYEN KAYLA            484-322-1527       2
PERKINS THEODORE        723-668-3397       3
PERRY BRUCE             540-916-2956       4
RIVERS VALENTIN         726-204-2377       5
SALAZAR STEPHAN         415-413-5058       6
SPENCE LEVI             985-365-7415       7
WILKINS ISRAEL          938-701-1455       8
WILKINS JORDAN          938-701-1455       9
WITT CANDACE            250-939-5404       10
```

```
unix> ./quicksort 3 6 < list1.txt
HULL THAD               708-807-6757       0
NGUYEN KAYLA            484-322-1527       1
JOHNSTON WILFRED        582-126-8861       2
PERKINS THEODORE        723-668-3397       3 **
PERRY BRUCE             540-916-2956       4 **
RIVERS VALENTIN         726-204-2377       5 **
SALAZAR STEPHAN         415-413-5058       6 **
SPENCE LEVI             985-365-7415       7
WILKINS ISRAEL          938-701-1455       8
WILKINS JORDAN          938-701-1455       9
WITT CANDACE            250-939-5404       10
```

**Note:** The line numbers added to right for the last output are for illustrational purposes and not produced by the program. They help you see that the data in lines 3-6 is sorted (by design), that the data lines 1-2 is not, and that the data in lines 0, 7-10 is (by chance). 
