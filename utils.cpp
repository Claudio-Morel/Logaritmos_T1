//minMaxSplitPolicy
#include "structures.cpp"
#include "structures.h"
#include <cmath>
#include <iostream>

vector<Cluster> minMaxSplit(Cluster &Cunion){
    double minRadio = 2;
    vector<Cluster> clusters;
    Cluster c1, c2, c3, c4;
    for (vector<Point>::iterator it = Cunion.puntos.begin(); it != Cunion.puntos.end(); it++){
        for (vector<Point>::iterator it2 = next(it); it2 != Cunion.puntos.end(); it2++){
            vector <Point> puntos = Cunion.puntos;
            c3 = Cluster((*it));
            c4 = Cluster((*it2));
            double distC3, distC4;
            for (int i = 0; i<Cunion.count; i++){
                if (i%2 == 0){
                    double minDist = 2;
                    vector <Point>::iterator itToAdd;;
                    for (vector<Point>::iterator it3 = puntos.begin(); it3 != puntos.end(); ++it3){
                        distC3 = (*it).distance((*it3));
                        if (distC3 < minDist){
                            itToAdd = it3;
                            minDist = distC3;
                        }
                    }
                    c3.radio = distC3;
                    Point p = (*itToAdd);
                    c3.insertar(p);
                    puntos.erase(itToAdd);
                }else{
                    double minDist = 2;
                    vector <Point>::iterator itToAdd;
                    for (vector<Point>::iterator it3 = puntos.begin(); it3 != puntos.end(); ++it3){
                        distC4 = (*it2).distance((*it3));
                        if (distC4 < minDist){
                            itToAdd = it3;
                            minDist = distC4;
                        }
                    }
                    c4.radio = distC4;
                    Point p = (*itToAdd);
                    c4.insertar(p);
                    puntos.erase(itToAdd);
                }
            }
            double maxRadio = max(c3.radio, c4.radio);
            if (maxRadio < minRadio){
                minRadio = maxRadio;
                c1 = c3;
                c2 = c4;
            }
        }
    }
    clusters.push_back(c1);
    clusters.push_back(c2);
    return clusters;
}

/* int main(){
    Point p = Point{0.5,0.5};
    Cluster c = Cluster(p);
    c.insertar(Point{0.1,0.1});
    c.insertar(Point{0.2,0.2});
    c.insertar(Point{0.3,0.3});
    c.insertar(Point{0.4,0.4});
    c.insertar(Point{0.5,0.5}); 
    c.insertar(Point{0.6,0.6});
    c.insertar(Point{0.7,0.7});
    c.insertar(Point{0.8,0.8});
    c.insertar(Point{0.9,0.9});
    c.insertar(Point{1,1});
    c.actualizarMedoide();
    cout << "Puntos cluster inicial:" << endl;
    for (Point p : c.puntos){
        cout << p.x << " " << p.y << endl;
    }
    vector <Cluster> clusters = minMaxSplit(c);
    cout << "Clusters resultantes:" << endl;
    for (Cluster c1 : clusters){
        cout << "medioide: " << c1.medoide.x << " " << c1.medoide.y << endl;
        for (Point p : c1.puntos){
            cout << p.x << " " << p.y << endl;
        }
    }


    return 0;
} */

tuple<unsigned long long, unsigned long long> twoClosestPoints(vector<Cluster>& clusters){
    double minDist = 2;
    tuple <unsigned long long, unsigned long long> indices;
    unsigned long long a,b;
    for (unsigned long long i = 0; i<clusters.size(); i++){
        for (unsigned long long j = i+1; j<clusters.size(); j++){
            double dist = (clusters[i].medoide).distance(clusters[j].medoide);
            if (dist < minDist){
                minDist = dist;
                a = i;
                b = j;
            }
        }
    }
    indices = make_tuple(a,b);
    return indices;
}

