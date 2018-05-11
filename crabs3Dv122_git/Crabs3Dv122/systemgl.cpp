#include "systemgl.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <math.h>

#include "geomshapes.h"
#include "templates.h"

SystemGL::SystemGL()
{
    resetTransform();

    gridXY_is_On = false;
    gridXZ_is_On = true;
    gridYZ_is_On = false;

    view = PILOT_VIEW;

    isOpen_c3d = false;
    C3DframeNum = 0;
    frameStep = 1;

    playOn = false;

    accumThreshold = 1.0;
    accumTolerance = 70.0;
    accumRunState = false;

    nearestThreshold = 0.5;
    nearestTolerance = 3;
    nearestRunState = false;

    unitScale = 1.0; //meters

    fromFrame = 0;
    toFrame = 0;
    seperator = ',';
    maxFrameValue = 0;

    frameForKmeans = 0;
    clusterSizeForKmeans = 1;
}

void SystemGL::init(void) {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
    glLineWidth(0.5);
    glPointSize(2.0);
}

void SystemGL::display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    drawView();

    checkRunStates();

    glColor3f(1.0, 1.0, 0.0);
    drawGrids();
    Draw_C3D();

    glFlush();
}

void SystemGL::reshape(int w, int h) {
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(35, (GLdouble)w/h, 0.001, 950.0);
    glMatrixMode(GL_MODELVIEW);
}

/*~~~~~~~~~*/
/* Dialogs */
/*~~~~~~~~~*/

void SystemGL::RemoveAccumulatedNoise(AccumulationNoiseRemover &accNsRmv) {
    accNsRmv.SetValues(&accumThreshold, &accumTolerance, &accumRunState);
}

void SystemGL::RemoveNearestNeighborMethod(NearestNeighborRemover &nearNeigh) {
    nearNeigh.SetValues(&nearestThreshold, &nearestTolerance, &nearestRunState);
}

/*~~~~~~~*/
/* Units */
/*~~~~~~~*/
void SystemGL::SetUnits(UnitsDialog &units) {
    units.SetUnit(&unitScale);
}

/*~~~~~~~~~~~~*/
/* PrintCloud */
/*~~~~~~~~~~~~*/
void SystemGL::SetValuesForCloudPrint(ExportCloudCSV &cloudCSV) {
    if(toFrame > maxFrameValue)
        toFrame = maxFrameValue;
    cloudCSV.SetValue(&fromFrame, &toFrame, &seperator);
}

void SystemGL::FixCloudLinear(void) {
    if(isOpen_c3d) {
        c3d_f.FixCloudLinear(c3d_f);
    }
}

void SystemGL::FixCloudLinearWithWeights(void) {
    if(isOpen_c3d) {
        c3d_f.FixCloudLinearWithWeights(c3d_f);
    }
}

//-------------------------------------------------------------------------------//

/***********/
/* Private */
/***********/


void SystemGL::pilotView(void) {
    glRotatef(rotZ, 0.0, 0.0, 1.0);
    glRotatef(rotY, 0.0, 1.0, 0.0);
    glRotatef(rotX, 1.0, 0.0, 0.0);
    glTranslatef(coordX, coordY, coordZ);
}

void SystemGL::polarView(void) {
    glTranslatef(0.0, 0.0, coordZ);
    glRotatef(-rotZ, 0.0, 0.0, 1.0);
    glRotatef(-rotX, 1.0, 0.0, 0.0);
    glRotatef(rotY, 0.0, 1.0, 0.0);
}

void SystemGL::drawGrids(void) {

    float step = 5.0;
    float gridMinMax = 1000.0;

    float centerX = coordX - fmod(coordX, step);
    float centerY = coordY - fmod(coordY, step);
    float centerZ = coordZ - fmod(coordZ, step);

    if(gridXY_is_On) {
        glColor3f(0.7, 0.7, 0.0);
        gridXY(-centerX-gridMinMax, -centerY-gridMinMax, -centerX+gridMinMax, -centerY+gridMinMax, step);
    }
    if(gridXZ_is_On) {
        glColor3f(0.7, 0.0, 0.7);
        gridXZ(-centerX-gridMinMax, -centerZ-gridMinMax, -centerX+gridMinMax, -centerZ+gridMinMax, step);
    }
    if(gridYZ_is_On) {
        glColor3f(0.0, 0.7, 0.7);
        gridYZ(-centerY-gridMinMax, -centerZ-gridMinMax, -centerY+gridMinMax, -centerZ+gridMinMax, step);
    }
}

void SystemGL::drawView(void) {
    switch (view) {
    case PILOT_VIEW:
        pilotView();
        break;
    case POLAR_VIEW:
        polarView();
        break;
    default:
        break;
    }
}

void SystemGL::resetTransform(void) {
    rotX = 0.0;
    rotY = 0.0;
    rotZ = 0.0;

    coordX = 0.0;
    coordY = 0.0;
    coordZ = -10.0;
}

bool SystemGL::Open_C3D(std::string path) {
    if(c3d_f.Read_C3D_File(path, c3d_f))
        isOpen_c3d = true;
    *pointsOnScreen = c3d_f.VisiblePoints(c3d_f, 0);
    fromFrame = 0;
    maxFrameValue = toFrame = c3d_f.Cloud().FrameSize();
    return isOpen_c3d;
}

void SystemGL::Draw_C3D(void) {
    if(isOpen_c3d) {
        C3DframeNum = *currFrame;
        *pointsOnScreen = c3d_f.VisiblePoints(c3d_f, C3DframeNum);
        for(int point_index = 0; point_index != c3d_f.Data().PointSize(); ++point_index) {
            for(int cluster_index = 0; cluster_index != c3d_f.Cloud().ClusterSize(); ++cluster_index) {
                glPointSize(c3d_f.Cloud().Cluster_(cluster_index).PointSize_());
                if(c3d_f.Cloud().Frame_(C3DframeNum).Point_(point_index).Visible())
                    if(c3d_f.Cloud().Frame_(C3DframeNum).Point_(point_index).C_ID() == c3d_f.Cloud().Cluster_(cluster_index).ID_()
                            && c3d_f.Cloud().Cluster_(cluster_index).Visible()) {
                        glBegin(GL_POINTS);
                        glColor3f(c3d_f.Cloud().Cluster_(cluster_index).RGB_().R()/255.0,
                                  c3d_f.Cloud().Cluster_(cluster_index).RGB_().G()/255.0,
                                  c3d_f.Cloud().Cluster_(cluster_index).RGB_().B()/255.0);

                        glVertex3f(c3d_f.Cloud().Frame_(C3DframeNum).Point_(point_index).X()*unitScale,
                                   c3d_f.Cloud().Frame_(C3DframeNum).Point_(point_index).Y()*unitScale,
                                   c3d_f.Cloud().Frame_(C3DframeNum).Point_(point_index).Z()*unitScale);
                        glEnd();
                    }
            }
        }

        if(playOn) {
            C3DframeNum+=frameStep;
            if(C3DframeNum >= c3d_f.Data().FrameSize())
                C3DframeNum = 0;
        }
        *currFrame = C3DframeNum;
    }
}

void SystemGL::checkRunStates(void) {
    if(isOpen_c3d) {
        if(accumRunState) {
            c3d_f.AccumulatedNoiseRemoval(c3d_f, accumThreshold, accumTolerance);
            *pointsOnScreen = c3d_f.VisiblePoints(c3d_f, 0);
            accumRunState = false;
        } else if(nearestRunState) {
            c3d_f.NearestNeighborRemoval(c3d_f, nearestThreshold, nearestTolerance);
            *pointsOnScreen = c3d_f.VisiblePoints(c3d_f, 0);
            nearestRunState = false;
        }
    } else {
        accumRunState = false;
        nearestRunState = false;
    }
}
