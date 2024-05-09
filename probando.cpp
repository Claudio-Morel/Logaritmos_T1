#include <iostream>
#include <random>
#include <chrono>
#include <queue>
#include "structures.h"

#include <iostream>
#include "structures.h"
#include "points.cpp"
#include <algorithm>
using namespace std;
#include<tuple>
#include <chrono>
#include <queue>

void Link(Nodo *T, Nodo *Tsup) {
    Point punto = T->entradas[0].centro;
    double MinDist = 2;
    Entry* hoja_min = nullptr; // Inicializar a nullptr

    queue<Nodo *> q;
    q.push(Tsup);
    while (!q.empty()) {
        Nodo* nodo = q.front();
        q.pop();
        vector<Entry>& entradas = nodo->entradas; // Utilizar referencia para evitar copias
        for (Entry& entrada : entradas) { // Utilizar referencia para modificar objetos en el vector
            if (entrada.hijos != nullptr) {
                q.push(entrada.hijos);
            }
            else {
                double dist = punto.distance(entrada.centro);
                cout << "DIFERENCIA ES :" << dist << endl;
                if (dist < MinDist) {
                    MinDist = dist;
                    hoja_min = &entrada; // Asignar la referencia al nodo más cercano
                }
            }
        }
        cout << q.size() << endl;
    }
    
    // Asignar T como hijo de hoja_min solo si hoja_min no es nullptr
    if (hoja_min != nullptr) {
        hoja_min->hijos = T;
    }
}


int main() {
    // Create some sample data
    Mtree tree1;
    Mtree tree2;

    Entry entry1, entry2, entry3;
    entry1.centro.x = 0.5;
    entry1.centro.y = 0.5;

    entry2.centro.x = 0.8;
    entry2.centro.y = 0.7;

    entry3.centro.x = 0.2;
    entry3.centro.y = 0.9;

    tree1.raiz.insertar(entry1);
    tree1.raiz.insertar(entry2);
    tree2.raiz.insertar(entry3);

    // Call the Link function
    Link(&tree2.raiz, &tree1.raiz);

    // Print the result
    tree1;


    return 0;
}
