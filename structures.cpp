#include "structures.h"
#include <cmath>

double Point::distance(Point& other) {
    double dx = x - other.x;
    double dy = y - other.y;
    return sqrt(dx*dx + dy*dy);
}

void Cluster::insertar(Point p) {
    puntos.push_back(p);
    count++;
}

void Cluster::actualizarMedoide(){
    double dist;
    for (Point &punto : puntos) {
        double maxDist = -1;
        for (Point &punto2 : puntos) {
            dist = punto.distance(punto2);
            if (maxDist < dist){
                maxDist = dist;
            }
        }
        if ( maxDist < radio) {
            radio = maxDist;
            medoide = punto;
        }
    }
}

Cluster::Cluster() {;
    count = 0;
    radio = 0;
}

Cluster::Cluster(Point p) {
    Cluster();
    medoide = p;
}