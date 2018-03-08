#include <cfloat>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <map>
#include <queue>
#include <sstream>
#include <utility>
#include <vector>

using namespace std;

const string REG = "REGIONAL";
const string GAT = "GATEWAY";
const float PI = 3.1415926535897932384626433;

class city
{
    public:
        friend istream & operator>>(istream &, city &);
        friend ostream & operator<<(ostream &, city &);
        string get_name() const { return name; }
        string get_type() const { return type; }
        int get_zone() const { return zone; }
        int get_population() const { return pop; }
        float get_latitude() const { return latitude; }
        float get_longitude() const { return longitude; }
    private:
        string name, type;
        int zone, pop;
        float latitude, longitude;
};

istream & operator>>(istream &fin, city &place)
{
    stringstream sin;
    string tmp;
    while (getline(fin, tmp))
    {
        if (tmp[0] != '#' && tmp != "")
        {
            break;
        }
    }
    if (fin.eof())
    {
        return fin;
    }
    sin.str(tmp);
    float lati, longi;
    sin >> place.zone >> place.name >> place.type >> lati >> longi >> place.pop;
    lati = lati*(PI / 180);
    longi = longi*(PI / 180);
    place.latitude = lati;
    place.longitude = longi;
    return fin;
}

ostream & operator<<(ostream &fout, city &place)
{
    float lati = place.latitude * (180/PI);
    float longi = place.longitude * (180/PI);
    fout << setw(20) << left << place.name << " "
         << setw(12) << left << place.type << " "
         << setw(2) << right << place.zone << " "
         << setw(10) << right << place.pop << " "
         << setw(8) << setprecision(2) << fixed << right << lati << " "
         << setw(8) << setprecision(2) << fixed << right << longi << "\n";
    return fout;
}

class dtable
{
    public:
        dtable(vector<city> &);
        ~dtable() { delete [] dist; }
        float * operator[](int i) { return &dist[(i*(i+1))/2]; }
        float operator()(int, int);
    private:
        float *dist;
};

dtable::dtable(vector<city> &citylist)
{
    dist = new float [(citylist.size()*(citylist.size()+1))/2];
    float lat1, lat2, long1, long2;
    float centangle, distance;
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

float dtable::operator()(int i, int j)
{
    if (i < j)
    {
        int tmp = j;
        j = i;
        i = tmp;
    }
    return (*this)[i][j]; 
}

void read_cityinfo(string fname, vector<city> &citylist, map<string, int> name_dict)
{
    if (!citylist.empty())
    {
        citylist.clear();
    }
    if (!name_dict.empty())
    {
        name_dict.clear();
    }
    fstream fin(fname.c_str(), ios::in);
    if (!fin.is_open())
    {
        fprintf(stderr, "Unable to open %s\n", fname.c_str());
        fin.close();
        exit(-1);
    }
    city tmp;
    int i = 0;
    while (fin >> tmp)
    {
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

void write_cityinfo(vector<city> &citylist)
{
    string fname = "cityinfo.txt";
    if (citylist.empty())
    {
        return;
    }
    fstream fout(fname.c_str(), ios::out);
    if (!fout.is_open())
    {
        fprintf(stderr, "Unable to open/create %s\n", fname.c_str());
        fout.close();
        exit(-2);
    }
    fout << "CITY INFO (N=" << (int)(citylist.size()) << "):\n\n";
    int linenum_width = floor(log10((int)(citylist.size()))) + 1;
    for (int i = 0; i < (int)(citylist.size()); i++)
    {
        fout << " " << setw(linenum_width) << right << i << " ";
        fout << citylist[i];
    }
    fout.close();
    return;
}

int longest_name(vector<city> &citylist)
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

int longest_distance(dtable &dist, int size)
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

void write_citydtable(vector<city> &citylist, dtable &dist)
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
        int * operator[](int i) { return &adj[(i*(i+1))/2]; }
        void set_edge(int, int);
        int get_edge(int, int);
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

int edge::get_edge(int i, int j)
{
    if (i < j)
    {
        int tmp = j;
        j = i;
        i = tmp;
    }
    return (*this)[i][j]; 
}

void get_regional_cities(vector<int> &ind, vector<city> &citylist)
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

void get_gateway_cities(vector<int> &ind, vector<city> &citylist)
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

void get_gateways_by_zone(vector< vector<int> > &zones, vector<city> &citylist, vector<int> &gate)
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

void write_citygraph(vector<city> &citylist, dtable &dist, edge &graph)
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

void bfs_route(int source, int sink, vector<float> &vdist, vector<int> &vlink, vector<city> &citylist, edge &graph)
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
                    vdist[k] = vdist[ind] + 1;
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

void dijkstra_route(int source, int sink, vector<float> &vdist, vector<int> &vlink, vector<city> &citylist, edge &graph, dtable &dist)
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
                    vdist[k] = vdist[i] + weight;
                    vlink[k] = i;
                }
            }
        }
    }
}

//write_citygraph() { }

//class rnumgen; <-- COSC307 only

//shortest_route() { }

int main(int argc, char *argv[])
{
    int flags[5];
    if (argc == 1)
    {
        flags[0] = 1;
    }
    else
    {
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
            else if (f == "-mode_bfs" || f == "-mode_dijkstra")
            {
                flags[4] = 1;
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
    if (flags[4] == 1)
    {
        fprintf(stderr, "The bfs/dijkstra modes are not yet implemented. Aborting\n");
        return -5; 
    }

    /*read_cityinfo()
    if (option_write_cityinfo) 
      write_cityinfo()

    create_citygraph()
    if (option_write_citydtable) 
      write_citydtable()

    if (option_write_citygraph) 
      write_citygraph()

    while (not done)
      shortest_route(from,to)*/
    return 0;
}
