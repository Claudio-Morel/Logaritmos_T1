#include <iostream>
#include "structures.h"
#include "points.cpp"
#include <algorithm>
using namespace std;
#include<tuple>

#define B 4096

tuple<int,double> checkDistance(Point point, vector<Point> samples){
    double dist;
    double minDist = 1;
    int index;
    for (int i  = 0; i<samples.size(); i++){
        dist = pow(point.x - samples[i].x, 2) + pow(point.y - samples[i].y, 2);
        if(dist < minDist){
            minDist = dist;
            index = i;
        }
    }
    tuple<int,double> tupla (index,minDist);
    return tupla;
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
        cout << "CEIL: " << ceil((n/(double)B))<< endl;
        long long k = min((long long)B, (long long)ceil((n/(double)B)));
        vector <vector <Point>> clusters(k);
        vector<Point> samples;
        vector<double> distancia_max(k,0);
        while(true){
            uniform_int_distribution<> dis(0, n-1);
            for (int i = 0; i<k; i++){
                unsigned long long indice = dis(gen);
                samples.push_back(points[indice]);
                points.erase(points.begin() + indice);
                n--;
            }
            for (Point point : points){
                tuple<int,double> tup = checkDistance(point, samples);
                int index = get<0>(tup);
                clusters[index].push_back(point);
                if(distancia_max[index]< get<1>(tup)){
                    distancia_max[index] = get<1>(tup);
                }
            }
            vector<int> toErase;
            for (int i = 0; i<clusters.size(); i++){
                if (clusters[i].size() < B/2){
                    toErase.push_back(i);
                }
            }
            for (int i = 0; i<toErase.size(); i++){
                vector<Point> rearrangePoints = clusters[toErase[i]];
                points.push_back(samples[toErase[i]]);
                clusters.erase(clusters.begin() + toErase[i]);
                samples.erase(samples.begin() + toErase[i]);
                n++;
                for (Point point : rearrangePoints){
                    tuple<int,double> tup = checkDistance(point, samples);
                    int index = get<0>(tup);
                    clusters[index].push_back(point);
                    if(distancia_max[index]< get<1>(tup)){
                        distancia_max[index] = get<1>(tup);
                    }
                }
            }
            if (clusters.size() > 1){
                //Agregar denuevo a puntos los ptos del cluster
                for(Point point : clusters[0]){
                    points.push_back(point);
                }
                break;
            }
        }
        for(int i = 0; i< samples.size();i++){
            Entry entrada = Entry{samples[i],distancia_max[i]};
            Mtree hijo = Ciaccia_Patella(clusters[i]);
            entrada.hijos = &hijo;
            mtree.raiz.entradas.push_back(entrada);
        }
        return mtree;
    }
}

int main(){
    set<Point> points = generatePoints(pow(2, 14));
    vector<Point> v(points.begin(), points.end());
    Mtree mtree = Ciaccia_Patella(v);
    return 0;
}

