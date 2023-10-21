#ifndef ASCII_SSS_VECTOR_H
#define ASCII_SSS_VECTOR_H


typedef struct Vector{
    double x;
    double y;
} Vector;

typedef struct Point{
    long long int x;
    long long int y;
} Point;


Vector vector_add(Vector u, Vector v);

Vector vector_subtract(Vector u, Vector v);

double vector_multiply(Vector u, Vector v);

Vector vector_scalarMultiply(Vector u, double skalar);

Vector vector_scalarDivide(Vector u, double skalar);

Vector vector_invert(Vector a);

double vector_distance(Vector u, Vector v);

double vector_length(Vector a);

double vector_angle(Vector u, Vector v);

Point vector_toPoint(Vector u);


Point point_subtract(Point p, Point q);

Point point_scalarDivide(Point p, int d);


#endif //ASCII_SSS_VECTOR_H