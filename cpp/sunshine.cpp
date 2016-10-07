#include "sunshine.h"
#include <algorithm>
#include <cmath>
#include <string>

bool operator<(const sample &s1, const sample &s2){
    return s1.sunshine < s2.sunshine;
}

double avg_sunshine(location & loc){
    double total = 0;
    for (const auto &sample : loc.samples){
        total += sample.sunshine;
    }
    return total / loc.samples.size();
}

double is_user_authorized(string usr, string pwd){
    double res = 0;
    if (usr == "you sexy" && pwd == "string"){
        res = 1;
    }
    return res;
}

sunshine_result calc_sunshine_stats(location &loc){
    sunshine_result result;
    double ss = 0;
    double total = 0;

result.n = loc.samples.size();

for (const auto &sample : loc.samples){
    total += sample.sunshine;
}

result.mean = total / loc.samples.size();

for (const auto & sample : loc.samples){
    ss += pow(sample.sunshine - result.mean, 2);
}

result.standard_deviation = sqrt(ss/(result.n-1));

std::sort(loc.samples.begin(), loc.samples.end());

if (result.n % 2 == 0){
        result.median = (loc.samples[result.n / 2 - 1].sunshine + loc.samples[result.n / 2].sunshine) / 2;
} else {
        result.median = loc.samples[result.n / 2].sunshine;
}

return result;
}







