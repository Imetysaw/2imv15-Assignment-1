// ParticleToy.cpp : Defines the entry point for the console application.
//

#include "View.h"

#if defined(__CYGWIN__) || defined(WIN32)
	#include <GL/glut.h>
#else
	#include <GLUT/glut.h>
#endif


/*
----------------------------------------------------------------------
main --- main routine
----------------------------------------------------------------------
*/

int main ( int argc, char ** argv )
{
	glutInit ( &argc, argv );
    float dt;
    int N;
    float d;
	if ( argc == 1 ) {
		N = 64;
		dt = 0.002f;
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
	printf ( "\t Quit by pressing the 'q' key\n" );
	printf ( "\t Decrease/increase time step by pressing the '-' and '=' keys\n" );
    printf ( "\t Show detailed info by pressing the 'p' key\n" );
    printf ( "\t Toggle global directional force using the  '[' key\n" );
	printf ( "\t Toggle the wall using the  'w' key\n" );
	printf ( "\t Toggle adaptive with the 'a' key\n" );
	printf ( "\t Toggle visibility of forces with the 'f' key\n" );
	printf ( "\t Toggle visibility of constraints with the 'o' key\n" );
	printf ( "\t Click on a particle and drag the mouse to apply force\n" );
	printf ( "\t Use ',' '.' and '/' to rotate\n" );
	printf ( "\t Use '1' to '5' to change between solvers:\n" );
	printf ( "\t 1: Explicit Euler\n" );
	printf ( "\t 2: Semi explicit Euler\n" );
	printf ( "\t 3: Implicit Euler\n" );
	printf ( "\t 4: Midpoint\n" );
	printf ( "\t 5: 4th order Runge-Kutta\n" );

	View v(768, 512, dt, SystemBuilder::CLOTH, N);

	exit ( 0 );
}

