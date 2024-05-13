#include "structures.cpp"
#include "structures.h"
#include <cmath>
#include <iostream>
#include <algorithm>

vector<Cluster> minMaxSplit(Cluster &Cunion){
    double minRadio = 2;
    vector<Cluster> clusters;
    Cluster c1, c2, c3, c4;
    for (vector<Point>::iterator it = Cunion.puntos.begin(); it != Cunion.puntos.end(); it++){
        for (vector<Point>::iterator it2 = next(it); it2 != Cunion.puntos.end(); it2++){
            vector <Point> puntos = Cunion.puntos;
            c3 = Cluster((*it));
            c4 = Cluster((*it2));
            double distC3, distC4;
            for (int i = 0; i<Cunion.count; i++){
                if (i%2 == 0){
                    double minDist = 2;
                    vector <Point>::iterator itToAdd;;
                    for (vector<Point>::iterator it3 = puntos.begin(); it3 != puntos.end(); ++it3){
                        distC3 = (*it).distance((*it3));
                        if (distC3 < minDist){
                            itToAdd = it3;
                            minDist = distC3;
                        }
                    }
                    c3.radio = distC3;
                    Point p = (*itToAdd);
                    c3.insertar(p);
                    puntos.erase(itToAdd);
                }else{
                    double minDist = 2;
                    vector <Point>::iterator itToAdd;
                    for (vector<Point>::iterator it3 = puntos.begin(); it3 != puntos.end(); ++it3){
                        distC4 = (*it2).distance((*it3));
                        if (distC4 < minDist){
                            itToAdd = it3;
                            minDist = distC4;
                        }
                    }
                    c4.radio = distC4;
                    Point p = (*itToAdd);
                    c4.insertar(p);
                    puntos.erase(itToAdd);
                }
            }
            double maxRadio = max(c3.radio, c4.radio);
            if (maxRadio < minRadio){
                minRadio = maxRadio;
                c1 = c3;
                c2 = c4;
            }
        }
    }
    clusters.push_back(c1);
    clusters.push_back(c2);
    return clusters;
}

tuple<unsigned long long, unsigned long long> twoClosestPoints(vector<Cluster>& clusters){
    double minDist = 2;
    tuple <unsigned long long, unsigned long long> indices;
    unsigned long long a,b;
    for (unsigned long long i = 0; i<clusters.size(); i++){
        for (unsigned long long j = i+1; j<clusters.size(); j++){
            double dist = (clusters[i].medoide).distance(clusters[j].medoide);
            if (dist < minDist){
                minDist = dist;
                a = i;
                b = j;
            }
        }
    }
    indices = make_tuple(a,b);
    return indices;
}

bool compareX(const Point &a, const Point &b) {
    return (a.x < b.x); 
} 
 
bool compareY(const Point &a, const Point &b) { 
    return (a.y < b.y); 
} 

double min(double x, double y) { 
	if (x < y) 
        return x;
    return y;
} 

tuple <double, Point, Point> stripClosest(Point strip[], int size, double d) { 
	double min = d; 
    Point a,b; 
    sort(strip, strip + size, compareY);
	for (int i = 0; i < size; ++i) {
		for (int j = i+1; j < size && (strip[j].y - strip[i].y) < min; ++j) {
			if (strip[i].distance(strip[j]) < min) {
				min = strip[i].distance(strip[j]); 
                a = strip[i];
                b = strip[j];
            }
        }
    }
	return make_tuple(min,a,b); 
} 

tuple <double, Point, Point> bruteForce(Point P[], int n) { 
    double min = 3; 
    Point a,b;
    for (int i = 0; i < n; ++i) {
        for (int j = i+1; j < n; ++j) {
            if (P[i].distance(P[j]) < min){ 
                min = P[i].distance(P[j]); 
                a = P[i];
                b = P[j];
            }
        }
    }
    return make_tuple(min,a,b);
} 

tuple<double, Point, Point> closestUtil(Point P[], int n) { 
	if (n <= 3) 
		return bruteForce(P, n);  
	int mid = n/2;
	Point midPoint = P[mid]; 
    Point a,b;
	tuple <double, Point, Point> dl = closestUtil(P, mid); 
	tuple <double, Point, Point> dr = closestUtil(P + mid, n - mid); 
    double d;
    if ((get<0>(dl)) < (get<0>(dr))){
        d = get<0>(dl);
        a = get<1>(dl);
        b = get<2>(dl);
    } else{
        d = get<0>(dr);
        a = get<1>(dr);
        b = get<2>(dr);
    }
	int j = 0; 
	for (int i = 0; i < n; i++){
		if (abs(P[i].x - midPoint.x) < d) {
            j++; 
        }
    }
    Point strip[j];
    int k = 0;
    for (int i = 0; i < n; i++){
		if (abs(P[i].x - midPoint.x) < d) {
			strip[k] = P[i];
            k++;
        }
    }
    tuple <double, Point, Point> other = stripClosest(strip, j, d);
    if (get<0>(other) < d){
        return other;
    }
    return make_tuple(d,a,b);
    
} 


tuple <unsigned long long, unsigned long long> twoClosestPoints2(vector<Cluster> &Cin) {
    Point *P = new Point[Cin.size()];
    for (int i = 0; i < Cin.size(); i++){  
        P[i] = (Cin[i]).medoide;
    }
    sort(P, P + Cin.size(), compareX);
    tuple <double, Point, Point> min = closestUtil(P, Cin.size());
    Point a = get<1>(min);
    Point b = get<2>(min);
    tuple <unsigned long long, unsigned long long> indices;
    for (unsigned long long i = 0; i<Cin.size(); i++){
        if (a.x == Cin[i].medoide.x && a.y == Cin[i].medoide.y){
            get<0>(indices) = i;
        }
        if (b.x == Cin[i].medoide.x && b.y == Cin[i].medoide.y){
            get<1>(indices) = i;
        }
    }
    delete [] P;
    return indices;
}
