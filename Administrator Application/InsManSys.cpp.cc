/*Administrator Application Insurance Management Form*/

#include "InsManSys.h"
#include "dispPol.h"
#include "dispUsr.h"
#include "MainMenu.h"

InsManSys::InsManSys() {
    widget.setupUi(this);
}

InsManSys::~InsManSys() {
}

void InsManSys::on_polButton_clicked() {
    dispPol dpol;
    dpol.setModal(true);
    this->hide();
    dpol.exec();
}

void InsManSys::on_usrButton_clicked() {
    dispUsr dusr;
    dusr.setModal(true);
    this->hide();
    dusr.exec();
}

void InsManSys::on_homeButton_clicked() {
    MainMenu mainmenu;
    mainmenu.setModal(true);
    this->hide();
    mainmenu.exec();
}