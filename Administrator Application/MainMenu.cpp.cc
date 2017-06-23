/*Administrator Application Main Menu Form*/

#include "MainMenu.h"
#include "VeManSys.h"
#include "dispDri.h"
#include "dispPol.h"
#include "alertLog.h"

MainMenu::MainMenu() {
    widget.setupUi(this);
}

MainMenu::~MainMenu() {
}

void MainMenu::on_vmsButton_clicked() {
    VeManSys vms;
    vms.setModal(true);
    this->hide();
    vms.exec();
}

void MainMenu::on_dmsButton_clicked() {
    dispDri dms;
    dms.setModal(true);
    this->hide();
    dms.exec();
}

void MainMenu::on_imsButton_clicked() {
    dispPol ims;
    ims.setModal(true);
    this->hide();
    ims.exec();
}

void MainMenu::on_alertButton_clicked() {
    alertLog alog;
    alog.setModal(true);
    this->hide();
    alog.exec();
}