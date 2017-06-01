//
// Created by Ruud Andriessen on 10/05/2017.
//

#include "View.h"
#include "imageio.h"
#include "forces/DirectionalForce.h"
#include "solvers/RungeKutta.h"
#include "solvers/Euler.h"
#include "solvers/Midpoint.h"

#if defined(__CYGWIN__) || defined(WIN32)
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

// Callback handles due to glut being ugly
static View* currentInstance = NULL;

extern "C"
void displayCallback() {
    currentInstance->onDisplay();
}
void idleCallback() {
    currentInstance->onIdle();
}
void motionCallback(int x, int y) {
    currentInstance->onMotionEvent(x, y);
}
void reshapeCallback(int w, int h) {
    currentInstance->onReshape(w, h);
}
void mouseCallback(int b, int s, int x, int y) {
    currentInstance->onMouseEvent(b, s, x, y);
}
void keypressCallback(unsigned char k, int x, int y) {
    currentInstance->onKeyPress(k, x, y);
}

View::View(int width, int height, float dt, SystemBuilder::AvailableSystems system, int N)
        : width(width), height(height), isSimulating(false), dumpFrames(false), drawVelocity(false), drawForces(true),
          drawConstraints(true), adaptive(false), frameNumber(0), dt(dt), N(N) {
    glutInitDisplayMode ( GLUT_RGBA | GLUT_DOUBLE );

    glutInitWindowPosition ( 0, 0 );
    glutInitWindowSize ( width, height );
    id = glutCreateWindow ( "Particles!" );

    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CW);
    glEnable(GL_CULL_FACE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHT0);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDepthFunc(GL_LEQUAL);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    currentInstance = this;

    glutKeyboardFunc ( keypressCallback );
    glutMouseFunc ( mouseCallback );
    glutMotionFunc ( motionCallback );
    glutReshapeFunc ( reshapeCallback );
    glutIdleFunc ( idleCallback );
    glutDisplayFunc ( displayCallback );

    initialize(system);

    glutMainLoop ();
}

void View::initialize(SystemBuilder::AvailableSystems type) {
    sys = SystemBuilder::get(type);
}

void View::onKeyPress ( unsigned char key, int x, int y )
{
    switch ( key )
    {
        case 'd':
        case 'D':
            dumpFrames = !dumpFrames;
            break;
        case 'q':
        case 'Q':
            sys->free ();
            exit ( 0 );
            break;
        case 'h': {
            sys = SystemBuilder::get(SystemBuilder::HAIR);
            sys->type = SystemBuilder::HAIR;
            break;
        }
        case 'c': {
            sys = SystemBuilder::get(SystemBuilder::CLOTH);
            sys->type = SystemBuilder::CLOTH;
            break;
        }
        case '1':
            printf("Using Explicit Euler\n");
            sys->solver = new Euler(Euler::EXPLICIT);
            break;
        case '2':
            printf("Using Semi Explicit Euler\n");
            sys->solver = new Euler(Euler::SEMI);
            break;
        case '3':
            printf("Using Implicit Euler\n");
            sys->solver = new Euler(Euler::IMPLICIT);
            break;
        case '4':
            printf("Using Midpoint\n");
            sys->solver = new Midpoint();
            break;
        case '5':
            printf("Using 4th order Runge-Kutta\n");
            sys->solver = new RungeKutta();
            break;
        case 'w':
        case 'W':
            sys->wallExists = !sys->wallExists;
            break;
        case '=':
            sys->dt += 0.001f;
            printf("Increase dt: %f\n", sys->dt);
            break;
        case '-':
            sys->dt -= 0.001f;
            printf("Decrease dt: %f\n", sys->dt);
            break;
        case 'v':
            drawVelocity = !drawVelocity;
            break;
        case 'o':
            drawConstraints = !drawConstraints;
            break;
        case 'f':
            drawForces = !drawForces;
            break;
        case ',':
            rotate = 1;
            break;
        case '.':
            rotate = -1;
            break;
        case '/':
            rotate = 0;
            break;
        case 'a':
            adaptive = !adaptive;
            if (adaptive)
                printf("Adaptive on\n");
            else
                printf("Adaptive off\n");
            break;
        case 'b':
            sys->springsCanBreak = !sys->springsCanBreak;
            if (sys->springsCanBreak)
                printf("Springs can now break\n");
            else
                printf("Springs can no longer break\n");
            break;
            break;
        case '[':
            printf("wind pressed\n");
            if(windForce) {
                printf("wind already created\n");
                bool active = windForce->toggle();
                active ? printf("wind active\n") : printf("wind not active\n");
            } else {
                printf("wind created\n");
                windForce = new DirectionalForce(this->sys->particles, Vec3f(5.f, 3.f, 5.f));
                sys->addForce(windForce);
            }
            break;
        case ' ':
            isSimulating = !isSimulating;
            if(isSimulating)
                sys->reset ();
            break;
    }
}

void View::onMouseEvent( int button, int state, int x, int y )
{
    initialMx = omx = mx = x;
    initialMy = omx = my = y;

    if(!mouse_down[0]){hmx=x; hmy=y;}
    if(mouse_down[button]) mouse_release[button] = state == GLUT_UP;
    if(mouse_down[button]) mouse_shiftclick[button] = glutGetModifiers()==GLUT_ACTIVE_SHIFT;
    mouse_down[button] = state == GLUT_DOWN;

    //Reset force on mouse up
    if(state == GLUT_UP){
        mouseDragForce->setActive(false);
    } else {
        //get world coordinates of click point
        GLdouble modelMatrix[16];
        glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
        GLdouble projectionMatrix[16];
        glGetDoublev(GL_PROJECTION_MATRIX, projectionMatrix);
        GLint viewMatrix[4];
        glGetIntegerv(GL_VIEWPORT, viewMatrix);
        Particle *closestParticle;
        double closestDistance = 10000000;
        for (int i = 0; i < sys->particles.size(); i++) {
            Vec3f position = sys->particles[i]->position;
            double screenCoordinates[3];
            gluProject(position[0], position[1], position[2], modelMatrix, projectionMatrix, viewMatrix,
                       &screenCoordinates[0], &screenCoordinates[1], &screenCoordinates[2]);
            double distance = abs(x - screenCoordinates[0]) + abs(y - (height - screenCoordinates[1]));
            if (distance < closestDistance) {
                closestDistance = distance;
                closestParticle = sys->particles[i];
            }
        }
        //update mouseDragParticle
        mouseDragParticle = closestParticle;
        //update the current mousedragforce
        mouseDragForce = new DirectionalForce({mouseDragParticle}, Vec3f(0, 0, 0.0f));
        sys->addForce(mouseDragForce);
    }
}

void View::onMotionEvent( int x, int y )
{
    this->mx = x;
    this->my = y;
    GLdouble modelMatrix[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
    GLdouble projectionMatrix[16];
    glGetDoublev(GL_PROJECTION_MATRIX, projectionMatrix);
    GLint viewMatrix[4];
    glGetIntegerv(GL_VIEWPORT, viewMatrix);
    double screenCoordinates[3];
    Particle* midParticle = sys->particles[sys->particles.size()/2];
    gluProject(midParticle->position[0], midParticle->position[1], midParticle->position[2], modelMatrix, projectionMatrix, viewMatrix,
               &screenCoordinates[0], &screenCoordinates[1], &screenCoordinates[2]);
    float z = screenCoordinates[2];
    double objCoordinates[3];
    gluUnProject(x, height - y, z, modelMatrix, projectionMatrix, viewMatrix, &objCoordinates[0],
                 &objCoordinates[1], &objCoordinates[2]);
    Vec3f position = mouseDragParticle->position;
    mouseDragForce->direction = 150.0f * Vec3f((objCoordinates[0] - position[0]), (objCoordinates[1] - position[1]),
                                      (objCoordinates[2] - position[2]));
}

void View::onReshape(int width, int height )
{
    if (height == 0) height = 1;
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

//	glutSetWindow ( win_id );
//	glutReshapeWindow ( width, height );

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glTranslatef(0.0f, .5f, -1.5f);

    gluPerspective(90.0f, aspect, 0.1f, 1000.0f);

    this->width = width;
    this->height = height;
}

void View::onIdle()
{
    if ( isSimulating ) sys->step(adaptive);
    else {getFromGUI();remapGUI();}

    glutSetWindow ( id );
    glutPostRedisplay ();
}

void View::onDisplay()
{
    preDisplay3D ();

    if (sys != NULL)
        sys->draw(drawVelocity, drawForces, drawConstraints);

    postDisplay ();
}


void View::preDisplay3D()
{
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glMatrixMode ( GL_MODELVIEW );
    glLoadIdentity ();
    glTranslatef(0.0f, 0.0f, -4.0f);
    glRotatef(20, 1.0f, 0.0f, 0.0f);
    glRotatef(camAngle, 0.0f, 1.0f, 0.0f);

    if (rotate != 0) {
        camAngle += rotate * 0.5f;
    }
}

void View::postDisplay()
{
    // Write frames if necessary.
    if (dumpFrames) {
        const int FRAME_INTERVAL = 40;
        if ((frameNumber % FRAME_INTERVAL) == 0) {
            const unsigned int w = glutGet(GLUT_WINDOW_WIDTH);
            const unsigned int h = glutGet(GLUT_WINDOW_HEIGHT);
            unsigned char * buffer = (unsigned char *) malloc(w * h * 4 * sizeof(unsigned char));
            if (!buffer)
                exit(-1);
            //glRasterPos2i(0, 0);
            glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
            static char filename[80];
            sprintf(filename, "snapshots/img%.5i.png", frameNumber / FRAME_INTERVAL);
            printf("Dumped %s.\n", filename);
            saveImageRGBA(filename, buffer, w, h);

            free(buffer);
        }
    }
    frameNumber++;

    glutSwapBuffers ();
}

void View::getFromGUI() {
    int i, j;
    // int size, flag;
    int hi, hj;
    // float x, y;
    if ( !mouse_down[0] && !mouse_down[2] && !mouse_release[0]
         && !mouse_shiftclick[0] && !mouse_shiftclick[2] ) return;

    i = (int)((       mx /(float)width)*N);
    j = (int)(((height-my)/(float)height)*N);

    if ( i<1 || i>N || j<1 || j>N ) return;

    if ( mouse_down[0] ) {

    }

    if ( mouse_down[2] ) {
    }

    hi = (int)((       hmx /(float)width)*N);
    hj = (int)(((height-hmy)/(float)height)*N);

    if( mouse_release[0] ) {
    }

    omx = mx;
    omy = my;
}



void View::remapGUI()
{
	for(int i=0; i < sys->particles.size(); i++)
	{
        sys->particles[i]->position[0] = sys->particles[i]->startPos[0];
        sys->particles[i]->position[1] = sys->particles[i]->startPos[1];
        sys->particles[i]->position[2] = sys->particles[i]->startPos[2];
	}
}