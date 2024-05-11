#include "structures.h"
#include <cmath>
using namespace std;

double Point::distance(Point& other) {
    double dx = x - other.x;
    double dy = y - other.y;
    return sqrt(dx*dx + dy*dy);
}

void Cluster::insertar(Point p) {
    this->puntos.push_back(p);
    this->count++;
}

void Cluster::actualizarMedoide(){
    double maxDist = -1;
    radio = 3;
    for (Point p : puntos){
        maxDist = -1;
        for (Point p2 : puntos){
            double dist = p.distance(p2);
            if (maxDist < dist){
                maxDist = dist;
            }
        }
        if (maxDist < radio){
            radio = maxDist;
            medoide = p;
        }
    }
}

Cluster::Cluster() {;
    count = 0;
    radio = 3;
}

Cluster::Cluster(Point p) {
    Cluster();
    medoide = p;
}