# Lab 4

## Introduction

ACME Shipping needs your help! They have obtained the rights to fly their planes into a number of cities across the world. Use your newly acquired knowledge about graphs to write a program called "citysim" that can tell ACME Shipping what the shortest valid route is between any two cities. 

To help you get started, run the Hydra script `home/cs302/labs/lab4/copy` to obtain the provided files. Your job is to write the citysim program and make it behave as described next.

While you have some design freedom, you must base your code on the classes and functions discussed next. How you tie them all together is up to you. Also, floating-point computations should be done using floats, not doubles. Most math functions take and produce doubles. That's fine. Feed them floats and assign the output to floats.

You might find it helpful to take a glance at the citylist.txt snippet included below before you continue reading.

## Citysim1

__Citysim1__ should implement a city class that stores the name (string), type (string), zone (int), latitude (float), longitude (float), and population (int). Make these variables private data members. Add `get_variable()` functions for each variable. Overload the input and output operators to facilitate `fin >> city` and `fout << city` type code. The city type is either REGIONAL or GATEWAY. We discuss these in more detail below.

Next, implement function `read_cityinfo()` which reads the "citylist.txt" file in the current directory into a `vector<city>` array using the overloaded city input operator. Empty lines or lines that begin with a hash symbol (#) should be ignored. Also implement function `write_cityinfo()` which writes the contents of the `vector<city>` array to a file called "cityinfo.txt" in the current directory if the program is given a `-write_info` option on the commandline. See below for example output. The name field is 20 characters wide. The type and population fields are 12 and 10 characters wide, respectively. The zone is 2 characters wide. The latitude and longitude fields are 8 characters wide and the output is restricted to two decimal places.

__Note:__ The latitude and longitude values are in degrees in "citylist.txt" and should be in degrees in "cityinfo.txt". However, your program will need them to be in radians for internal use. Perform the necessary conversion when you read and write these variables. Recall that `radians=degrees*(pi/180)` and `degrees=radians*(180/pi)`.

## Citysim2

__Citysim2__ should implement a dtable class which computes and contains a table of distances between all cities. Since the distance from A to B is the same as from B to A, store this table in a 1D array that holds the diagonal and lower triangle of the full distance matrix; you don't need the diagonal but including it simplifies the implementation (see the answerkey for Exam 1). Overload the function operator so you can exploit the distance symmetry and return the same value for `distance(i,j)` as for `distance(j,i)` by looking the appropriate value in the 1D array. __Hint:__ Have the overloaded function operator swap arguments i and j if `i < j` before doing a lookup.

Use the [great-circle distance](http://en.wikipedia.org/wiki/Great-circle_distance) when computing the distance between two cities. This is the shortest distance between them on a sphere representing the Earth which we will assume has a radius of 3982 miles. Don't try to be clever and use a more accurate radius as it will throw your output off relative to the solution code. Also, use the Haversine Formula for your computation.

Round the distances to the closest 5.0 mile distance. This will make it easier to compare your distances with those produced by the solution code. __Hint:__ Set `distance = 5.0*round(distance/5.0).`

Next, implement function `write_citydtable()` which writes the contents of the distance table to a file called "citydtable.txt" in the current directory if the program is given a `-write_dtable` option on the commandline. See below for example output. Your output must match the layout. __Hint:__ The width of the text string is `2*w+4` where w is the length of the longest city name. Use `setfill()` to create the ellipses.

## Citysim3

__Citysim3__ should implement function `create_citygraph()` which determines which cities are adjacent to one another (connected) thereby implicitly creating a graph. The rules for adjacency are as follows.
* A REGIONAL city is adjacent to all other REGIONAL cities in the same zone. A REGIONAL city is also adjacent to the closest GATEWAY city in the same zone.
* A GATEWAY city is adjacent to all other GATEWAY cities in the same zone. A GATEWAY city is also adjacent to the closest GATEWAY in each of the other zones if the distance is 6,000 miles or less.

__Hint:__ First extract an index list for each type of city. Then iterate through these lists following the above rules. Use the indices to look up the pertinent information in `the vector<city>` array discussed above. In fact, think of these indices as pointers.
