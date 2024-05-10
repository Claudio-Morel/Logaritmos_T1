#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include "structures.h"
#include "points.cpp"

void Link(vector<Nodo*> T, Nodo* Tsup) {
    vector<Entry*> Hojas;
    queue<Nodo*> q;
    q.push(Tsup);
    while (!q.empty()) {
        Nodo* nodo = q.front();
        q.pop();
        for (Entry& entrada : nodo->entradas) {
            if (entrada.hijos != NULL) {
                q.push(entrada.hijos);
            } else {
                Hojas.push_back(&entrada); // Agregar entrada hoja al vector E
            }
        }
    }

    // Iterar sobre cada nodo en el vector T
    for (Nodo* T_node : T) {
        Point punto = T_node->entradas[0].centro; // Suponiendo que siempre hay al menos una entrada en cada nodo T
        double MinDist = 2;
        Entry* hoja_min = nullptr; // Inicializa a nullptr para evitar comportamiento indefinido

        // Encontrar la hoja más cercana en el vector E
        for (Entry* entry : Hojas) {
            double dist = punto.distance(entry->centro);
            if (dist < MinDist) {
                MinDist = dist;
                hoja_min = entry;
            }
        }

        // Asignar T_node como hijo de hoja_min si hoja_min no es nullptr
        if (hoja_min != nullptr) {
            cout << "AAAAA" << endl;
            hoja_min->hijos = T_node;
            cout << "o" << endl;
        }

    }
}

int ALT(Nodo* nodo) {
    int maxAlt = 0;
    for (Entry entrada : (*nodo).entradas) {
        int altura;
        if (entrada.hijos == NULL) {
            altura = 1;
        } else {
            altura = 1 + ALT(entrada.hijos);
        }
        if (altura > maxAlt) {
            maxAlt = altura;
        }
    }
    return maxAlt;
}





int main() {
    // Crear el árbol Tsup con dos hojas
    Nodo Tsup;
    Entry hoja1, hoja2;
    hoja1.centro.x = 0.3;
    hoja1.centro.y = 0.4;
    hoja2.centro.x = 0.6;
    hoja2.centro.y = 0.7;
    Tsup.entradas.push_back(hoja1);
    Tsup.entradas.push_back(hoja2);

    // Crear el vector T con dos nodos
    vector<Nodo*> T;
    Nodo node1, node2;
    Entry entry1, entry2;
    entry1.centro.x = 0.2;
    entry1.centro.y = 0.5;
    entry2.centro.x = 0.8;
    entry2.centro.y = 0.9;
    node1.entradas.push_back(entry1);
    node2.entradas.push_back(entry2);
    T.push_back(&node1);
    T.push_back(&node2);

    // Enlazar los nodos del vector T con las hojas del árbol Tsup
    Link(T, &Tsup);

    cout << "ALT :"<< ALT(&Tsup)<< endl;


    return 0;
}






