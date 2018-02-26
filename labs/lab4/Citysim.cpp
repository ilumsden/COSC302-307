#define _USE_MATH_DEFINES

#include <cmath>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

using namespace std;

const string REG = "REGIONAL";
const string GAT = "GATEWAY";

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
    sin >> place.zone >> place.name >> place.type >> lati >> longi;
    if (place.type != REG && place.type != GAT)
    {
        fprintf(stderr, "%s does not have a valid city type.\n", place.name.c_str());
        exit(-1);
    }
    lati = lait*(M_PI / 180);
    longi = longi*(M_PI / 180);
    place.latitude = lati;
    place.longitude = longi;
    return fin;
}

ostream & operator<<(ostream &fout, city &place)
{
    float lati = place.latitude * (180/M_PI);
    float longi = place.longitude * (180/M_PI);
    fout << setw(20) << place.name << " "
         << setw(12) << place.type << " "
         << setw(2) << right << place.zone << " "
         << setw(10) << right << place.pop << " "
         << setw(8) << setprecision(2) << right << lati << " "
         << setw(8) << setprecision(2) << right << longi << "\n";
    return fout;
}

//class dtable;

//create_citygraph() { }

void read_cityinfo(string fname, vector<city> &citylist)
{
    if (!citylist.empty())
    {
        citylist.clear();
    }
    fstream fin(fname, ios::in);
    if (!fin.is_open())
    {
        fprintf(stderr, "Unable to open %s\n", fname.c_str());
        fin.close();
        exit(-2);
    }
}

//write_cityinfo() { }
//write_citydtable() { }
//write_citygraph() { }

//class rnumgen; <-- COSC307 only

//shortest_route() { }

int main(int argc, char *argv[])
{
  option decoding

  object declarations

  read_cityinfo()
  if (option_write_cityinfo) 
    write_cityinfo()

  create_citygraph()
  if (option_write_citydtable) 
    write_citydtable()

  if (option_write_citygraph) 
    write_citygraph()

  while (not done)
    shortest_route(from,to) 
}
