//
// Created by Ruud Andriessen on 03/05/2017.
//

#include "Force.h"

void Force::setActive(bool state) {
    active = state;
}
void Force::toggle() {
    active = !active;
}