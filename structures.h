#include <vector>
using namespace std;

#ifndef STRUCTURES_H
#define STRUCTURES_H

class Nodo; 
class Mtree;

class Point{
    public:
    double x;
    double y;
    double norma = -1;

    bool operator<(const Point& other) const;

    double distance(Point& other);
};

class Entry{
    public:
    Point centro;
    double radio;
    Nodo *hijos;
};

class Nodo{
    public:
    int count;
    vector<Entry> entradas;

    void insertar(Entry e){
        entradas.push_back(e);
        count++;
    }
};


class Mtree {
    public:
    unsigned long long n;
    Nodo *raiz;
    int altura = 1;
};

class Cluster {
    public:
    Point medoide;
    vector<Point> puntos;
    double radio;
    unsigned long long count = 0;

    Cluster(Point p);

    Cluster();

    void insertar(Point p);

    void actualizarMedoide();
};

class Query{
    public:
    Point punto;
    double radio;
    vector <Point> resultado;
};

#endif