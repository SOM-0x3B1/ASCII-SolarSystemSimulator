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


/** Adds two vectors together. */
Vector vector_add(Vector u, Vector v);

/** Subracts two vectors from each other. */
Vector vector_subtract(Vector u, Vector v);

//double vector_multiply(Vector u, Vector v);

/** Multiplies a vector with a skalar. */
Vector vector_scalarMultiply(Vector u, double m);

/** Divides a vector with a skalar. */
Vector vector_scalarDivide(Vector u, double d);

/**
 * Inverts a vector.
 * @return (-a.x, -a.y)
 */
Vector vector_invert(Vector a);

/** Measures the distance between two coord. vectors. */
double vector_distance(Vector u, Vector v);

//double vector_length(Vector a);

//double vector_angle(Vector u, Vector v);

/** Returns the unit vector of a vector between two coord. vectors. */
Vector vector_unitVector(Vector u, Vector v);

/** Converts a Vector to a Point. */
Point vector_toPoint(Vector a);


/** Subracts two vectors from each other. */
Point point_subtract(Point p, Point q);

/** Multiplies a vector with a skalar. */
Point point_scalarMultiply(Point p, int m);

//Point point_scalarDivide(Point p, int d);

//Vector point_ToVector(Point p);


#endif //ASCII_SSS_VECTOR_H