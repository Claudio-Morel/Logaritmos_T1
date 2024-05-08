#include <vector>
#include <algorithm>
#include "structures.h"
#include "utils.cpp"
#include "Search.cpp"
#include <queue>
using namespace std;

int B = 4096/sizeof(Entry);
vector <Nodo *> toDelete;

vector<Cluster> ClusterGen(vector<Point>& Cin) {
    // Step 1
    cout << "Step 1" << endl;
    vector<Cluster> Cout = vector<Cluster>();
    vector<Cluster> C;

    // Step 2
    cout << "Step 2" << endl;
    for (Point & p : Cin) {
        Cluster c = Cluster(p);
        c.insertar(p);
        C.push_back(c);
    }

    // Step 3
    cout << "Step 3" << endl;
    double minDist = 2;
    while (C.size() > 1) {
       // cout << C.size() << endl;
        // Step 3.1 (Encontrar los dos clusters más cercanos)
        tuple <unsigned long long, unsigned long long> indices = twoClosestPoints(C);
        unsigned long long a = get<0>(indices);
        unsigned long long b = get<1>(indices);
        // Step 3.2
        if ((C[a]).count + (C[b]).count <= B) {
            for (Point punto : (C[b]).puntos) {
                (C[a]).insertar(punto);
            }
            (C[a]).actualizarMedoide();
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
    cout << "Step 4" << endl;
    cout << C.size() << endl;
    Cluster c = C.back();
    Cluster cPrima;
    // Step 5
    cout << "Step 5" << endl;
    cout << Cout.size() << endl;
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
    cout << "Step 6" << endl;
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
    cout << "Step 7" << endl;
    return Cout;
}

Entry OutputHojaCluster(Cluster &C){
    Nodo *hojas = new Nodo();
    for (Point p : C.puntos) {
        (*hojas).insertar(Entry{p});
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
    for (vector<Entry>::iterator it = Cmra.begin(); it != Cmra.end(); ++it) {
        double maxDist = 0;
        for (vector<Entry>::iterator jt = Cmra.begin(); jt != Cmra.end(); ++jt) {
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
    if (Cin.size()< B){
        Entry toReturn = OutputHoja(Cin);
        return toReturn.hijos;
    }
    vector<Cluster> Cout = ClusterGen(Cin);
    vector<Entry> C;
    for (Cluster &c : Cout) {
        C.push_back(OutputHojaCluster(c));
    }
    while (C.size() > B) {
        vector<Point> Cin2;
        for (Entry e : C) {
            Cin2.push_back(e.centro);
        }
        Cout = ClusterGen(Cin2);
        vector <Entry> Cmra;
        for (Cluster c : Cout) {
            for (Entry e : C) {
                for (Point p : c.puntos){
                    if (p.x == e.centro.x && p.y == e.centro.y){
                        Cmra.push_back(e);
                    }
                }
            }
        }
        C = vector <Entry>();
        for (Entry e : Cmra) {
            C.push_back(OutputInterna(e.hijos->entradas));
        }
    }
    Entry Out = OutputInterna(C);
    return Out.hijos;  
}

int main(){
    Mtree mtree = Mtree();
    set<Point> puntos = generatePoints(pow(2, 12));
    vector<Point> Cin(puntos.begin(), puntos.end());
    Nodo *raiz = SextonSwinbank(Cin);
    mtree.raiz = raiz;
    vector<Query> querys = QueryGen(100);
    for (Query q : querys){
        Search(&q, mtree);
        if (q.resultado.size() > 0){
            cout << "Resultados de la query:" << endl;
            cout << "Query realizada en el punto: "<< q.punto.x << ", " << q.punto.y << endl;
            cout << "Radio de la query: " << q.radio << endl;
            cout << "Resultados: " << q.resultado.size() << endl;
        }
    }
    for (Nodo * n : toDelete){
        delete n;
    }
    return 0;
}
