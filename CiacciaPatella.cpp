#include "structures.h"
#include "points.cpp"
#include <algorithm>
using namespace std;

#define B 4096

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
        long long k = min((long long)B, (long long)(n/B));
        vector<Point> sample;
        uniform_int_distribution<> dis(0, n-1);
        for (int i = 0; i<k; i++){
            unsigned long long indice = dis(gen)%n;
            sample.push_back(points[indice]);
            points.erase(points.begin() + indice);
            n--;
        }
        return mtree;
    }
}

int main(){
    set<Point> points = generatePoints(100);
    vector<Point> v(points.begin(), points.end());
    Mtree mtree = Ciaccia_Patella(v);
    return 0;
}

