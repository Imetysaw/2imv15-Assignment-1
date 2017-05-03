cmake_minimum_required(VERSION 3.6)
project(ParticleToy)

set(CMAKE_CXX_STANDARD 11)

FIND_PACKAGE(PNG REQUIRED)
INCLUDE_DIRECTORIES(${PNG_INCLUDE_DIRS})
LINK_LIBRARIES(${PNG_LIBRARIES})

FIND_PACKAGE(GLUT REQUIRED)
INCLUDE_DIRECTORIES(${GLUT_INCLUDE_DIRS})
LINK_LIBRARIES(${GLUT_LIBRARIES})

FIND_PACKAGE(OpenGl REQUIRED)
INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIRS})
LINK_LIBRARIES(${OPENGL_LIBRARIES})


INCLUDE_DIRECTORIES(include)

set(SOURCE_FILES
        include/gfx/config.h
        include/gfx/config-osx.h
        include/gfx/geom3d.h
        include/gfx/geom4d.h
        include/gfx/gfx.h
        include/gfx/mat2.h
        include/gfx/mat3.h
        include/gfx/mat4.h
        include/gfx/quat.h
        include/gfx/symmat2.h
        include/gfx/symmat3.h
        include/gfx/symmat4.h
        include/gfx/vec2.h
        include/gfx/vec3.h
        include/gfx/vec4.h
        CircularWireConstraint.cpp
        CircularWireConstraint.h
        imageio.cpp
        imageio.h
        linearSolver.cpp
        linearSolver.h
        Particle.cpp
        Particle.h
        ParticleToy.cpp
        RodConstraint.cpp
        RodConstraint.h
        Solver.cpp
        SpringForce.cpp
        SpringForce.h)


add_executable(ParticleToy ${SOURCE_FILES})