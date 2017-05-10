//
// Created by Ruud Andriessen on 10/05/2017.
//

#include "View.h"
#include "imageio.h"

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

View::View(int width, int height, float dt, SystemBuilder::AvailableSystems system)
        : width(width), height(height), isSimulating(false), dumpFrames(false), frameNumber(0), dt(dt) {
    glutInitDisplayMode ( GLUT_RGBA | GLUT_DOUBLE );

    glutInitWindowPosition ( 0, 0 );
    glutInitWindowSize ( width, height );
    id = glutCreateWindow ( "Particles!" );

    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
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
    printf("SYSTEM IS GO");
    sys = SystemBuilder::get(type);
    printf("%i", sys->particles.size());
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

        case ' ':
            isSimulating = !isSimulating;
            if(isSimulating)
                sys->reset ();
            break;
    }
}

void View::onMouseEvent( int button, int state, int x, int y )
{
//    omx = mx = x;
//    omx = my = y;
//
//    if(!mouse_down[0]){hmx=x; hmy=y;}
//    if(mouse_down[button]) mouse_release[button] = state == GLUT_UP;
//    if(mouse_down[button]) mouse_shiftclick[button] = glutGetModifiers()==GLUT_ACTIVE_SHIFT;
//    mouse_down[button] = state == GLUT_DOWN;
}

void View::onMotionEvent( int x, int y )
{
    this->mx = x;
    this->my = y;
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

    gluPerspective(45.0f, aspect, 0.1f, 100.0f);

    this->width = width;
    this->height = height;
}

void View::onIdle()
{
    if ( isSimulating ) sys->step(dt);
//    else        {get_from_UI();remap_GUI();}

    glutSetWindow ( id );
    glutPostRedisplay ();
}

void View::onDisplay()
{
    preDisplay3D ();

    if (sys != NULL)
        sys->draw();

    postDisplay ();
}


void View::preDisplay3D()
{
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode ( GL_MODELVIEW );
    glLoadIdentity ();
    glTranslatef(0.0f, 0.0f, -4.0f);
    glRotatef(20, 1.0f, 0.0f, 0.0f);
//    glRotatef(camAngle, 0.0f, 1.0f, 0.0f);
//    camAngle+=0.5f;
}

void View::postDisplay()
{
    // Write frames if necessary.
    if (dumpFrames) {
        const int FRAME_INTERVAL = 4;
        if ((frameNumber % FRAME_INTERVAL) == 0) {
            const unsigned int w = glutGet(GLUT_WINDOW_WIDTH);
            const unsigned int h = glutGet(GLUT_WINDOW_HEIGHT);
            unsigned char * buffer = (unsigned char *) malloc(w * h * 4 * sizeof(unsigned char));
            if (!buffer)
                exit(-1);
            // glRasterPos2i(0, 0);
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
