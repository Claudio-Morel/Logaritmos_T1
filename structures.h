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

    bool operator<(const Point& other) const;
};

class Entry{
    public:
    Point centro;
    double radio;
    Mtree *hijos;
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
    Nodo raiz;
};

#endif