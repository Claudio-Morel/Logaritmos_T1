#include <vector>
using namespace std;

struct Point {
    double x;
    double y;

    bool operator<(const Point& other) const {
        if (x < other.x) return true;
        if (x > other.x) return false;
        return y < other.y;
    }
};

class Nodo{
    public:
    int count;
    vector<entry> entradas;

    void insertar(entry e){
        entradas.push_back(e);
        count++;
    }
};

class entry{
    public:
    Point centro;
    double radio;
    Nodo *hijos;
};

class Mtree {
    public:
    unsigned long long n;
    Nodo raiz;
};