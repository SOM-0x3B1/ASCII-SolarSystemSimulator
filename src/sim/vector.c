#include "vector.h"
#include "math.h"

double negyzet(double x){
    return x * x;
}

Vektor vektor_osszeAd(Vektor *u, Vektor *v){
    Vektor res = {u->x + v->x, u->y + v->y};
    return res;
}

Vektor vektor_kivon(Vektor *u, Vektor *v){
    Vektor res = {u->x - v->x, u->y - v->y};
    return res;
}

double vektor_szoroz(Vektor *u, Vektor *v){
    double res = (u->x * v->x) + (u->y * v->y);
    return res;
}

Vektor vektor_skalarSzoroz(Vektor *u, double skalar){
    Vektor res = {u->x * skalar, u->y * skalar};
    return res;
}

Vektor vektor_skalarOszt(Vektor *u, double skalar){
    Vektor res = {u->x / skalar, u->y / skalar};
    return res;
}

Vektor vektor_negal(Vektor *a){
    Vektor res = {-(a->x), -(a->y)};
    return res;
}

double vektor_tavolsag(Vektor *u, Vektor *v){
    return sqrt(negyzet(u->x - v->x) + negyzet(u->y - v->y));
}

double vektor_hossz(Vektor *a){
    return sqrt((a->x) + negyzet(a->y));
}

double vektor_rad(Vektor *u, Vektor *v){
    double szorzat = vektor_szoroz(u, v);
    double hU = vektor_hossz(u);
    double hV = vektor_hossz(v);

    return acos(szorzat / (hU * hV));
}

Pont vektor_toPont(Vektor u){
    Pont p = {(int)u.x, (int)u.y};
    return p;
}