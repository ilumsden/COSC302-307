#include <cmath>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <iomanip>
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
        float operator()(int, int);
    private:
        vector<float> dist;
};

dtable::dtable(vector<city> &citylist)
{
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
            centangle = acos(sin(lat1)*sin(lat2) + cos(lat1)*cos(lat2)*cos(long1 - long2));
            distance = centangle * 3982;
            distance = 5.0*round(distance/5.0);
            dist.push_back(distance);
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
    return (&dist[(i*(i+1))/2])[(j*(j+1))/2];
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
    fout << "DISTANCE TABLE:\n\n";
    for (int i = 1; i < (int)(citylist.size()); i++)
    {
        for (int j = i - 1; j >= 0; j--)
        {
            name = citylist[i].get_name() + " to " + citylist[j].get_name();
            fout << " " << right << i << " ";
            fout << setfill('.') << setw(2*width+4) << left << name;
            fout << " " << setw(mile_length) << setfill(' ') << right << dist(i, j) << " miles\n";
        }
        fout << "\n";
    }
    fout.close();
    return;
}

//write_citydtable() { }
//write_citygraph() { }

//class rnumgen; <-- COSC307 only

//shortest_route() { }

int main(int argc, char *argv[])
{
    bool flags[argc];
    if (argc == 1)
    {
        flags[0] = true;
    }
    else
    {
        string f;
        for (int i = 1; i < argc; i++)
        {
            f = argv[i];
            if (f == "-write_info")
            {
                flags[1] = true;
            }
            else if (f == "-write_dtable")
            {
                flags[2] = true;
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
    if (flags[1] == true)
    {
        fprintf(stdout, "Write City Info\n");
        write_cityinfo(citylist);
    }
    dtable dist(citylist);
    if (flags[2] == true)
    {
        fprintf(stdout, "Write City Dtable\n");
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
