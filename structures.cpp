#include "structures.h"
#include <cmath>

double Point::distance(Point& other) {
    double dx = x - other.x;
    double dy = y - other.y;
    return dx*dx + dy*dy;
}

void Cluster::insertar(Point p) {
    puntos.push_back(p);
    count++;
}

Cluster::Cluster(Point p) {
    medoide = p;
    puntos.push_back(p);
    count = 1;
    radio = 0;
}