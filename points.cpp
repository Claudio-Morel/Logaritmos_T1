#include <random>
#include <set>
#include "structures.h"
using namespace std;
#include "structures.h"

bool Point::operator<(const Point& other) const{
        if (x < other.x) return true;
        if (x > other.x) return false;
        return y < other.y;
};

set<Point> generatePoints(unsigned long long n) {
    set <Point> points;
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dis(0.0, 1.0);

    while (points.size() < n) {
        Point p;
        p.x = dis(gen);
        p.y = dis(gen);
        points.insert(p);
    }

    return points;
}