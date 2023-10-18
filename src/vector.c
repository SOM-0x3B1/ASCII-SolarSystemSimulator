#include "vector.h"
#include "math.h"


double negyzet(double x){
    return x * x;
}

Vector vector_add(Vector u, Vector v){
    Vector res = {u.x + v.x, u.y + v.y};
    return res;
}

Vector vector_subtract(Vector u, Vector v){
    Vector res = {u.x - v.x, u.y - v.y};
    return res;
}

double vector_multiply(Vector u, Vector v){
    double res = (u.x * v.x) + (u.y * v.y);
    return res;
}

Vector vector_scalarMultiply(Vector u, double skalar){
    Vector res = {u.x * skalar, u.y * skalar};
    return res;
}

Vector vector_scalarDivide(Vector u, double skalar){
    Vector res = {u.x / skalar, u.y / skalar};
    return res;
}

Vector vector_invert(Vector a){
    Vector res = {-(a.x), -(a.y)};
    return res;
}

double vector_distance(Vector u, Vector v){
    return sqrt(negyzet(u.x - v.x) + negyzet(u.y - v.y));
}

double vector_length(Vector a){
    return sqrt((a.x) + negyzet(a.y));
}

double vector_angle(Vector u, Vector v){
    double szorzat = vector_multiply(u, v);
    double dU = vector_length(u);
    double dV = vector_length(v);

    return acos(szorzat / (dU * dV));
}

Point vector_toPoint(Vector u) {
    Point p = {(int) round(u.x), (int) round(u.y)};
    return p;
}


Point point_scalarSubtract(Point p, Point q){
    Point res = {p.x - q.x, p.y - q.y};
    return res;
}

Point point_scalarDivide(Point p, int d){
    Point res = {p.x / d, p.y / d};
    return res;
}