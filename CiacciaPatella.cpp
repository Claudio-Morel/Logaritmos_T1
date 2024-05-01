#include <iostream>
#include "structures.h"
#include "points.cpp"
#include <algorithm>
using namespace std;

#define B 4096

int checkDistance(Point point, vector<Point> sample){
    double dist;
    double minDist = 1;
    int index;
    for (int i  = 0; i<sample.size(); i++){
        dist = pow(point.x - sample[i].x, 2) + pow(point.y - sample[i].y, 2);
        if(dist < minDist){
            minDist = dist;
            index = i;
        }
    }
    return index;
}


Mtree Ciaccia_Patella(vector<Point> points){
    unsigned long long n = points.size();
    random_device rd;
    mt19937 gen(rd());
    Mtree mtree;
    if(n <= B){
        Entry entry;
        Nodo nodo;
        nodo.entradas;
        for (int i = 0; i < n; i++){
            entry.centro = points[i];
            nodo.insertar(entry);
        }
        mtree.raiz = nodo;
        return mtree;
    }
    else{
        while(true){
            long long k = min((long long)B, (long long)(n/B));
            cout << "k: " << k << endl;
            vector<Point> sample;
            vector <vector <Point>> clusters;
            uniform_int_distribution<> dis(0, n-1);
            for (int i = 0; i<k; i++){
                unsigned long long indice = dis(gen);
                sample.push_back(points[indice]);
                points.erase(points.begin() + indice);
                n--;
            }
            clusters.resize(k);
            cout << sample.size() << endl;
            cout << clusters.size() << endl;
            for (Point point : points){
                int index = checkDistance(point, sample);
                clusters[index].push_back(point);
            }
            cout << clusters.size() << endl;
            vector<int> toErase;
            for (int i = 0; i<clusters.size(); i++){
                if (clusters[i].size() < B/2){
                    toErase.push_back(i);
                }
            }
            for (int i = 0; i<toErase.size(); i++){
                vector<Point> rearrangePoints = clusters[toErase[i]];
                points.push_back(sample[toErase[i]]);
                clusters.erase(clusters.begin() + toErase[i]);
                sample.erase(sample.begin() + toErase[i]);
                n++;
                for (Point point : rearrangePoints){
                    int index = checkDistance(point, sample);
                    clusters[index].push_back(point);
                }
            }
            if (clusters.size() > 1){
                break;
            }
        }
        return mtree;
    }
}

int main(){
    set<Point> points = generatePoints(pow(2, 25));
    vector<Point> v(points.begin(), points.end());
    Mtree mtree = Ciaccia_Patella(v);
    return 0;
}

