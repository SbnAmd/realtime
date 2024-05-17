
#include "qsort.h"



int compare(const void *elem1, const void *elem2)
{
    int result;

    result = strcmp((*((struct myStringStruct *)elem1)).qstring, (*((struct myStringStruct *)elem2)).qstring);

    return (result < 0) ? 1 : ((result == 0) ? 0 : -1);
}


int l_compare(const void *elem1, const void *elem2)
{
    /* D = [(x1 - x2)^2 + (y1 - y2)^2 + (z1 - z2)^2]^(1/2) */
    /* sort based on distances from the origin... */

    double distance1, distance2;

    distance1 = (*((struct my3DVertexStruct *)elem1)).distance;
    distance2 = (*((struct my3DVertexStruct *)elem2)).distance;

    return (distance1 > distance2) ? 1 : ((distance1 == distance2) ? 0 : -1);
}



void qsort_large(){

    struct my3DVertexStruct array[MAXARRAY];
    FILE *fp;
    int i,count=0;
    int x, y, z;

    if (0) {

    }
    else {
        fp = fopen("/home/sbn/Desktop/realtime/C/tasks/qsort/input_large.dat","r");

        while((fscanf(fp, "%d", &x) == 1) && (fscanf(fp, "%d", &y) == 1) && (fscanf(fp, "%d", &z) == 1) &&  (count < MAXARRAY)) {
            array[count].x = x;
            array[count].y = y;
            array[count].z = z;
            array[count].distance = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
            count++;
        }
    }

    qsort(array,count,sizeof(struct my3DVertexStruct),l_compare);


}

void qsort_small(){

    struct myStringStruct array[MAXARRAY];
    FILE *fp;
    int i,count=0;


    fp = fopen("/home/sbn/Desktop/realtime/C/tasks/qsort/input_small.dat","r");

    while((fscanf(fp, "%s", &array[count].qstring) == 1) && (count < MAXARRAY)) {
        count++;
    }

    qsort(array,count,sizeof(struct myStringStruct),compare);


}