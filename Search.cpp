#include "structures.h"
#include "points.cpp"
#include <iostream>
#include <set>
#include <queue>
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
    while (!q.empty()){
        Nodo *n = q.front();
        q.pop();
        for (Entry e : n->entradas){
            if (e.hijos != nullptr ){
                if ((*pQuery).punto.distance(e.centro) < 2){
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
}