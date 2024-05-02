#include <iostream>
#include "structures.h"
#include "points.cpp"
#include <algorithm>
using namespace std;
#include<tuple>
#include <chrono>

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
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    mt19937 gen(seed);
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
        //Calculamos k
        long long k = min((long long)B, (long long)ceil((n/(double)B)));
        vector <vector <Point>> clusters(k); 
        vector<Point> samples;
        vector<double> distancia_max(k,0);
        while(true){
            uniform_int_distribution<> dis(0, n-1);
            // Elegimos k puntos random y los agregamos a samples.
            for (int i = 0; i<k; i++){
                unsigned long long indice = dis(gen);
                samples.push_back(points[indice]);
                points.erase(points.begin() + indice);
                n--;
            }
            // Asociamos todos los puntos a su sample más cercano.
            for (Point point : points){
                tuple<int,double> tup = checkDistance(point, samples);
                int index = get<0>(tup);
                clusters[index].push_back(point);
                if(distancia_max[index]< get<1>(tup)){
                    distancia_max[index] = get<1>(tup);
                }
            }
            //Inicializamos vector para guardar los elementos a borrar.
            vector<int> toErase;
            //Revisamos si los debemos borrar
            for (int i = 0; i<clusters.size(); i++){
                if (clusters[i].size() < B/2){
                    toErase.push_back(i);
                }
            }
            //Los borramos y reasignamos los puntos.
            for (int i = toErase.size()-1; i>=0; i--){                
                vector<Point> rearrangePoints = clusters[toErase[i]];
                rearrangePoints.push_back(samples[toErase[i]]);
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
            //Revisamos si es que hay solo un cluster, si es así, repetimos el while
            // si no, se deja de iterar.
            if (clusters.size() > 1){
                break;
            }else {
                for(Point point: samples){
                    points.push_back(point);
                }
                samples.clear();
                toErase.clear();
                clusters.resize(k);
                for(int i =0; i<k; i++){
                    clusters[i].clear();
                }
            }
        }
        // (Paso 6)
        // Vamos calculando recursivamente, asignando los arboles.
        unsigned long long n_total = 0;
        cout << "k es :" << k <<" samples es :" <<samples.size()<< endl;
        for(int i = 0; i< samples.size();i++){
            n_total += (1+ clusters[i].size());
            cout << "RECURSIVO CTM " <<clusters[i].size() << endl;
            Entry entrada = Entry{samples[i],distancia_max[i]};
            Mtree hijo = Ciaccia_Patella(clusters[i]);
            entrada.hijos = &hijo;
            mtree.raiz.entradas.push_back(entrada);
        }
        cout << n_total<< endl;
        return mtree;
    }
}

int main(){
    set<Point> points = generatePoints(pow(2, 18));
    vector<Point> v(points.begin(), points.end());
    Mtree mtree = Ciaccia_Patella(v);
    return 0;
}

// Al agregar puntos al cluster, no los borramos.
