#include "SextonSwinbank.cpp"
#include "CiacciaPatella.cpp"
#include <fstream>

extern vector<Nodo *> toDelete;

void ssExperiment(vector<Point>&Cin, vector<Query> querys, unsigned long long size){
    ofstream file;
    file.open("results/ssExperiment" + to_string(size) + ".txt");
    auto start = chrono::high_resolution_clock::now();
    Mtree mtree = Mtree();
    mtree.raiz = SextonSwinbank(Cin);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    file << "Size: " << size << endl;
    file << "Time: " << duration.count() << " microseconds" << endl;
    file << "Result;Time;I/0s"  << endl;
    for (Query q : querys){
        Search(&q, mtree);
        file << q.resultado.size() << ";" << q.time << ";" << q.ios << endl;
    }
}

void cpExperiment(vector<Point>&Cin, vector<Query> querys, unsigned long long size){
    ofstream file;
    file.open("results/ssExperiment" + to_string(size) + ".txt");
    auto start = chrono::high_resolution_clock::now();
    Mtree mtree = Mtree();
    mtree.raiz = Ciaccia_Patella(Cin);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(end - start);
    file << "Size: " << size << endl;
    file << "Time: " << duration.count() << " microseconds" << endl;
    file << "Result;Time;I/0s"  << endl;
    for (Query q : querys){
        Search(&q, mtree);
        file << q.resultado.size() << ";" << q.time << ";" << q.ios << endl;
    }
}

int main(){
    vector <Nodo *> toDelete;
    vector <Point> Cin;
    vector <Query> querys;
    for (int i = 10; i < 21; i++){
        set<Point> puntos = generatePoints(pow(2, i));
        Cin = vector<Point>(puntos.begin(), puntos.end());
        querys = QueryGen(100);
        ssExperiment(Cin, querys, pow(2, i));
        for (Nodo *n : toDelete){
            delete n;
        }
    }
    for (Nodo *n : toDelete){
        delete n;
    }
    return 0;
}