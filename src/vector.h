#ifndef ASCII_SSS_VECTOR_H
#define ASCII_SSS_VECTOR_H

#endif //ASCII_SSS_VECTOR_H

typedef struct Vektor{
    double x;
    double y;
} Vektor;

typedef struct Pont{
    int x;
    int y;
} Pont;


Vektor vektor_osszeAd(Vektor *u, Vektor *v);

Vektor vektor_kivon(Vektor *u, Vektor *v);

Vektor vektor_skalarSzoroz(Vektor *u, double skalar);

Vektor vektor_skalarOszt(Vektor *u, double skalar);

Vektor vektor_negal(Vektor *u);

double vektor_tavolsag(Vektor *u, Vektor *v);

Pont vektor_toPont(Vektor u);