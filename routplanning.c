#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>
#include <assert.h>

#include "straight.h"
#include "mylagrange.h"
#include "routplanning.h"
 
rout routplanning(double armbaselong,double arm2rate,double arm3rate,double angle1,double angle2,double angle3,double angle4,double distance,int pointsnum){

    motor *origmotor=(motor *)malloc(sizeof(motor));

    origmotor->motor1angle=angle1;
    origmotor->motor2angle=angle2;
    origmotor->motor3angle=angle3;
    origmotor->motor4angle=angle4;

    arm *armdata=(arm *)malloc(sizeof(arm));

    armdata->longbase=armbaselong;
    armdata->longrate2=arm2rate;
    armdata->longrate3=arm3rate;

    calcu(armdata,origmotor,distance,pointsnum);

    rout *routpoly=(rout *)malloc(sizeof(rout));
   
    long double data[100][2];
    char first[20],second[20],readdata[100];
    int a,linenum,mid,i,end;
    FILE *fileopen;

    for(i=0;i<100;i++){
	routpoly->polynomial[i]=0;
    }
    
    linenum=0;
    for(i=0;i<100;i++){
        readdata[i]='\0';
        data[i][0]=0;
	data[i][1]=0;
    }
    for(i=0;i<20;i++){
        first[i]='\0';
        second[i]='\0';
    }

    if(!(fileopen=fopen("moveangle.txt","r"))){
	printf("read file from move angle failed");
    }
    
    
    while(fgets(readdata,sizeof(readdata),fileopen)){
        for(i=0;readdata[i]!=' ';i++){
            if(readdata[i]==','){
                mid=i;
            }
        }
        end=i;
        for(a=0;a<mid;a++){
            first[a]=readdata[a];
        }

        data[linenum][0]=atof(first);

        for(i=0,a=(mid+1);a<end;a++,i++){
            second[i]=readdata[a];
        }
        data[linenum][1]=atof(second);

        for(i=0;i<100;i++){
        readdata[i]='\0';
        }
        for(i=0;i<20;i++){
            first[i]='\0';
            second[i]='\0';
        }
        linenum++;
    }
 
    fclose(fileopen);
    printf("\nThe fuction of first(x) and third(f(x)) motor angle:\n");
    lagrange(data,pointsnum,routpoly);
    
    if(!(fileopen=fopen("moveangle.txt","w"))){
        printf("read file from move angle failed");
    }
    long double tmpangle =angle4;
    for(i=0;i<pointsnum;i++){
	if(i==0)
	    fprintf(fileopen,"%Lf,%Lf,%Lf \n",data[i][0],data[i][1],tmpangle-=((data[i][0]-angle1)+(data[i][1]-angle3)));
	else
	    fprintf(fileopen,"%Lf,%Lf,%Lf \n",data[i][0],data[i][1],tmpangle-=((data[i][0]-data[i-1][0])+(data[i][1]-data[i-1][1])));
    }
    fclose(fileopen);   
    return *routpoly;
}

