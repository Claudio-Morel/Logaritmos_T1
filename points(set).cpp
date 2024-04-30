#include <iostream>
#include <random>
#include <set>
using namespace std;

struct Point {
    double x;
    double y;

    bool operator<(const Point& other) const {
        if (x < other.x) return true;
        if (x > other.x) return false;
        return y < other.y;
    }
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

int main() {
    int n =25;
    set<Point> points = generatePoints(pow(2,n));
    return 0;
}