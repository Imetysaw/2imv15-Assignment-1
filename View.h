//
// Created by Ruud Andriessen on 10/05/2017.
//

#ifndef PARTICLETOY_VIEW_H
#define PARTICLETOY_VIEW_H


#include "System.h"
#include "SystemBuilder.h"

class View {
public:
    View(int width, int height, float dt, SystemBuilder::AvailableSystems system, int N);

    ~View() { delete sys; };

    // Event handlers
    void onDisplay();
    void onIdle(void);
    void onMotionEvent(int x, int y);
    void onReshape(int width, int height);
    void onMouseEvent(int button, int state, int x, int y);
    void onKeyPress(unsigned char key, int x, int y);

private:
    System* sys = NULL;
    int id;
    int mx, my, omx, omy, hmx, hmy;
    int width, height;
    bool dumpFrames;
    bool isSimulating;
    int N;
    int frameNumber;
    float dt;
    float camAngle = 0;
    int mouse_down[3];
    int mouse_release[3];
    int mouse_shiftclick[3];

    void getFromGUI();
    void remapGUI();
    void initialize(SystemBuilder::AvailableSystems type);

    // Display utility
    void preDisplay3D(void);
    void postDisplay(void);
};

#endif //PARTICLETOY_VIEW_H
