#include <vector>
#include <algorithm>
#include "structures.h"
using namespace std;


vector<vector<Point>> ClusterGen(vector<Point>& Cin) {
    // Step 1
    vector<Cluster> Cout;
    vector<Cluster> C;

    // Step 2
    for (Point & p : Cin) {
        C.push_back(Cluster(p));
    }

    // Step 3
    vector<Cluster>::iterator it1 = C.begin();
    double minDist = 1;
    Cluster c1, c2;
    while (C.size() > 1) {
        // Step 3.1
        for (vector<Cluster>::iterator it = C.begin(); it != C.end(); ++it) {
            for (vector<Cluster>::iterator jt = next(it); jt != C.end(); ++jt) {
                double dist = (*it).medoide.distance((*jt).medoide);
                if (dist < minDist) {
                    minDist = dist;
                    c1 = *it;
                    c2 = *jt;
                }
                
            }
        }

        // Step 3.2
        if (c1.elementos + c2.elementos <= 4096) {
            it1->insert(it1->end(), it2->begin(), it2->end());
            C.erase(it2);
        } else {
            Cout.push_back(*it1);
            C.erase(it1);
        }
    }

    // Step 4
    vector<Point>& c = C.back();

    // Step 5
    if (!Cout.empty()) {
        // Step 5.1
        auto it = min_element(Cout.begin(), Cout.end(), [&](const vector<Point>& c1, const vector<Point>& c2) {
            return Distance(c, c1) < Distance(c, c2);
        });
        vector<Point>& cPrime = *it;
        Cout.erase(it);

        // Step 6
        if (c.size() + cPrime.size() <= B) {
            c.insert(c.end(), cPrime.begin(), cPrime.end());
            Cout.push_back(c);
        } else {
            vector<Point> c1, c2;
            // Perform MinMax split policy to divide c ∪ cPrime into c1 and c2
            // ...

            Cout.push_back(c1);
            Cout.push_back(c2);
        }
    } else {
        // Step 5.2
        vector<Point> cPrime;
        // ...

        if (c.size() + cPrime.size() <= B) {
            c.insert(c.end(), cPrime.begin(), cPrime.end());
            Cout.push_back(c);
        } else {
            vector<Point> c1, c2;
            // Perform MinMax split policy to divide c ∪ cPrime into c1 and c2
            // ...

            Cout.push_back(c1);
            Cout.push_back(c2);
        }
    }

    // Step 7
    return Cout;
}

