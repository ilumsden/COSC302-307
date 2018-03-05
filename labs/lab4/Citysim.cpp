#include <cfloat>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <map>
#include <fstream>
#include <sstream>
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
    /*if (place.type != REG && place.type != GAT)
    {
        fprintf(stderr, "%s does not have a valid city type.\n", place.name.c_str());
        exit(-1);
    }*/
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

//create_citygraph() { }

void read_cityinfo(string fname, vector<city> &citylist)
{
    if (!citylist.empty())
    {
        citylist.clear();
    }
    fstream fin(fname.c_str(), ios::in);
    if (!fin.is_open())
    {
        fprintf(stderr, "Unable to open %s\n", fname.c_str());
        fin.close();
        exit(-2);
    }
    city tmp;
    while (fin >> tmp)
    {
        if (fin.eof())
        {
            break;
        }
        citylist.push_back(tmp);
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
        exit(-3);
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
        exit(-5);
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

void get_regional_cities(vector<int> &ind, vector<city> &citylist)
{
    for (int i = 0; i < (int)(citylist.size()); i++)
    {
        if (citylist[i].get_type() == REG)
        {
            ind.append(i);
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
            ind.append(i);
        }
    }
    return;
}

void make_dists_vector(vector<city> &citylist, vector< pair<int, float> > &regiondists)
{
    int reg = 0;
    for (int i = 0; i < (int)(citylist.size()); i++)
    {
        if (citylist[i].get_zone() > reg)
        {
            reg = citylist[i].get_zone();
        }
    }
    if (!regiondists.empty())
    {
        regiondists.clear();
    }
    for (int j = 0; j < reg; j++)
    {
        regiondists.push_back(make_pair(0, FLT_MAX));
    }
    return;
}

void create_citygraph(vector<city> &citylist, dtable &dist, map< int, vector<int> > &graph )
{
    vector<int> reg;
    vector<int> gate;
    get_regional_cities(reg, citylist);
    get_gateway_cities(gate, citylist);
    int base;
    vector<int> tmpdata;
    for (int i = 0; i < (int)(reg.size()); i++)
    {
        base = reg[i];
        for (int j = 0; j < (int)(reg.size()); i++)
        {
            if (i == j)
            {
                continue;
            }
            if (citylist[base].get_zone() == citylist[reg[j]].get_zone())
            {
                tmpdata.push_back(reg[j]);
            }
        }
        for (int k = 0; k < (int)(gate.size()); k++)
        {
            int curr_gate = gate[k];
            int min_gate = 0;
            float min_dist = FLT_MAX;
            if (citylist[base].get_zone() == citylist[curr_gate].get_zone())
            {
                if (dist(base, curr_gate) < min_dist)
                {
                    min_dist = dist(base, curr_gate);
                    min_gate = curr_gate;
                }
            }
        }
        tmpdata.push_back(min_gate);
        graph.insert(make_pair(base, tmpdata));
        tmpdata.clear();
    }
    vector< pair<int, float> > regiondists;
    make_dists_vector(citylist, regiondists);
    for (int i = 0; i < (int)(gate.size()); i++)
    {
        base = gate[i];
        for (int j = 0; j < (int)(gate.size()); i++)
        {
            int curr = gate[j];
            if (i == j)
            {
                continue;
            }
            if (citylist[base].get_zone() == citylist[curr].get_zone())
            {
                tmpdata.push_back(curr);
            }
            else
            {
                if (dist(base, curr) <  
            }
        }
    }
}

//write_citygraph() { }

//class rnumgen; <-- COSC307 only

//shortest_route() { }

int main(int argc, char *argv[])
{
    int flags[3];
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
            else
            {
                fprintf(stderr, "Usage: ./Citysim -write_info|write_dtable\n");
                return -4;
            }
        }
    }
    vector<city> citylist;
    string readfile = "citylist.txt";

    read_cityinfo(readfile, citylist);
    if (flags[1] == 1)
    {
        write_cityinfo(citylist);
    }
    dtable dist(citylist);
    if (flags[2] == 1)
    {
        write_citydtable(citylist, dist);
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
