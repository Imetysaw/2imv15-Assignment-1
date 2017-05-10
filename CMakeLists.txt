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


include_directories(lib/nanogui/include)
include_directories(${NANOGUI_EXTRA_INCS})
add_definitions(${NANOGUI_EXTRA_DEFS})

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
        constraints/CircularWireConstraint.cpp
        constraints/CircularWireConstraint.h
        imageio.cpp
        imageio.h
        linearSolver.cpp
        linearSolver.h
        Particle.cpp
        Particle.h
        ParticleToy.cpp
        constraints/RodConstraint.cpp
        constraints/RodConstraint.h
        forces/SpringForce.cpp
        forces/SpringForce.h forces/DirectionalForce.cpp forces/DirectionalForce.h forces/Force.cpp forces/Force.h System.cpp System.h solvers/Euler.cpp solvers/Euler.h solvers/Solver.h constraints/Constraint.h solvers/ConstraintSolver.cpp solvers/ConstraintSolver.h forces/DragForce.cpp forces/DragForce.h solvers/Solver.cpp View.cpp View.h SystemBuilder.cpp SystemBuilder.h)


add_executable(ParticleToy ${SOURCE_FILES})