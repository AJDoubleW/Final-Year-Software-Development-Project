/*Driver Application main menu*/

#include "MainMenu.h"
#include "RegForm.h"
#include "Ve_sim.h"
#include "compReqForm.h"

MainMenu::MainMenu() {
    widget.setupUi(this);
}

MainMenu::~MainMenu() {
}

void MainMenu::on_regButton_clicked() {
    RegForm regform;
    regform.setModal(true);
    this->hide();
    regform.exec();
    
}

void MainMenu::on_startButton_clicked() {
    Ve_sim vesim;
    vesim.setModal(true);
    this->hide();
    vesim.exec();
}

void MainMenu::on_compReqButton_clicked() {
    compReqForm crf;
    crf.setModal(true);
    this->hide();
    crf.exec();
}