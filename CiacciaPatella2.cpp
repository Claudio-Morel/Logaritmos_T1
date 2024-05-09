#include <iostream>
#include "structures.h"
#include "points.cpp"
#include <algorithm>
using namespace std;
#include<tuple>
#include <chrono>
#include <queue>

int B = 4096/sizeof(Entry);
#define h 1

tuple<int,double> checkDistance(Point point, vector<Point> samples){
    double dist;
    double minDist = 2;
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
                if (dist < MinDist) {
                    MinDist = dist;
                    hoja_min = &entrada; // Asignar la referencia al nodo más cercano
                }
            }
        }
    }
    
    // Asignar T como hijo de hoja_min solo si hoja_min no es nullptr
    if (hoja_min != nullptr) {
        hoja_min->hijos = T;
    }
}

Nodo* Ciaccia_Patella(vector<Point> points){
    unsigned long long n = points.size();
    random_device rd;
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    mt19937 gen(seed);
    cout << "Paso 0" << endl;
    Nodo *nodo = new Nodo();
    if(n <= B){
        Entry entry;
        (*nodo).entradas;
        for (int i = 0; i < n; i++){
            entry.centro = points[i];
            (*nodo).insertar(entry);
        }
        return nodo;
    }
    else{
        cout << "Paso 2 "<< endl;
        //Calculamos k
        long long k = min((long long)B, (long long)ceil((n/(double)B)));
        vector <vector <Point>> clusters(k); 
        vector<Point> samples;
        vector<double> distancia_max(k,0);
        while(true){
            uniform_int_distribution<> dis(0, n-1);
            // Elegimos k puntos random y los agregamos a samples.
            cout << "Paso 2.1" << endl;
            for (int i = 0; i<k; i++){
                unsigned long long indice = dis(gen);
                samples.push_back(points[indice]);
            }
            // Asociamos todos los puntos a su sample más cercano.
            cout << "Paso 3" << endl;
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
            cout << "Paso 4.1" << endl;
            //Revisamos si los debemos borrar
            for (int i = 0; i<clusters.size(); i++){
                if (clusters[i].size() < B/2){
                    toErase.push_back(i);
                }
            }
            //Los borramos y reasignamos los puntos.
            cout << "Paso 4.2" << endl;
            for (int i = toErase.size()-1; i>=0; i--){                
                vector<Point> rearrangePoints = clusters[toErase[i]];
                rearrangePoints.push_back(samples[toErase[i]]);
                clusters.erase(clusters.begin() + toErase[i]);
                samples.erase(samples.begin() + toErase[i]);
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
                cout << "Paso 5.1" << endl;
                break;
            }else {
                cout << "Paso 5.2" << endl;
                samples.clear();
                toErase.clear();
                clusters.resize(k);
                for(int i =0; i<k; i++){
                    clusters[i].clear();
                }
            }
        }
        vector<Nodo *> hijos; 
        cout << "Paso 6"<< endl;
        for(int i = 0; i< samples.size();i++){
            //Paso 6
            Nodo *hijo = Ciaccia_Patella(clusters[i]);
            hijos.push_back(hijo);
            
        }
        //Paso 7
        cout << "Paso 7" << endl;
        vector<int> Borrar;
        for(int i = 0; i< hijos.size(); i++){
            if((hijos)[i]->entradas.size() < B/2){
                //Borramos su raiz
                //Eliminamos pfj de F
                //Agregamos sus hijos2 a hijos y se añaden sus puntos a F 
                for (Entry e: (hijos[i])->entradas){
                    if(e.hijos!= NULL){
                        hijos.push_back(e.hijos);
                        samples.push_back(e.centro);
                    }
                }
                Borrar.push_back(i);
            }
        }
        cout << "Paso 7.2" << endl;
        for(int k = Borrar.size() - 1; k >= 0; k--){
            cout << "Paso 7.2.1" << endl;
            samples.erase(samples.begin()+Borrar[k]);
            cout << "Paso 7.2.2" << endl;
            hijos.erase(hijos.begin() + Borrar[k]);
        }

        cout << "Paso 8" << endl;
        vector<Nodo *> Tprima;

        cout<< "Paso 9 "<< endl;
        int i= 0;
        for(Nodo* hijo: hijos){
            // Paso 9
            if((*hijo).altura == h){
                Tprima.push_back(hijo);
            }else{
                cout << "Paso 9.1" << endl;
                samples.erase(samples.begin() + i);
                cout << "Paso 9.2"<< endl;
                //Trabajamos con los hijos2 del hijo.
                for(int j = 0; j< hijo->entradas.size() ;j++){
                    Nodo *hijo2 = hijo->entradas[j].hijos;
                    if((*hijo2).altura = h){
                        Tprima.push_back(hijo2);
                        cout << "Paso 9.3" << endl;
                        samples.push_back(hijo->entradas[j].centro);
                    }
                }
            }
            i++;
        }
        cout << "Paso 10"<<endl;
        Nodo* Tsup = new Nodo();
        Tsup = Ciaccia_Patella(samples);

        cout << "Paso 11" << endl;
        for(Nodo *nodo: Tprima){
            Link(nodo,Tsup);
        }
        

        cout << "Paso 12" << endl;
        //Setear radios covertores

        cout << "Paso 13" << endl;
        //Se retorna el arbol
        return Tsup;


    }
}

int main(){
    set<Point> points = generatePoints(pow(2, 20));
    vector<Point> v(points.begin(), points.end());
    Nodo *nodo = Ciaccia_Patella(v);
    return 0;
}
