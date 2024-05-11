#include "structures.h"
#include "points.cpp"
#include <iostream>
#include <set>
#include <queue>
#include <chrono>
using namespace std;


vector<Query> QueryGen(unsigned long long n){
    vector<Query> queries;
    set<Point> points = generatePoints(n);
    vector <Point> puntos = vector<Point>(points.begin(), points.end());
    for (Point p : puntos){
        Query q;
        q.punto = p;
        q.radio = 0.02;
        queries.push_back(q);
    }
    return queries;
}

void Search(Query *pQuery, Mtree mtree){
    Nodo *raiz = mtree.raiz;
    queue<Nodo*> q;
    q.push(raiz);
    auto start = chrono::high_resolution_clock::now();
    while (!q.empty()){
        Nodo *n = q.front();
        pQuery->ios++;
        q.pop();
        for (Entry e : n->entradas){
            if (e.hijos != nullptr ){
                if ((*pQuery).punto.distance(e.centro) < (*pQuery).radio + e.radio){
                    q.push(e.hijos);
                }
            }
            else {
                if ((*pQuery).punto.distance(e.centro) < (*pQuery).radio){
                    (*pQuery).resultado.push_back(e.centro);
                }
            }
        }
    }
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    pQuery->time = duration.count();
}