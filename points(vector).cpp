#include <iostream>
#include <random>
#include <vector>
using namespace std;

struct Point {
    double x;
    double y;
};

vector<Point> generatePoints(unsigned long long n) {
    vector<Point> points;
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dis(0.0, 1.0);

    for (int i = 0; i < n; i++) {
        Point p;
        p.x = dis(gen);
        p.y = dis(gen);
        points.push_back(p);
    }

    return points;
}

int main() {
    int n = 10; // Cambia este valor para generar más o menos puntos
    vector<Point> points = generatePoints(n);

    for (const auto& point : points) {
        cout << "(" << point.x << ", " << point.y << ")" << endl;
    }

    return 0;
}