#include <vector>
#include <algorithm>
#include <iostream> 
#include "structures.h"
#include "utils.cpp"
#include "Search.cpp"
#include <queue>
using namespace std;

int B = 4096/sizeof(Entry);
vector <Nodo *> toDelete;

vector<Cluster> ClusterGen(vector<Point>& Cin) {
    // Step 1
    vector<Cluster> Cout = vector<Cluster>();
    vector<Cluster> C;

    // Step 2
    for (Point & p : Cin) {
        Cluster c = Cluster(p);
        c.insertar(p);
        C.push_back(c);
    }

    // Step 3
    double minDist = 2;
    while (C.size() > 1) {
        // Step 3.1 (Encontrar los dos clusters más cercanos)
        tuple<unsigned long long, unsigned long long> indices2 = twoClosestPoints2(C);
        unsigned long long a = get<0>(indices2);
        unsigned long long b = get<1>(indices2);
        // Step 3.2
        if ((C[a]).count + (C[b]).count <= B) {
            for (Point punto : (C[b]).puntos) {
                (C[a]).insertar(punto);
            }
            C[a].actualizarMedoide();
            C.erase(C.begin() + b);
        
        //step 3.3
        } else {
            if ((C[a]).count < (C[b]).count) {
                Cluster c = C[b];
                C.erase(C.begin() + b);
                Cout.push_back(c);
            } else {
                Cluster c = C[a];
                C.erase(C.begin() + a);
                Cout.push_back(c);
            }
        }
    }
    // Step 4
    Cluster c = C.back();
    Cluster cPrima;
    // Step 5
    if (!Cout.empty()) {
        //step 5.1
        vector <Cluster>::iterator it_cPrima;
        double minDist = 2;
        for (vector<Cluster>::iterator it = Cout.begin(); it != Cout.end(); ++it) {
            double dist = c.medoide.distance((*it).medoide);
            if (dist < minDist){
                minDist = dist;
                it_cPrima = it;
            }
        }
        cPrima = *it_cPrima;
        Cout.erase(it_cPrima);
    }
    else{
        cPrima = Cluster();
    }
    // Step 6
    for (Point punto : cPrima.puntos) {
        c.insertar(punto);
    }
    c.actualizarMedoide();
    if (c.count <= B) {
        Cout.push_back(c);
    } else {
        // Step 6.2
        vector <Cluster> C1C2 = minMaxSplit(c);
        for (Cluster c : C1C2) {
            Cout.push_back(c);
        }
    }
    // Step 7
    return Cout;
}

Entry OutputHojaCluster(Cluster &C){
    Nodo *hojas = new Nodo();
    for (Point p : C.puntos) {
        (*hojas).insertar(Entry{p,0,nullptr});
    }
    return Entry{C.medoide, C.radio, hojas};
}

Entry OutputHoja(vector<Point> &Cin){
    Cluster C = Cluster();
    Nodo *hojas = new Nodo();
    toDelete.push_back(hojas);
    for (Point p : Cin) {
        C.insertar(p);
        (*hojas).insertar(Entry{p, 0, nullptr});
    }
    C.actualizarMedoide();
    Point g =  C.medoide;
    double r = C.radio;
    return Entry{g, r, hojas};
}

Entry OutputInterna(vector<Entry> &Cmra){
    Point G;
    double R = 0;
    Nodo *C = new Nodo();
    toDelete.push_back(C);
    vector<Entry>::iterator it1;
    for (vector<Entry>::iterator it = Cmra.begin(); it != Cmra.end(); it++) {
        double maxDist = 0;
        for (vector<Entry>::iterator jt = Cmra.begin(); jt != Cmra.end(); jt++) {
            double dist = (*it).centro.distance((*jt).centro) + jt->radio;
            if (maxDist < dist) {
                maxDist = dist;
            }
        }
        if (R < maxDist) {
            R = maxDist;
            G = (*it).centro;
        }
        (*C).insertar(*it);
    }
    return Entry{G, R, C};
}

Nodo *SextonSwinbank(vector<Point> &Cin){
    cout << "Creando el arbol con " << Cin.size() << " puntos..." << endl;
    if (Cin.size()< B){
        Entry toReturn = OutputHoja(Cin);
        return toReturn.hijos;
    }
    vector<Cluster> Cout = ClusterGen(Cin);
    cout << "Clusters generados: " << Cout.size() << endl;
    vector<Entry> C;
    for (Cluster &c : Cout) {
        C.push_back(OutputHojaCluster(c));
    }
    while (C.size() > B) {
        cout << "Merging clusters..." << endl;
        cout << "Clusters restantes: " << C.size() << endl;
        vector<Point> Cin2;
        for (Entry e : C) {
            Cin2.push_back(e.centro);
        }
        Cout = ClusterGen(Cin2);
        vector <vector <Entry>> Cmra;
        for (Cluster c : Cout) {
            c.actualizarMedoide();
            vector<Entry> s;
            for (Entry e : C) {
                for (Point p : c.puntos){
                    if (p.x == e.centro.x && p.y == e.centro.y){
                        s.push_back(e);
                    }
                } 
            }
            Cmra.push_back(s);
        }
        C = vector <Entry>();
        for (vector<Entry> s : Cmra) {
            C.push_back(OutputInterna(s));
        }
    }
    Entry Out = OutputInterna(C);
    return Out.hijos;  
}
