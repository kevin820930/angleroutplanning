#ifndef _ROUTPLANNING_H
#define _ROUTPLANNING_H

typedef struct rout{
    long double polynomial[100];
}rout;


rout routplanning(double armbaselong,double arm2rate,double arm3rate,double angle1,double angle2,double angle3,double angle4,double distance,int pointsnum);

#endif

