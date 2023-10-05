#include "vector.h"
#include "math.h"

double negyzet(double a){
    return a * a;
}

Vektor vektor_osszeAd(Vektor *u, Vektor *v){
    Vektor w = {u->x + v->x, u->y + v->y};
    return w;
}

Vektor vektor_kivon(Vektor *u, Vektor *v){
    Vektor w = {u->x - v->x, u->y - v->y};
    return w;
}

Vektor vektor_skalarSzoroz(Vektor *u, double skalar){
    Vektor w = {u->x * skalar, u->y * skalar};
    return w;
}

Vektor vektor_skalarOszt(Vektor *u, double skalar){
    Vektor w = {u->x / skalar, u->y / skalar};
    return w;
}

Vektor vektor_negal(Vektor *u){
    Vektor w = {-(u->x), -(u->y)};
    return w;
}

double vektor_tavolsag(Vektor *u, Vektor *v){
    return sqrt(negyzet(u->x - v->x) + negyzet(u->y - v->y));
}

Pont vektor_toPont(Vektor u){
    Pont p = {(int)u.x, (int)u.y};
    return p;
}