/*Insurance Main Menu form*/

#include "MainMenu.h"
#include "newPol.h"
#include "viewReq.h"

MainMenu::MainMenu() {
    widget.setupUi(this);
}

MainMenu::~MainMenu() {
}

void MainMenu::on_polButton_clicked() {
    newPol pol;
    pol.setModal(true);
    this->hide();
    pol.exec();
}

void MainMenu::on_reqButton_clicked() {
    viewReq req;
    req.setModal(true);
    this->hide();
    req.exec();
}
