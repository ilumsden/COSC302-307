#include <cstdio>
#include <iostream>
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
    sin.str(tmp);
    sin >> place.zone >> place.name >> place.type >> place.latitude >> place.longitude;
    if (place.type != REG && place.type != GAT)
    {
        fprintf(stderr, "%s does not have a valid city type.\n", place.name.c_str());
        exit(-1);
    }
    return;
}

class dtable;

create_citygraph() { }

read_cityinfo() { }
write_cityinfo() { }
write_citydtable() { }
write_citygraph() { }

class rnumgen; <-- COSC307 only

shortest_route() { }

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
