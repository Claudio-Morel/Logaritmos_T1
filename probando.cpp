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
            if (entrada.hijos != nullptr) {
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
        if (entrada.hijos == nullptr) {
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

void setRadio(Nodo * T){
    if(T == nullptr){
        return;
    }
    //Caso de que no sea hoja
    //Primero seteo radio de todos sus hijos
    for(Entry &entrada: (T->entradas)){
        if(entrada.hijos == nullptr){
            entrada.radio = 0;
        }else{
            double maxDist = 0;
            setRadio(entrada.hijos);
            for(Entry &entrada_h : entrada.hijos->entradas){
                double dist = entrada.centro.distance(entrada_h.centro) + entrada_h.radio;
                if(dist > maxDist){
                    maxDist = dist;
                }
            }
            entrada.radio = maxDist;

        }
    }
}





int main() {
    // Caso de prueba 1: Árbol con un solo nodo
    Nodo singleNode;
    Entry singleEntry;
    singleEntry.centro.x = 0.5;
    singleEntry.centro.y = 0.5;
    singleNode.entradas.push_back(singleEntry);
    setRadio(&singleNode);
    cout << "Radio para el nodo único: " << singleNode.entradas[0].radio << endl;

    // Caso de prueba 2: Árbol con varios niveles
    Nodo root;
    Entry rootEntry;
    rootEntry.centro.x = 0.5;
    rootEntry.centro.y = 0.5;
    root.entradas.push_back(rootEntry);
    Nodo child1, child2;
    Entry childEntry1, childEntry2;
    childEntry1.centro.x = 0.3;
    childEntry1.centro.y = 0.3;
    childEntry2.centro.x = 0.7;
    childEntry2.centro.y = 0.7;
    child1.entradas.push_back(childEntry1);
    child2.entradas.push_back(childEntry2);
    root.entradas[0].hijos = &child1;
    child1.entradas[0].hijos = &child2;
    setRadio(&root);
    cout << "Radio para el nodo raíz: " << root.entradas[0].radio << endl;
    cout << "Radio para el primer hijo: " << root.entradas[0].hijos->entradas[0].radio << endl;

    
    return 0;
}







