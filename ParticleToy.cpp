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
		dt = 0.4f;
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

	View v(512, 512, dt, SystemBuilder::CLOTH, N);

	exit ( 0 );
}

