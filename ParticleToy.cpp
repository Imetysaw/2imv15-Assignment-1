// ParticleToy.cpp : Defines the entry point for the console application.
//

#include "Particle.h"
#include "System.h"

#include "forces/SpringForce.h"
#include "forces/DirectionalForce.h"

#include "solvers/Euler.h"

#include "constraints/RodConstraint.h"
#include "constraints/CircularWireConstraint.h"

#include "imageio.h"
#include "forces/DragForce.h"

#if defined(_WIN32) || defined(WIN32)
	#include <GL/glut.h>
#else
	#include <GLUT/glut.h>
#endif

/* macros */

/* global variables */

static int N;
static float dt, d;
static int dsim;
static int dump_frames;
static int frame_number;


static int win_id;
static int win_x, win_y;
static int mouse_down[3];
static int mouse_release[3];
static int mouse_shiftclick[3];
static int omx, omy, mx, my;
static int hmx, hmy;

static float camAngle = 0;

static System* sys = NULL;
static RodConstraint * delete_this_dummy_rod = NULL;
static CircularWireConstraint * delete_this_dummy_wire = NULL;


/*
----------------------------------------------------------------------
free/clear/allocate simulation data
----------------------------------------------------------------------
*/

static void init_system(void)
{
    sys = new System(new Euler());

	const float dist = 0.2f;
	const Vec3f center(0.0, 0.0, 0.0);
	const Vec3f offset(dist, 0.0, 0.0);

	// Create three particles, attach them to each other, then add a
	// circular wire constraint to the first.

	sys->addParticle(new Particle(center + offset, 1.0f));
    sys->addParticle(new Particle(center + offset + offset, 1.0f));
    sys->addParticle(new Particle(center + offset + offset + offset, 1.0f));

	// You should replace these with a vector generalized forces and one of
	// constraints...
	sys->addForce(new SpringForce(sys->particles[0], sys->particles[1], dist, 1.0, 1.0));
    sys->addForce(new DirectionalForce(sys->particles, Vec3f(0, -0.0981f, 0)));

//	sys->addConstraint(new RodConstraint(sys->particles[1], sys->particles[2], dist, {1, 2}));
	sys->addConstraint(new CircularWireConstraint(sys->particles[0], center, dist, {0}));
}

static void init_cloth(void) {
    sys = new System(new Euler());

    const int gSize = 12;

    // Initialize particles
    for (float x = -0.5f; x < 0.5f; x += 1.0f / gSize) {
        for (float y = 0.5f; y > -0.5f; y -= 1.0f / gSize) {
            sys->addParticle(new Particle(Vec3f(x, y, 0.0f), 0.5f));
        }
    }

    // Add gravity and drag to all particles
    sys->addForce(new DirectionalForce(sys->particles, Vec3f(0, -0.0098f, 0)));
    sys->addForce(new DirectionalForce(sys->particles, Vec3f(0, 0, -0.005f)));
    sys->addForce(new DragForce(sys->particles, 0.2f));

    for (int x = 0; x < gSize - 1; x++) {
        for (int y = 0; y < gSize; y++) {
            sys->addForce(new SpringForce(sys->particles[x + y * gSize],
                                          sys->particles[x + 1 + y * gSize],
                                          1.0f / gSize, 0.6, 0.5));
        }
    }

    for (int x = 0; x < gSize; x++) {
        for (int y = 0; y < gSize - 1; y++) {
            sys->addForce(new SpringForce(sys->particles[x + y * gSize],
                                          sys->particles[x + (y + 1) * gSize],
                                          1.0f / gSize, 0.6, 0.5));
        }
    }

    for (int x = 0; x < gSize - 1; x++) {
        for (int y = 0; y < gSize - 1; y++) {
            sys->addForce(new SpringForce(sys->particles[x + y * gSize],
                                          sys->particles[x + 1 + (y + 1) * gSize],
                                          sqrt(2 * pow(1.0f / gSize, 2)), 1.0, 0.5));
        }
    }



    for (int x = 0; x < gSize - 1; x++) {
        for (int y = 1; y < gSize; y++) {
            sys->addForce(new SpringForce(sys->particles[x + y * gSize],
                                          sys->particles[x + 1 + (y - 1) * gSize],
                                          sqrt(2 * pow(1.0f / gSize, 2)), 1.0, 0.5));
        }
    }

    sys->addConstraint(new CircularWireConstraint(sys->particles[0], Vec3f(-0.5f, 0.6f, 0), 0.1f, {0}));
    sys->addConstraint(new CircularWireConstraint(sys->particles[gSize*gSize - gSize], Vec3f(0.45f, 0.6f, 0), 0.1f, {gSize*gSize-gSize}));
}

/*
----------------------------------------------------------------------
OpenGL specific drawing routines
----------------------------------------------------------------------
*/

static void pre_display ( void )
{
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode ( GL_MODELVIEW );
	glLoadIdentity ();
    glTranslatef(0.0f, 0.0f, -4.0f);
    glRotatef(20, 1.0f, 0.0f, 0.0f);
    glRotatef(camAngle, 0.0f, 1.0f, 0.0f);
    camAngle+=0.5f;
}

static void post_display ( void )
{
	// Write frames if necessary.
	if (dump_frames) {
		const int FRAME_INTERVAL = 4;
		if ((frame_number % FRAME_INTERVAL) == 0) {
			const unsigned int w = glutGet(GLUT_WINDOW_WIDTH);
			const unsigned int h = glutGet(GLUT_WINDOW_HEIGHT);
			unsigned char * buffer = (unsigned char *) malloc(w * h * 4 * sizeof(unsigned char));
			if (!buffer)
				exit(-1);
			// glRasterPos2i(0, 0);
			glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
			static char filename[80];
			sprintf(filename, "snapshots/img%.5i.png", frame_number / FRAME_INTERVAL);
			printf("Dumped %s.\n", filename);
			saveImageRGBA(filename, buffer, w, h);
			
			free(buffer);
		}
	}
	frame_number++;
	
	glutSwapBuffers ();
}

/*
----------------------------------------------------------------------
relates mouse movements to particle toy construction
----------------------------------------------------------------------
*/

static void get_from_UI ()
{
	int i, j;
	// int size, flag;
	int hi, hj;
	// float x, y;
	if ( !mouse_down[0] && !mouse_down[2] && !mouse_release[0] 
	&& !mouse_shiftclick[0] && !mouse_shiftclick[2] ) return;

	i = (int)((       mx /(float)win_x)*N);
	j = (int)(((win_y-my)/(float)win_y)*N);

	if ( i<1 || i>N || j<1 || j>N ) return;

	if ( mouse_down[0] ) {

	}

	if ( mouse_down[2] ) {
	}

	hi = (int)((       hmx /(float)win_x)*N);
	hj = (int)(((win_y-hmy)/(float)win_y)*N);

	if( mouse_release[0] ) {
	}

	omx = mx;
	omy = my;
}

static void remap_GUI()
{
	for(int i=0; i<sys->particles.size(); i++)
	{
        sys->particles[i]->position[0] = sys->particles[i]->startPos[0];
        sys->particles[i]->position[1] = sys->particles[i]->startPos[1];
        sys->particles[i]->position[2] = sys->particles[i]->startPos[2];
	}
}

/*
----------------------------------------------------------------------
GLUT callback routines
----------------------------------------------------------------------
*/

static void key_func ( unsigned char key, int x, int y )
{
	switch ( key )
	{
	case 'd':
	case 'D':
		dump_frames = !dump_frames;
		break;

	case 'q':
	case 'Q':
		sys->free ();
		exit ( 0 );
		break;

	case ' ':
		dsim = !dsim;
        if(dsim)
            sys->reset ();
		break;
	}
}

static void mouse_func ( int button, int state, int x, int y )
{
	omx = mx = x;
	omx = my = y;

	if(!mouse_down[0]){hmx=x; hmy=y;}
	if(mouse_down[button]) mouse_release[button] = state == GLUT_UP;
	if(mouse_down[button]) mouse_shiftclick[button] = glutGetModifiers()==GLUT_ACTIVE_SHIFT;
	mouse_down[button] = state == GLUT_DOWN;
}

static void motion_func ( int x, int y )
{
	mx = x;
	my = y;
}

static void reshape_func ( int width, int height )
{
    if (height == 0) height = 1;
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

//	glutSetWindow ( win_id );
//	glutReshapeWindow ( width, height );

    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0f, aspect, 0.1f, 100.0f);

	win_x = width;
	win_y = height;
}

static void idle_func ( void )
{
	if ( dsim ) sys->step(dt);
	else        {get_from_UI();remap_GUI();}

	glutSetWindow ( win_id );
	glutPostRedisplay ();
}

static void display_func ( void )
{
	pre_display ();

    sys->draw();

	post_display ();
}


/*
----------------------------------------------------------------------
open_glut_window --- open a glut compatible window and set callbacks
----------------------------------------------------------------------
*/

static void open_glut_window ( void )
{
	glutInitDisplayMode ( GLUT_RGBA | GLUT_DOUBLE );

	glutInitWindowPosition ( 0, 0 );
	glutInitWindowSize ( win_x, win_y );
	win_id = glutCreateWindow ( "Particletoys!" );

	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
//	glClear ( GL_COLOR_BUFFER_BIT );
//	glutSwapBuffers ();
//	glClear ( GL_COLOR_BUFFER_BIT );
//	glutSwapBuffers ();
//
//	glEnable(GL_LINE_SMOOTH);
//	glEnable(GL_POLYGON_SMOOTH);

	pre_display ();

	glutKeyboardFunc ( key_func );
	glutMouseFunc ( mouse_func );
	glutMotionFunc ( motion_func );
	glutReshapeFunc ( reshape_func );
	glutIdleFunc ( idle_func );
	glutDisplayFunc ( display_func );
}


/*
----------------------------------------------------------------------
main --- main routine
----------------------------------------------------------------------
*/

int main ( int argc, char ** argv )
{
	glutInit ( &argc, argv );

	if ( argc == 1 ) {
		N = 64;
		dt = 0.1f;
		d = 5.f;
		fprintf ( stderr, "Using defaults : N=%d dt=%g d=%g\n",
			N, dt, d );
	} else {
		N = atoi(argv[1]);
		dt = atof(argv[2]);
		d = atof(argv[3]);
	}

	printf ( "\n\nHow to use this application:\n\n" );
	printf ( "\t Toggle construction/simulation display with the spacebar key\n" );
	printf ( "\t Dump frames by pressing the 'd' key\n" );
	printf ( "\t Quit by pressing the 'q' key\n" );

	dsim = 0;
	dump_frames = 0;
	frame_number = 0;

    init_system();
	
	win_x = 512;
	win_y = 512;
	open_glut_window ();

	glutMainLoop ();

	exit ( 0 );
}
