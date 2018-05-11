#include "geomshapes.h"
#include <GL/gl.h>

#define drawLine(x1,y1,z1,x2,y2,z2) glBegin(GL_LINES); \
    glVertex3f((x1), (y1), (z1)); glVertex3f((x2), (y2), (z2)); glEnd();

void gridXY(float Xmin, float Ymin, float Xmax, float Ymax, float step) {

    for(float x = Xmin; x <= Xmax; x+=step) {
        drawLine(x, Ymin, 0.0, x, Ymax, 0.0);
    }
    for(float y = Ymin; y <= Ymax; y+=step) {
        drawLine(Xmin, y, 0.0, Xmax, y, 0.0);
    }
}

void gridXZ(float Xmin, float Zmin, float Xmax, float Zmax, float step) {

    for(float x = Xmin; x <= Xmax; x+=step) {
        drawLine(x, 0.0, Zmin, x, 0.0, Zmax);
    }
    for(float z = Zmin; z <= Zmax; z+=step) {
        drawLine(Xmin, 0.0, z, Xmax, 0.0, z);
    }
}

void gridYZ(float Ymin, float Zmin, float Ymax, float Zmax, float step) {

    for(float y = Ymin; y <= Ymax; y+=step) {
        drawLine(0.0, y, Zmin, 0.0, y, Zmax);
    }
    for(float z = Ymin; z <= Ymax; z+=step) {
        drawLine(0.0, Ymin, z, 0.0, Ymax, z);
    }
}
