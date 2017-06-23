/*Insurer Login Menu Form*/

#include "RegLog.h"
#include "regForm.h"
#include "Login.h"

RegLog::RegLog() {
    widget.setupUi(this);
}

RegLog::~RegLog() {
}

void RegLog::on_regButton_clicked() {
    regForm regform;
    regform.setModal(true);
    this->hide();
    regform.exec();
}

void RegLog::on_logButton_clicked() {
    Login login;
    login.setModal(true);
    this->hide();
    login.exec();
}