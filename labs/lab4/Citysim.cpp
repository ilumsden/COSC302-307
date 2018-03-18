#include <algorithm>
#include <cfloat>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <cstdlib>
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

void write_citydtable(const vector<city> &citylist, const dtable &dist)
{
    string fname = "citydtable.txt";
    fstream fout(fname.c_str(), ios::out);
    if (!fout.is_open())
    {
        fprintf(stderr, "Unable to open/create %s\n", fname.c_str());
        fout.close();
        exit(-4);
    }
    int width = longest_name(citylist);
    int mile_length = longest_distance(dist, (int)(citylist.size()));
    string name, distance;
    int linenum_width = floor(log10((int)(citylist.size()))) + 1;
    fout << "DISTANCE TABLE:\n\n";
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

class edge
{
    public:
        edge(int);
        ~edge() { delete [] adj; }
        int * operator[](int i) const { return &adj[(i*(i+1))/2]; }
        void set_edge(int, int);
        int get_edge(int, int) const;
    private:
        int *adj;
};

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

void get_gateways_by_zone(vector< vector<int> > &zones, const vector<city> &citylist, const vector<int> &gate)
{
    vector<int> zonex;
    int numzones = 0;
    if (!zones.empty())
    {
        zones.clear();
    }
    for (int i = 0; i < (int)(citylist.size()); i++)
    {
        if (citylist[i].get_zone() > numzones)
        {
            numzones = citylist[i].get_zone();
        }
    }
    int curr;
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

void create_citygraph(vector<city> &citylist, dtable &dist, edge &graph)
{
    vector<int> reg;
    vector<int> gate;
    get_regional_cities(reg, citylist);
    get_gateway_cities(gate, citylist);
    int base, curr;
    float min_dist;
    int min_ind;
    for (int i = 0; i < (int)(reg.size()); i++)
    {
        base = reg[i];
        for (int j = i+1; j < (int)(reg.size()); j++)
        {
            curr = reg[j];
            if (citylist[base].get_zone() == citylist[curr].get_zone())
            {
                graph.set_edge(base, curr);
            }
        }
        min_ind = -1;
        min_dist = FLT_MAX;
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
    vector< vector<int> > zones;
    vector<int> zonex;
    get_gateways_by_zone(zones, citylist, gate);
    for (int i = 0; i < (int)(gate.size()); i++)
    {
        base = gate[i];
        for (int j = i+1; j < (int)(gate.size()); j++)
        {
            curr = gate[j];
            if (citylist[base].get_zone() == citylist[curr].get_zone())
            {
                graph.set_edge(base, curr);
            }
        }
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

void write_citygraph(const vector<city> &citylist, const dtable &dist, const edge &graph)
{
    int linenum_width = floor(log10((int)(citylist.size()))) + 1;
    string fname = "citygraph.txt";
    fstream fout(fname.c_str(), ios::out);
    fout << "CITY GRAPH:\n\n";
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

void bfs_route(int source, int sink, vector<float> &vdist, vector<int> &vlink, const vector<city> &citylist, const dtable &dist, const edge &graph)
{
    if (!vdist.empty())
    {
        vdist.clear();
    }
    if (!vlink.empty())
    {
        vlink.clear();
    }
    vdist.assign(citylist.size(), FLT_MAX);
    vlink.assign(citylist.size(), -1);
    vdist[source] = 0;
    vlink[source] = source;
    queue<int> vert;
    vert.push(source);
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
    while (!vert.empty())
    {
        vert.pop();
    }
}

typedef enum { WHITE, BLACK } vcolor_t;

void dijkstra_route(int source, int sink, vector<float> &vdist, vector<int> &vlink, const vector<city> &citylist, const edge &graph, const dtable &dist)
{
    vector<vcolor_t> vcolor;
    vcolor.assign(citylist.size(), WHITE);
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
        vcolor[i] = BLACK;
        if (i == sink)
        {
            break;
        }
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

void show_route(int source, int sink, vector<float> &vdist, vector<int> &vlink, const vector<city> &citylist, const dtable &dist)
{
    int linenum_width = floor(log10((int)(citylist.size()))) + 1;
    if (vdist[sink] == FLT_MAX)
    {
        fprintf(stdout, "No path from %s to %s\n", citylist[source].get_name().c_str(), citylist[sink].get_name().c_str());
        return;
    }
    int totaldist_width = floor(log10((int)(vdist[sink]))) + 1;
    int name_width = longest_name(citylist) + 3;
    int seperationdist_width = longest_distance(dist, (int)(citylist.size())); 
    stack<int> S;
    stack<float> D;
    for (int i = sink; i != source; i = vlink[i])
    {
        S.push(i);
        D.push(vdist[i]);
    }
    S.push(source);
    D.push(vdist[source]);
    int city1;
    float fulldist, prevdist;
    while (!S.empty())
    {
        city1 = S.top();
        S.pop();
        if (city1 != source)
        {
            prevdist = fulldist; 
        }
        fulldist = D.top();
        D.pop();
        cout << "   " << setw(totaldist_width) << right << fulldist << " miles : "
             << setw(linenum_width) << right << city1 << " "
             << setw(name_width) << left << citylist[city1].get_name();
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

int total_population(const vector<city> &citylist)
{
    int tpop = 0;
    for (int i = 0; i < (int)(citylist.size()); i++)
    {
        tpop += citylist[i].get_population();
    }
    return tpop;
}

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

class rnumgen
{
    public:
        rnumgen(int seed);
        void pdf(const vector<float> &);
        int rand() const;
    private:
        vector<float> F;
};

rnumgen::rnumgen(int seed)
{
    char *byte = (char *)&seed;
    swap(byte[0], byte[3]);
    swap(byte[1], byte[2]);
    int seedval = *((int *)byte);
    srand(seedval);
}

void rnumgen::pdf(const vector<float> &v)
{
    F.resize(v.size());
    partial_sum(v.begin(), v.end(), F.begin());
    transform(F.begin(), F.end(), F.begin(), bind2nd(divides<float>(), *(F.end()-1)));
}

int rnumgen::rand() const
{
    const float randnorm = RAND_MAX + 1.0f;
    const float p = (float)std::rand()/randnorm;
    return upper_bound(F.begin(), F.end(), p) - F.begin();
}

void prep_rnumgen(const int numzones, const int tpop, const vector<city> &citylist, rnumgen &RNG)
{
    vector<float> problist;
    int pop, zone_cities;
    float prob;
    for (int i = 1; i < numzones + 1; i++)
    {
        pop = zone_population(i, citylist);
        zone_cities = num_zone_cities(i, citylist);
        prob = ((float)(pop)/tpop)*(1.0f/zone_cities);
        problist.push_back(prob);
    }
    RNG.pdf(problist);
}

int rand_city(const vector<city> &citylist, rnumgen &random)
{
    int zone = random.rand();
    int city;
    do
    {
        city = std::rand() % (int)(citylist.size());
    } while (citylist[city].get_zone() != zone);
    fprintf(stdout, "City = %s\n", citylist[city].get_name().c_str());
    return city;
}

int main(int argc, char *argv[])
{
    int flags[8];
    if (argc == 1)
    {
        flags[0] = 1;
    }
    else
    {
        string f;
        string city1, city2;
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

    read_cityinfo(readfile, citylist, name_dict);
    if (flags[1] == 1)
    {
        write_cityinfo(citylist);
    }
    dtable dist(citylist);
    if (flags[2] == 1)
    {
        write_citydtable(citylist, dist);
    }
    edge graph((int)(citylist.size()));
    create_citygraph(citylist, dist, graph);
    if (flags[3] == 1)
    {
        write_citygraph(citylist, dist, graph);
    }
    string city1, city2;
    int source, sink;
    vector<float> vdist;
    vector<int> vlink;
    int seed = 0;
    if (flags[7] == 1)
    {
        seed = time(NULL);
    } 
    rnumgen RNG(seed);
    int num_zones = nzones(citylist);
    int tpop = total_population(citylist);
    prep_rnumgen(num_zones, tpop, citylist, RNG);
    if (flags[4] == 1 || flags[5] == 1)
    {
        printf("Enter> ");
        while (cin >> city1 >> city2)
        {
            if (city1 == "*")
            {
                source = rand_city(citylist, RNG);
            }
            else
            {
                source = name_dict.upper_bound(city1)->second;
            }
            if (city2 == "*")
            {
                sink = rand_city(citylist, RNG);
            }
            else
            {
                sink = name_dict.upper_bound(city2)->second;
            }
            if (flags[5] == 1)
            {
                dijkstra_route(source, sink, vdist, vlink, citylist, graph, dist);
            }
            if (flags[4] == 1)
            {
                bfs_route(source, sink, vdist, vlink, citylist, dist, graph);
            }
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
