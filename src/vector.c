#include "vector.h"
#include "math.h"

/**
 * Raises a number to the power of 2.
 * @param x base
 * @return base^2
 */
static double pow2(double x){
    return x * x;
}


Vector vector_create(double length, double degree){
    Vector v;
    double rad = degree * (3.14159265359 / 180.0);
    v.x = length * cos(rad);
    v.y = length * sin(rad);
    return v;
}

Vector vector_add(Vector u, Vector v){
    Vector res = {u.x + v.x, u.y + v.y};
    return res;
}

Vector vector_subtract(Vector u, Vector v){
    Vector res = {u.x - v.x, u.y - v.y};
    return res;
}

/*double vector_multiply(Vector u, Vector v){
    double res = (u.x * v.x) + (u.y * v.y);
    return res;
}*/

Vector vector_scalarMultiply(Vector u, double m){
    Vector res = {u.x * m, u.y * m};
    return res;
}

Vector vector_scalarDivide(Vector u, double d){
    Vector res = {u.x / d, u.y / d};
    return res;
}

Vector vector_invert(Vector a){
    Vector res = {-(a.x), -(a.y)};
    return res;
}

double vector_distance(Vector u, Vector v){
    return sqrt(pow2(u.x - v.x) + pow2(u.y - v.y));
}

/*double vector_length(Vector a){
    return sqrt((a.x) + pow2(a.y));
}*/

/*double vector_angle(Vector u, Vector v){
    double m = vector_multiply(u, v);
    double dU = vector_length(u);
    double dV = vector_length(v);

    return acos(m / (dU * dV));
}*/

Vector vector_unitVector(Vector u, Vector v){
    Vector w = vector_subtract(u, v);
    double d = vector_distance(u, v);
    return vector_scalarDivide(w, d);
}

Point vector_toPoint(Vector a) {
    Point p = {(long long int) round(a.x), (long long int) round(a.y)};
    return p;
}


Point point_subtract(Point p, Point q){
    Point res = {p.x - q.x, p.y - q.y};
    return res;
}

Point point_scalarMultiply(Point p, int m){
    Point res = {p.x * m, p.y * m};
    return res;
}

/*Point point_scalarDivide(Point p, int d){
    Point res = {p.x / d, p.y / d};
    return res;
}*/

/*Vector point_ToVector(Point p){
    Vector v = (Vector){(double)p.x, (double)p.y};
    return v;
}*/