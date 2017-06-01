//
// Created by Ruud Andriessen on 03/05/2017.
//

#include "Force.h"

void Force::setActive(bool state) {
    active = state;
}
bool Force::toggle() {
    active = !active;
    return active;
}