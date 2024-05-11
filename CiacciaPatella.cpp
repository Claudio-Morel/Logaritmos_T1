#include <iostream>
#include "structures.h"
#include "points.cpp"
#include <algorithm>
using namespace std;
#include<tuple>
#include <chrono>

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


Nodo* Ciaccia_Patella(vector<Point> points){
    unsigned long long n = points.size();
    random_device rd;
    unsigned seed = chrono::system_clock::now().time_since_epoch().count();
    mt19937 gen(seed);
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
                n--;
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
                n++;
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
        // HASTA PASO 5 ESTAMOS JOYA!



        // (Paso 6)
        // Vamos calculando recursivamente, asignando los arboles.
        // Para cada Fj
        cout << "paso 6" << endl;
        vector<Nodo *> hijos;
        for(int i = 0; i< samples.size();i++){
            //Arbol Tj
            Nodo *hijo = Ciaccia_Patella(clusters[i]);
            //Paso 7
            cout << "Paso 7" << endl;
            if((*hijo).entradas.size()< B/2){
                //Borramos su raiz
                cout << "Paso 7.1" << endl;
                vector<Entry> raiz = (*hijo).entradas;
                // hijo.raiz = NULL;
                //Sacamos su punto de samples
                cout << "Paso 7.2"<< endl;
                samples.erase(samples.begin()+i);
                //Trabajamos con los hijos2 de este hijo, obteniendo sus raices (que son parte de las entrys del hijo)
                // y dejandolas en samples, agregando estos hijos2 como hijos.
                cout << "Paso 7.3"<< endl;
                for(int j = 0; j< raiz.size();j++){
                    Nodo *hijo2 = raiz[j].hijos;
                    if (hijo2 != NULL) { // Verificar si hijo2 no es NULL antes de acceder a él
                        hijos.push_back(hijo2);
                        samples.push_back(raiz[i].centro);
                        //Revisar si sigo teniendo uno a uno.
                    } else {
                        // Nada
                    }

                }

            }else{
                hijos.insert(hijos.begin()+i,hijo);
            }
        }
        vector<Nodo *> Tprima;
        int i= 0;
        for(Nodo* hijo: hijos){
            cout << "paso 9" << endl;
            // Paso 9
            if((*hijo).altura == h){
                cout << "Caso 1" << endl;
                Tprima.push_back(hijo);
            }else{
                cout << "Caso 2" << endl;
                //Borrar el punto de F.
                vector<int> Posiciones;
                samples.erase(samples.begin()+i);

                vector<Entry> entrys = (*hijo).entradas;
                // Para Cada Tj
                for(int i = 0; i< entrys.size();i++){
                    //Si el hijo tiene altura H lo agrego a T prima
                    Nodo *hijo_2 =  entrys[i].hijos;
                    int altura_hijo = 1;//hijo_2->altura;
                    if(altura_hijo == h){
                        Tprima.push_back(hijo_2);
                        Posiciones.push_back(i);
                    }
                
                }
                for(int i = 0; i< Posiciones.size(); i++){
                int posicion = Posiciones[i]; 
                vector<Entry> entrada_m = (*Tprima[i]).entradas;
                //Insertar pf1 prima en F
                samples.push_back(entrys[i].centro);
                (*hijo).entradas.erase((*hijo).entradas.begin()+i);
                }
            }
            i++;
        }


        //Paso 10!!!!!!!!!
        cout << "paso 10" << endl;
        Nodo* Tsup = Ciaccia_Patella(samples);
        //Paso 11
        for(int i = 0; i<Tprima.size();i++){
            Entry entrada =(*Tsup).entradas[i];
            entrada.hijos = Tprima[i];
        }
        //Paso 12.
        //SETEAR RADIOS!

        return nodo;

        

    }
}

int main(){
    set<Point> points = generatePoints(pow(2, 10));
    set<Point> points = generatePoints(pow(2, 19));
    cout << sizeof(Point)<< endl;
    vector<Point> v(points.begin(), points.end());
    Nodo *nodo = Ciaccia_Patella(v);
    return 0;
}
