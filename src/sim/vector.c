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

Vector vector_scalarDivision(Vector u, double skalar){
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
    double hU = vector_length(u);
    double hV = vector_length(v);

    return acos(szorzat / (hU * hV));
}

Point vector_toPoint(Vector u){
    Point p = {(int)u.x, (int)u.y};
    return p;
}