#include <vector>
#include <string>
using namespace std;

class sample {
    public:
    sample() {
        date = ""; sunshine = 0;
    }
    sample(string d, double s){
        date = d;
        sunshine = s;
    }
    string date;
    double sunshine;
};

//for median calculation (sorting)
bool operator<(const sample &s1, const sample &s2);

class location {
    public:
    double longitude;
    double latitude;
    vector<sample> samples;

};

class sunshine_result {
    public: 
        float median;
        float mean;
        float standard_deviation;
        int n;
};

//will return the arithmetic mean sunshine for the given location
double avg_sunshine(location & loc); //code in sunshine.cpp

sunshine_result calc_sunshine_stats(location &loc);

//may you be blessed with love, happiness and health friend 


