/*Administrator Application Vehicle Management System Menu Form*/

#include "VeManSys.h"
#include "addVec.h"
#include "dispVec.h"
#include "loaGetSet.h"
#include "MainMenu.h"
#include <qt4/QtGui/qmessagebox.h>

VeManSys::VeManSys() {
    widget.setupUi(this);
}

VeManSys::~VeManSys() {
}

void VeManSys::on_addButton_clicked() {
    if (getLoa() != 2) {
        QMessageBox::critical(this, "Not Authorised", "You are not authorised to perform this action. You must be a level 2 administrator to add a new vehicle.");
        return;
    }
    addVec addv;
    addv.setModal(true);
    this->hide();
    addv.exec();
}

void VeManSys::on_modButton_clicked() {
    dispVec disp;
    disp.setModal(true);
    this->hide();
    disp.exec();
}

void VeManSys::on_homeButton_clicked() {
    MainMenu mainmenu;
    mainmenu.setModal(true);
    this->hide();
    mainmenu.exec();
}