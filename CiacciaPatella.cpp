#include <iostream>
#include "structures.h"
#include "SextonSwinbank.cpp"
#include <algorithm>
using namespace std;
#include<tuple>
#include <chrono>
#include <queue>

extern int B;
extern vector<Nodo *> toDelete;

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

// Funcion que une un nodo T a su respectivo padre.
void Link(vector<Nodo*> T, Nodo* Tsup){
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
        int i = 0;
        int posicion = 0;
        for (Entry* entry : Hojas) {
            double dist = punto.distance(entry->centro);
            if (dist < MinDist) {
                MinDist = dist;
                hoja_min = entry;
                posicion = i;
            }
            i++;
        }

        // Asignar T_node como hijo de hoja_min si hoja_min no es nullptr
        if (hoja_min != nullptr) {
            hoja_min->hijos = T_node;
            Hojas.erase(Hojas.begin() + posicion);
        }

    }
}

void setRadio(Nodo * T){
    if(T == nullptr){
        return;
    }
    //Caso de que no sea hoja
    //Primero seteo radio de todos sus hijos
    for(Entry &entrada: T->entradas){
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

Nodo* Ciaccia_Patella(vector<Point> points){
    unsigned long long n = points.size();
    random_device rd;
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    mt19937 gen(seed);
    Nodo *nodo = new Nodo();
    toDelete.push_back(nodo);
    if(n <= B){
        Entry entry;
        (*nodo).entradas;
        for (int i = 0; i < n; i++){
            entry.centro = points[i];
            (*nodo).insertar(entry);
        }
        setRadio(nodo);
        return nodo;
    }
    else{
        //Calculamos k
        long long k = min((long long)B, (long long)ceil((n/(double)B)));
        vector <vector <Point>> clusters(k); 
        vector<Point> samples;
        vector<double> distancia_max(k,0);
        while(true){
            uniform_int_distribution<> dis(0, n-1);
            // Elegimos k puntos random y los agregamos a samples.
            for (int i = 0; i<k; i++){
                unsigned long long indice = dis(gen);
                samples.push_back(points[indice]);
            }
            // Asociamos todos los puntos a su sample más cercano.
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
            //Revisamos si los debemos borrar
            for (int i = 0; i<clusters.size(); i++){
                if (clusters[i].size() < B/2){
                    toErase.push_back(i);
                }
            }
            //Los borramos y reasignamos los puntos.
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
                break;
            }else {
                samples.clear();
                toErase.clear();
                clusters.resize(k);
                for(int i =0; i<k; i++){
                    clusters[i].clear();
                }
            }
        }
        vector<Nodo *> hijos; 
        for(int i = 0; i< samples.size();i++){
            //Paso 6
            Nodo * hijo = new Nodo();
            toDelete.push_back(hijo);
            hijo = Ciaccia_Patella(clusters[i]);
            hijos.push_back(hijo);
            
        }
        //Paso 7
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
        for(int k = Borrar.size() - 1; k >= 0; k--){
            samples.erase(samples.begin()+Borrar[k]);
            hijos.erase(hijos.begin() + Borrar[k]);
        }
        vector<Nodo *> Tprima;
        int h= numeric_limits<int>::max();
        //Calculo de h -> altura minima de todos los hijos.
        for(Nodo* hijo : hijos){
            if(ALT(hijo)<h){
                h =ALT(hijo);
            }
        }
        for (int i = hijos.size()-1; i >= 0; i--){
            Nodo* hijo = hijos[i];
            // Paso 9
            if(ALT(hijo) == h){
                Tprima.push_back(hijo);
            }else{
                samples.erase(samples.begin() + i);
                //Trabajamos con los hijos2 del hijo.
                queue<Nodo*> buscar;
                queue<Point*> punto_asociado;
                buscar.push(hijo);
                punto_asociado.push(((Point*){}));
                while(!buscar.empty()){
                    Nodo * hijo_n = buscar.front();
                    buscar.pop();
                    Point *p = punto_asociado.front();
                    punto_asociado.pop();
                    if(ALT(hijo_n)== h){
                        hijos.push_back(hijo_n);
                        samples.push_back(*p);
                    }else{
                        for(Entry entrada_n : hijo_n->entradas){
                            if(entrada_n.hijos != NULL){
                                buscar.push(entrada_n.hijos);
                                punto_asociado.push(&(entrada_n.centro));
                            }
                            
                        }
                    }
                }
            }
        }
        Nodo* Tsup = new Nodo();
        toDelete.push_back(Tsup);
        Tsup = Ciaccia_Patella(samples);
        Link(Tprima,Tsup);
        

        setRadio(Tsup);

        return Tsup;


    }
}

int main(){
    set <Point> puntos = generatePoints(pow(2, 19));
    vector<Point> Cin(puntos.begin(), puntos.end());
    Mtree mtree = Mtree();
    mtree.raiz = Ciaccia_Patella(Cin);
    return 0;
}
