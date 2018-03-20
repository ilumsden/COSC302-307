#include <algorithm>
#include <cfloat>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <map>
#include <numeric>
#include <queue>
#include <sstream>
#include <stack>
#include <utility>
#include <vector>

using namespace std;

/*
 * Citysim.cpp
 * Author: Ian Lumsden
 * Date: March 20, 2018
 *
 * This program reads data from the "citylist.txt" file in the CWD of the executable.
 * It then does various things depending on the flags provided:
 *     * "-write_info": the program will print the data read from "citylist.txt" into "cityinfo.txt" with special formatting
 *     * "-write_dtable": the program calculates the distances between all the cities and print this data into "citydtable.txt" 
 *     * "-write_graph": the program creates a graph of the cities and prints the graph's data to "citygraph.txt"
 *     * "-randomseed": when using the "-mode_bfs" or "-mode_dijkstra" flags, the user can specify a random city with "*". This flag will cause the random number generator that chooses a city for a "*" to be seeded with `time(NULL)` instead of 0.
 *     * "-mode_bfs": the program will determine the shortest route in terms of number of graph edges traversed between the cities specified by the user.
 *     * "-mode_dijkstra": the program will determine the shortest route in terms of distance travelled between the cities specified by the user.
 *     * "-show": the program will print the details of the BFS or Dijkstra route to stdout.
 */

// These constants are used for comparison and basic calculation purposes.
const string REG = "REGIONAL";
const string GAT = "GATEWAY";
const float PI = 3.1415926535897932384626433;

// A class for storing the data to define a city.
class city
{
    public:
        friend istream & operator>>(istream &, city &);
        friend ostream & operator<<(ostream &, const city &);
        /* Various accessor methods to allow external code to
         * access the private members.
         */
        string get_name() const { return name; }
        string get_type() const { return type; }
        int get_zone() const { return zone; }
        int get_population() const { return pop; }
        float get_latitude() const { return latitude; }
        float get_longitude() const { return longitude; }
    private:
        /* name is the name of the city.
         * type is the type of the city and is either REGIONAL or
         * GATEWAY.
         */
        string name, type;
        /* zone specifies the zone where the city is.
         * pop is the population of the city.
         */
        int zone, pop;
        float latitude, longitude;
};

// This function parses a line from a citylist.txt into a city object.
istream & operator>>(istream &fin, city &place)
{
    stringstream sin;
    string tmp;
    /* This while loop ensures the line being parsed is not
     * empty or commented out.
     */
    while (getline(fin, tmp))
    {
        if (tmp[0] != '#' && tmp != "")
        {
            break;
        }
    }
    /* This ensures that the function doesn't try to read extra data after
     * the End-of-File is reached.
     */
    if (fin.eof())
    {
        return fin;
    }
    /* A string stream is used to parse the data from the line into the 
     * corresponding members of the city object. Before being added to the
     * city object, the latitude and longitude are converted to radians.
     */
    sin.str(tmp);
    float lati, longi;
    sin >> place.zone >> place.name >> place.type >> lati >> longi >> place.pop;
    lati = lati*(PI / 180);
    longi = longi*(PI / 180);
    place.latitude = lati;
    place.longitude = longi;
    return fin;
}

ostream & operator<<(ostream &fout, const city &place)
{
    // The latitude and longitude are converted from radians to degrees.
    float lati = place.latitude * (180/PI);
    float longi = place.longitude * (180/PI);
    // This prints the city object's data out with correct formatting.
    fout << setw(20) << left << place.name << " "
         << setw(12) << left << place.type << " "
         << setw(2) << right << place.zone << " "
         << setw(10) << right << place.pop << " "
         << setw(8) << setprecision(2) << fixed << right << lati << " "
         << setw(8) << setprecision(2) << fixed << right << longi << "\n";
    return fout;
}

/* This class stores the distances between cities.
 * These distances are accessed as if the data was stored
 * in a matrix.
 */
class dtable
{
    public:
        dtable(vector<city> &);
        // Custom destructor to prevent memory leaks.
        ~dtable() { delete [] dist; }
        /* This function allows the data from the dtable object
         * to be accessed using 2D array accessing notation.
         */
        float * operator[](int i) const { return &dist[(i*(i+1))/2]; }
        float operator()(int, int) const;
    private:
        float *dist;
};

dtable::dtable(vector<city> &citylist)
{
    /* The internal storage is sized so that a single
     * vector can be used to emulate a matrix.
     */
    dist = new float [(citylist.size()*(citylist.size()+1))/2];
    float lat1, lat2, long1, long2;
    float centangle, distance;
    /* This for loop calculates the great-circle distance between
     * all the pairs of cities in `citylist` and stores those distances
     * in the dtable object.
     */
    for (int i = 0; i < (int)(citylist.size()); i++)
    {
        for (int j = i; j >= 0; j--)
        {
            lat1 = citylist[i].get_latitude();
            lat2 = citylist[j].get_latitude();
            long1 = citylist[i].get_longitude();
            long2 = citylist[j].get_longitude();
            centangle = sqrt(pow(sin((lat1-lat2)/2), 2) + cos(lat1)*cos(lat2)*pow(sin((long1-long2)/2), 2));
            centangle = 2*asin(centangle);
            distance = centangle * 3982;
            distance = 5.0*round(distance/5.0);
            (*this)[i][j] = distance;
        }
    }
    return;
}

/* This function allows the dtable object to be used like
 * a function to access the distance between two cities.
 */
float dtable::operator()(int i, int j) const
{
    if (i < j)
    {
        int tmp = j;
        j = i;
        i = tmp;
    }
    return (*this)[i][j]; 
}

/* This function opens the file specified by `fname` and uses the
 * overloaded >> operator for city objects to parse the file's data
 * into the `citylist` vector. This function also creates a map that
 * associates the names of the cities with their indicies for `citylist`.
 */
void read_cityinfo(string fname, vector<city> &citylist, map<string, int> &name_dict)
{
    /* Empties the `citylist` vector and `name_dict`
     * map if they contain anything.
     */
    if (!citylist.empty())
    {
        citylist.clear();
    }
    if (!name_dict.empty())
    {
        name_dict.clear();
    }
    // Opens the file in read-only mode.
    fstream fin(fname.c_str(), ios::in);
    // Throws an error is the file failed to open.
    if (!fin.is_open())
    {
        fprintf(stderr, "Unable to open %s\n", fname.c_str());
        fin.close();
        exit(-1);
    }
    city tmp;
    int i = 0;
    /* This while loop parses the contents of a single
     * city and adds it to `citylist`. It also adds the 
     * city and its index to the map.
     */
    while (fin >> tmp)
    {
        /* This is a redundancy to ensure that extra data
         * is not read.
         */
        if (fin.eof())
        {
            break;
        }
        citylist.push_back(tmp);
        name_dict.insert(make_pair(tmp.get_name(), i));
        i++;
    }
    fin.close();
    return;
}

/* This function prints the data of the cities in `citylist`
 * to a file called `cityinfo.txt`.
 */
void write_cityinfo(const vector<city> &citylist)
{
    string fname = "cityinfo.txt";
    /* If there are no cities to print the info for,
     * the function simply returns without doing
     * anything.
     */
    if (citylist.empty())
    {
        return;
    }
    // Opens (and creates) the `cityinfo.txt` file in write-only mode.
    fstream fout(fname.c_str(), ios::out);
    // Throws an error if the file cannot be openned.
    if (!fout.is_open())
    {
        fprintf(stderr, "Unable to open/create %s\n", fname.c_str());
        fout.close();
        exit(-2);
    }
    // Prints the header info.
    fout << "CITY INFO (N=" << (int)(citylist.size()) << "):\n\n";
    // `linenum_width` is used to control the width of the city number.
    int linenum_width = floor(log10((int)(citylist.size()))) + 1;
    // Prints the city number and city data for each city in `citylist`.
    for (int i = 0; i < (int)(citylist.size()); i++)
    {
        fout << " " << setw(linenum_width) << right << i << " ";
        fout << citylist[i];
    }
    fout.close();
    return;
}

/* A simple helper function that determines the longest city name
 * of the cities in `citylist`.
 * Used to help with formatting.
 */
int longest_name(const vector<city> &citylist)
{
    int length = 0;
    for (int i = 0; i < (int)(citylist.size()); i++)
    {
        if ((int)(citylist[i].get_name().length()) > length)
        {
            length = citylist[i].get_name().length();
        }
    }
    return length;
}

/* A simple helper function that determines the longest distance
 * between cities.
 * Used to help with formatting.
 */
int longest_distance(const dtable &dist, const int size)
{
    int length;
    int max_length = 0;
    for (int i = 0; i < size; i++)
    {
        for (int j = i; j >= 0; j--)
        {
            length = floor(log10(dist(i, j))) + 1;
            if (length > max_length)
            {
                max_length = length;
            }
        }
    }
    return max_length;
}

/* This function writes the formatted contents of a dtable object
 * to a "citydtable.txt" file.
 */
void write_citydtable(const vector<city> &citylist, const dtable &dist)
{
    string fname = "citydtable.txt";
    // Opens and creates the "citydtable.txt" file.
    fstream fout(fname.c_str(), ios::out);
    // Throws an error if the file could not be openned/created.
    if (!fout.is_open())
    {
        fprintf(stderr, "Unable to open/create %s\n", fname.c_str());
        fout.close();
        exit(-4);
    }
    /* Obtains multiple integers for formatting purposes.
     * `width` is the length of the longest name.
     * `mile_length` is the textual length of the largest distance.
     * `linenum_width` is used to control the width of the city number.
     */
    int width = longest_name(citylist);
    int mile_length = longest_distance(dist, (int)(citylist.size()));
    string name, distance;
    int linenum_width = floor(log10((int)(citylist.size()))) + 1;
    // Prints the header.
    fout << "DISTANCE TABLE:\n\n";
    /* Goes through each pair of cities and prints a formatted string
     * stating the cities and the distance between them.
     */
    for (int i = 1; i < (int)(citylist.size()); i++)
    {
        for (int j = 0; j < i; j++)
        {
            name = citylist[i].get_name() + " to " + citylist[j].get_name() + " ";
            fout << " " << setw(linenum_width) << right << i << " ";
            fout << setfill('.') << setw(2*width+4) << left << name;
            fout << setw(mile_length) << setfill(' ') << right << dist(i, j) << " miles\n";
        }
        fout << "\n";
    }
    fout.close();
    return;
}

// A class used to store the adjacencies in the "graph"
class edge
{
    public:
        edge(int);
        // Custom destructor to prevent memory leak.
        ~edge() { delete [] adj; }
        /* Square bracket-access operator to allow for
         * array-based lookup of the adjacencies.
         */
        int * operator[](int i) const { return &adj[(i*(i+1))/2]; }
        void set_edge(int, int);
        int get_edge(int, int) const;
    private:
        int *adj;
};

/* Custom constructor for the edge class.
 * This function sizes the `adj` array appropriately
 * for simulated matrix-style lookup and storage.
 */
edge::edge(int num_cities)
{
    adj = new int[(num_cities*(num_cities+1))/2];
    /* This loop prevents issues that could be caused by the Hydra machines.
     * For some reason, the compiler on Hydra initializes ints to (seemingly) random
     * values rather than 0. This loop ensures that the initial values are all 0.
     */
    for (int i = 0; i < num_cities; i++)
    {
        adj[i] = 0;
    }
}

/* This function sets an adjacency between the two cities
 * specified by the int parameters.
 */
void edge::set_edge(int i, int j)
{
    if (i < j)
    {
        int tmp = j;
        j = i;
        i = tmp;
    }
    (*this)[i][j] = 1;
    return;
}

/* This function returns 1 if there is an adjacency
 * between the two cities specified by the int
 * parameters. Otherwise, it should return 0.
 */
int edge::get_edge(int i, int j) const
{
    if (i < j)
    {
        int tmp = j;
        j = i;
        i = tmp;
    }
    return (*this)[i][j]; 
}

/* This function generates a vector of ints (initially passed
 * into the function by-reference) that is filled with the 
 * citylist indices corresponding to REGIONAL cities.
 */
void get_regional_cities(vector<int> &ind, const vector<city> &citylist)
{
    for (int i = 0; i < (int)(citylist.size()); i++)
    {
        if (citylist[i].get_type() == REG)
        {
            ind.push_back(i);
        }
    }
    return;
}

/* This function generates a vector of ints (initially passed
 * into the function by-reference) that is filled with the 
 * citylist indices corresponding to GATEWAY cities.
 */
void get_gateway_cities(vector<int> &ind, const vector<city> &citylist)
{
    for (int i = 0; i < (int)(citylist.size()); i++)
    {
        if (citylist[i].get_type() == GAT)
        {
            ind.push_back(i);
        }
    }
    return;
}

/* This function fills the `zone` vector with the indicies of the gateway cities
 * separated by zones. In other words, the contents of `zones[0]` would be
 * the indicies of the gateway cities in Zone 1; the contents of `zones[1] would
 * be the indicies of the gateway cities in Zone 2; etc.
 */
void get_gateways_by_zone(vector< vector<int> > &zones, const vector<city> &citylist, const vector<int> &gate)
{
    vector<int> zonex;
    int numzones = 0;
    if (!zones.empty())
    {
        zones.clear();
    }
    // This loop determines the total number of zones.
    for (int i = 0; i < (int)(citylist.size()); i++)
    {
        if (citylist[i].get_zone() > numzones)
        {
            numzones = citylist[i].get_zone();
        }
    }
    int curr;
    /* This loop goes through all the gateway cities and adds
     * them to the vector of ints corresponding to the city's zone.
     */
    for (int i = 0; i < numzones; i++)
    {
        for (int j = 0; j < (int)(gate.size()); j++)
        {
            curr = gate[j];
            if (citylist[curr].get_zone() == i+1)
            {
                zonex.push_back(curr);
            }
        }
        zones.push_back(zonex);
        zonex.clear();
    }
    return;
}

/* This function generates the "graph" (i.e. fills the edge object)
 * by determining which cities are adjacent. It then sets the corresponding
 * element in the edge object.
 */
void create_citygraph(vector<city> &citylist, dtable &dist, edge &graph)
{
    /* The `get_regional_cities` and `get_gateway_cities` helper
     * functions are used to obtain vectors of the citylist indicies
     * for regional and gateway cities respectively.
     */
    vector<int> reg;
    vector<int> gate;
    get_regional_cities(reg, citylist);
    get_gateway_cities(gate, citylist);
    /* `base` is the current source city for determining adjacencies.
     * `curr` is the current candidate city for an adjacency.
     */
    int base, curr;
    /* `min_dist` is the smallest distance between a gateway city and `base`.
     * `min_ind` is the citylist index corresponding to `min_dist`.
     */
    float min_dist;
    int min_ind;
    // This loop controls `base`.
    for (int i = 0; i < (int)(reg.size()); i++)
    {
        base = reg[i];
        // This loop controls `curr`.
        for (int j = i+1; j < (int)(reg.size()); j++)
        {
            curr = reg[j];
            /* If `base` and `curr` (both currently regional cities) are in
             * the same zone, they are adjacent, and the corresponding element
             * in the edge object is set.
             */
            if (citylist[base].get_zone() == citylist[curr].get_zone())
            {
                graph.set_edge(base, curr);
            }
        }
        min_ind = -1;
        min_dist = FLT_MAX;
        /* This block determines the closest gateway city to `base` that
         * is in the same zone as `base`. Once this city is determined,
         * the corresponding element in the edge object is set.
         */
        for (int k = 0; k < (int)(gate.size()); k++)
        {
            curr = gate[k];
            if (citylist[base].get_zone() == citylist[curr].get_zone())
            {
                if (dist(base, curr) < min_dist)
                {
                    min_dist = dist(base, curr);
                    min_ind = curr;
                }
            }
        }
        if (min_dist != FLT_MAX && min_ind != -1)
        {
            graph.set_edge(base, min_ind);
        }
    }
    /* `zones` is used to store the output of the `get_gateways_by_zone` function.
     * `zonex` is used to store the citylist indicies of the gateway cities is zone "x".
     */
    vector< vector<int> > zones;
    vector<int> zonex;
    get_gateways_by_zone(zones, citylist, gate);
    for (int i = 0; i < (int)(gate.size()); i++)
    {
        base = gate[i];
        for (int j = i+1; j < (int)(gate.size()); j++)
        {
            curr = gate[j];
            /* If `base` and `curr` (both gateway cities) are in the same
             * zone, they are adjacent, and the corresponding element of the 
             * edge object is set.
             */
            if (citylist[base].get_zone() == citylist[curr].get_zone())
            {
                graph.set_edge(base, curr);
            }
        }
        /* This for loop determines the gateway city in each zone (excluding
         * `base`'s zone) that is closest to `base` with a distance under 6000 miles.
         * Each of these cities are considered adjacent to `base`, and the corresponding
         * elements in the edge object are set.
         */
        for (int k = 0; k < (int)(zones.size()); k++)
        {
            if (citylist[base].get_zone() == k+1)
            {
                continue;
            }
            zonex = zones[k];
            min_dist = FLT_MAX;
            min_ind = 0;
            for (int l = 0; l < (int)(zonex.size()); l++)
            {
                curr = zonex[l];
                if (dist(base, curr) < min_dist && dist(base, curr) < 6000)
                {
                    min_dist = dist(base, curr);
                    min_ind = curr;
                }
            }
            if (min_dist != FLT_MAX)
            {
                graph.set_edge(base, min_ind);
            }
        }
    }
    return;
}

/* This function prints the contents of the "graph"
 * into a file named "citygraph.txt".
 */
void write_citygraph(const vector<city> &citylist, const dtable &dist, const edge &graph)
{
    // `linenum_width` is an integer used for formating the city number.
    int linenum_width = floor(log10((int)(citylist.size()))) + 1;
    string fname = "citygraph.txt";
    // Opens and creates "citygraph.txt" in write mode.
    fstream fout(fname.c_str(), ios::out);
    // Throws an error if the file could not be openned/created.
    if (!fout.is_open())
    {
        fprintf(stderr, "Unable to open/create %s\n", fname.c_str());
        fout.close();
        exit(-5);
    }
    // Prints the header
    fout << "CITY GRAPH:\n\n";
    /* Loops through each city, and prints the city and its
     * adjacencies with correct formatting.
     */
    for (int i = 0; i < (int)(citylist.size()); i++)
    {
        fout << " " << setw(linenum_width) << right << i << " " << citylist[i].get_name() << "\n";
        for (int j = 0; j < (int)(citylist.size()); j++)
        {
            if (i == j)
            {
                continue;
            }
            if (graph.get_edge(i, j) == 1)
            {
                fout << " ";
                for (int k = 0; k < linenum_width; k++)
                {
                    fout << " ";
                }
                fout << " ";
                fout << setw(linenum_width) << right << j << " "
                     << citylist[j].get_name() << ": "
                     << dist(i, j) << " miles\n";
            }
        }
        if (i+1 != (int)(citylist.size()))
        {
            fout << "\n";
        }
    }
    return;
}

/* This function uses a BFS traversal of the "graph" to determine
 * the shortest distance between source and sink in terms of number of edges.
 * In other words, the path returned by this function will traverse the fewest
 * number of edges of any possible path, even if it is not the shortest total
 * distance.
 */
void bfs_route(int source, int sink, vector<float> &vdist, vector<int> &vlink, const vector<city> &citylist, const dtable &dist, const edge &graph)
{
    // Clears the vectors if they have any contents.
    if (!vdist.empty())
    {
        vdist.clear();
    }
    if (!vlink.empty())
    {
        vlink.clear();
    }
    /* Default values are assigned to the vectors.
     * This is necessary because these defaults are assumed in the remainder of the code.
     */
    vdist.assign(citylist.size(), FLT_MAX);
    vlink.assign(citylist.size(), -1);
    // Assigns the default values for `source`.
    vdist[source] = 0;
    vlink[source] = source;
    /* `vert` is used to store the cities under consideration for the next step of the route.
     * Initially, it only contains `source.
     */
    queue<int> vert;
    vert.push(source);
    /* For each city that is not `sink`, the city is removed from `vert`, and its adjacencies are added to `vert`. 
     * Also, the `vdist` value for the adjacencies are updated to be the total distance from `source` along the current
     * route.
     */
    while (!vert.empty())
    {
        int ind = vert.front();
        vert.pop();
        if (ind == sink)
        {
            break;
        }
        for (int k = 0; k < (int)(citylist.size()); k++)
        {
            if (graph.get_edge(ind, k) == 1)
            {
                if (vdist[k] == FLT_MAX)
                {
                    vdist[k] = vdist[ind] + dist(ind, k);
                    vlink[k] = ind;
                    vert.push(k);
                }
            }
        }
    }
    // This empties `vert` if `sink` was reached before `vert` was empty.
    while (!vert.empty())
    {
        vert.pop();
    }
}

// An enum to be used to controll vertex selection in the Dijkstra function. 
typedef enum { WHITE, BLACK } vcolor_t;

/* This function uses Dijkstra's Algorithm to determine the path
 * between source and sink with the shortest distance travelled.
 * Unlike `bfs_route`, this algorithm bases its definition of "shortest path"
 * on the actual distance of the path, rather than the number of edges traversed.
 */
void dijkstra_route(int source, int sink, vector<float> &vdist, vector<int> &vlink, const vector<city> &citylist, const edge &graph, const dtable &dist)
{
    /* `vcolor` is a vector of `vcolor_t` enums used to keep track of which nodes have already
     * been visited. If a node is WHITE, it has not yet been visited. If a node is BLACK, it has already
     * been visited.
     */
    vector<vcolor_t> vcolor;
    vcolor.assign(citylist.size(), WHITE);
    // `vdist` and `vlink` are both filled with their default, assumed values.
    if (!vdist.empty())
    {
        vdist.clear();
    }
    vdist.assign(citylist.size(), FLT_MAX);
    vdist[source] = 0;
    if (!vlink.empty())
    {
        vlink.clear();
    }
    vlink.assign(citylist.size(), -1);
    vlink[source] = source;
    while (1)
    {
        /* This block determines the next step in the route by determining the shortest distance
         * from `source`. The first time through the while loop, the city selected will always be
         * `source`. Otherwise, the city selected will be the unvisited city with the smallest distance
         * from `source`. If no city is found that meets these criteria, the function simply returns.
         */
        int i;
        int i_mindist = -1;
        float mindist = FLT_MAX;
        for (int j = 0; j < (int)(vcolor.size()); j++)
        {
            if (vcolor[j] == WHITE && mindist > vdist[j])
            {
                i_mindist = j;
                mindist = vdist[j];
            }
        }
        if ((i = i_mindist) == -1)
        {
            return;
        }
        // The color of the current city is set to BLACK to state that the city has been visited.
        vcolor[i] = BLACK;
        // The loop is broken when the route reaches `sink`.
        if (i == sink)
        {
            break;
        }
        /* For each WHITE adjacency of the current city, the adjacency's `vdist` value is set to
         * be the distance from `source` of the current city plus the distance between the current
         * city and the adjacency.
         */
        for (int k = 0; k < (int)(citylist.size()); k++)
        {
            if (graph.get_edge(i, k) == 1)
            {
                float weight = dist(i, k);
                if (vcolor[k] == WHITE)
                {
                    if (vdist[k] > vdist[i] + weight)
                    {
                        vdist[k] = vdist[i] + weight;
                        vlink[k] = i;
                    }
                }
            }
        }
    }
}

// This function prints the route data to the console.
void show_route(int source, int sink, vector<float> &vdist, vector<int> &vlink, const vector<city> &citylist, const dtable &dist)
{
    // `linenum_width` is an int used for formatting the city number.
    int linenum_width = floor(log10((int)(citylist.size()))) + 1;
    /* If the distance between `source` and `sink` is FLT_MAX, there is no path from
     * `source` to `sink`. So, an error message is printed to stdout, and the function returns.
     */
    if (vdist[sink] == FLT_MAX)
    {
        fprintf(stdout, "No path from %s to %s\n", citylist[source].get_name().c_str(), citylist[sink].get_name().c_str());
        return;
    }
    // More ints for formatting
    int totaldist_width = floor(log10((int)(vdist[sink]))) + 1;
    int name_width = longest_name(citylist) + 3;
    int seperationdist_width = longest_distance(dist, (int)(citylist.size())); 
    /* If `source` is the same as `sink`, a special line is printed to state that
     * the cities are the same. This line is printed to ensure that the output matches
     * that of the solution executable.
     */
    if (source == sink)
    {
        cout << setw(linenum_width) << right << source << " "
             << citylist[source].get_name() << " to "
             << setw(linenum_width) << right << sink << " "
             << citylist[sink].get_name() << " :  "
             << setw(totaldist_width) << right << "0"
             << " miles\n";
        return;
    }
    /* `S` is a stack used to store the cities on the route between `source` and `sink`.
     * The bottom of the stack is `sink`, and the top of the stack is `source`.
     */
    stack<int> S;
    // `D` is a stack used to store the distances from `source` for each city in `S`.
    stack<float> D;
    for (int i = sink; i != source; i = vlink[i])
    {
        S.push(i);
        D.push(vdist[i]);
    }
    S.push(source);
    D.push(vdist[source]);
    // `city1` is the index for the current city.
    int city1;
    /* `fulldist` is the distance from `source` of the current city.
     * `prevdist` is the distance from `source` of the previous city.
     */
    float fulldist, prevdist;
    while (!S.empty())
    {
        city1 = S.top();
        S.pop();
        /* This if statement ensures that there are not issues with trying to
         * perform this assignment on the first iteration, when fulldist does not have a
         * meaningful value.
         */
        if (city1 != source)
        {
            prevdist = fulldist; 
        }
        fulldist = D.top();
        D.pop();
        // Prints the base content of each line (city, full distance, etc.).
        cout << "   " << setw(totaldist_width) << right << fulldist << " miles : "
             << setw(linenum_width) << right << city1 << " "
             << setw(name_width) << left << citylist[city1].get_name();
        /* If the current city is the source, there is no additional information that needs
         * to be printed, so only a new line is printed. Otherwise, the distance between the
         * current city and the previous city is printed (fulldist - prevdist).
         */
        if (city1 == source)
        {
            cout << "\n";
        }
        else
        {
            cout << "    " << setw(seperationdist_width) << right << fulldist - prevdist
                 << " miles\n"; 
        }
    }
    cout << "\n";
}

/* This helper function calculates the total number
 * of zones in citylist.
 */
int nzones(const vector<city> &citylist)
{
    int numzones = 0;
    for (int i = 0; i < (int)(citylist.size()); i++)
    {
        if (citylist[i].get_zone() > numzones)
        {
            numzones = citylist[i].get_zone();
        }
    }
    return numzones;
}

/* This helper function determines the total population
 * of the specified zone.
 */
int zone_population(const int zone, const vector<city> &citylist)
{
    int pop = 0;
    for (int i = 0; i < (int)(citylist.size()); i++)
    {
        if (citylist[i].get_zone() == zone)
        {
            pop += citylist[i].get_population();
        }
    }
    return pop;
}

/* This helper function calculates the total population
 * of all the cities in citylist.
 */
int total_population(const vector<city> &citylist)
{
    int tpop = 0;
    for (int i = 0; i < (int)(citylist.size()); i++)
    {
        tpop += citylist[i].get_population();
    }
    return tpop;
}

/* This helper function determines the number of
 * cities in the specified zone.
 */
int num_zone_cities(const int zone, const vector<city> &citylist)
{
    int num = 0;
    for (int i = 0; i < (int)(citylist.size()); i++)
    {
        if (citylist[i].get_zone() == zone)
        {
            num++;
        }
    }
    return num;
}

/* This class is used to generate random citylist indicies
 * according to the probabilities passed in through the
 * vector of floats passed to `pdf`.
 */
class rnumgen
{
    public:
        rnumgen(int seed);
        void pdf(const vector<float> &, const vector<city> &);
        int rand() const;
    private:
        vector<float> F;
};

/* This function seeds the random number generator using
 * a manipulated version of the passed seed.
 */
rnumgen::rnumgen(int seed)
{
    char *byte = (char *)&seed;
    swap(byte[0], byte[3]);
    swap(byte[1], byte[2]);
    int seedval = *((int *)byte);
    srand(seedval);
}

/* This function sets the internal probability vector with the probabilities of
 * each city.
 */
void rnumgen::pdf(const vector<float> &v, const vector<city> &citylist)
{
    F.resize(citylist.size());
    int zone;
    for (int i = 0; i < (int)(citylist.size()); i++)
    {
        zone = citylist[i].get_zone();
        F.push_back(v[zone-1]);
    }
    partial_sum(F.begin(), F.end(), F.begin());
    transform(F.begin(), F.end(), F.begin(), bind2nd(divides<float>(), *(F.end()-1)));
}

// This function uses the internal probability vector to generate a random number.
int rnumgen::rand() const
{
    const float randnorm = RAND_MAX + 1.0f;
    const float p = (float)std::rand()/randnorm;
    return upper_bound(F.begin(), F.end(), p) - F.begin();
}

// This function creates the initial probability vector that it passes into rnumgen::pdf
void prep_rnumgen(const int numzones, const int tpop, const vector<city> &citylist, rnumgen &RNG)
{
    vector<float> problist;
    int pop, zone_cities;
    float prob;
    // This for loop calculates the probability of a city in each zone and adds it to `problist`.
    for (int i = 1; i < numzones + 1; i++)
    {
        pop = zone_population(i, citylist);
        zone_cities = num_zone_cities(i, citylist);
        prob = (((float)(pop))/tpop)*(1.0f/((float)(zone_cities)));
        problist.push_back(prob);
    }
    RNG.pdf(problist, citylist);
}

int main(int argc, char *argv[])
{
    // `flags` is an int array that specifies if a certain flag was provided.
    int flags[8];
    // If there are no user-provided command line arguments, only flags[0] is set.
    if (argc == 1)
    {
        flags[0] = 1;
    }
    else
    {
        /* This loop goes through the user-provided command line arguments and sets
         * the corresponding elements of `flags`.
         */
        string f;
        for (int i = 1; i < argc; i++)
        {
            f = argv[i];
            if (f == "-write_info")
            {
                flags[1] = 1;
            }
            else if (f == "-write_dtable")
            {
                flags[2] = 1;
            }
            else if (f == "-write_graph")
            {
                flags[3] = 1;
            }
            else if (f == "-mode_bfs")
            {
                flags[4] = 1;
            }
            else if (f == "-mode_dijkstra")
            {
                flags[5] = 1;
            }
            else if (f == "-show")
            {
                flags[6] = 1;
            }
            else if (f == "-randomseed")
            {
                flags[7] = 1;
            }
            /* If an invalid command line argument is provided, an error message is printed, and
             * the program exits.
             */
            else
            {
                fprintf(stderr, "Usage: ./Citysim -write_info|write_dtable|write_graph|mode_bfs|mode_dijkstra\n");
                return -3;
            }
        }
    }
    vector<city> citylist;
    map<string, int> name_dict;
    string readfile = "citylist.txt";
    // The `read_cityinfo` function is called to read the data from "citylist.txt" into `citylist`.
    read_cityinfo(readfile, citylist, name_dict);
    // If the "-write_info" flag is set, the `write_cityinfo` function is called. 
    if (flags[1] == 1)
    {
        write_cityinfo(citylist);
    }
    // `dist` is a dtable object which stores the distances between every city in citylist.
    dtable dist(citylist);
    // If the "-write_dtable" flag is set, the `write_citydtable` function is called.
    if (flags[2] == 1)
    {
        write_citydtable(citylist, dist);
    }
    // A graph is created using `citylist` and `dist`. The adjacencies are stored in `graph`.
    edge graph((int)(citylist.size()));
    create_citygraph(citylist, dist, graph);
    // If the "-write_graph" flag is set, the `write_citygraph` function is called.
    if (flags[3] == 1)
    {
        write_citygraph(citylist, dist, graph);
    }
    /* `city1` and `city2` are strings that store the city names specified by the user.
     * `source` and `sink` are the indicies corresponding to `city1` and `city2`.
     * `vdist` and `vlink` are used to store the data describing the BFS or Dijkstra route.
     */
    string city1, city2;
    int source, sink;
    vector<float> vdist;
    vector<int> vlink;
    /* If the "-randomseed" flag is set, the seed for the random number generator is set to `time(NULL)`.
     * Otherwise, the seed is set to 0. Then, the random number generator (`RNG`) is initialized.
     */
    int seed = 0;
    if (flags[7] == 1)
    {
        seed = time(NULL);
    } 
    rnumgen RNG(seed);
    // Preps the random number generator.
    int num_zones = nzones(citylist);
    int tpop = total_population(citylist);
    prep_rnumgen(num_zones, tpop, citylist, RNG);
    // The remainder of the code only runs if the "-mode_bfs" or "-mode_dijkstra" flags are set.
    if (flags[4] == 1 || flags[5] == 1)
    {
        /* While stdin is open, two strings will be read and stored in `city1` and `city2`. If either
         * string is "*", the `source`/`sink` variable will be set using rnumgen::rand. Otherwise, the
         * indicies are set with the `name_dict` map.
         */ 
        printf("Enter> ");
        while (cin >> city1 >> city2)
        {
            if (city1 == "*")
            {
                source = RNG.rand() % (int)(citylist.size());
            }
            else
            {
                if (!name_dict.count(city1))
                {
                    source = name_dict.upper_bound(city1)->second;
                }
                else
                {
                    source = name_dict.find(city1)->second;
                }
            }
            if (city2 == "*")
            {
                sink = RNG.rand() % (int)(citylist.size());
            }
            else
            {
                if (!name_dict.count(city2))
                {
                    sink = name_dict.upper_bound(city2)->second;
                }
                else
                {
                    sink = name_dict.find(city2)->second;
                }
            }
            /* If the "-mode_dijkstra" flag is set, the `dijkstra_route` function is called.
             * If the "-mode_bfs" flag is set, the `bfs_route` function is called.
             * If both are set, `bfs_route` takes precedence.
             */
            if (flags[5] == 1)
            {
                dijkstra_route(source, sink, vdist, vlink, citylist, graph, dist);
            }
            if (flags[4] == 1)
            {
                bfs_route(source, sink, vdist, vlink, citylist, dist, graph);
            }
            // If the "-show" flag is set, the `show_route` function is called.
            if (flags[6] == 1)
            { 
                show_route(source, sink, vdist, vlink, citylist, dist);
            }
            else
            {
                printf("\n");
            }
            printf("Enter> ");
        } 
    }
    return 0;
}
