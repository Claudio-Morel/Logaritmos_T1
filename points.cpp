#include <iostream>
#include <random>
#include <set>
#include "structures.cpp"
using namespace std;



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