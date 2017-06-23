/*File saving the level of access of the admin*/

#include "loaGetSet.h"

int levacc;

int getLoa() {
    return levacc;
}

void setLoa(int loa) {
    levacc = loa;
}