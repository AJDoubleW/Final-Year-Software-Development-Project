/*Administrator application register/login form*/

#include "RegLog.h"
#include "regForm.h"
#include "Login.h"

RegLog::RegLog() {
    widget.setupUi(this);
}

RegLog::~RegLog() {
}

void RegLog::on_regButton_clicked() {
    regForm regf;
    regf.setModal(true);
    this->hide();
    regf.exec();
}

void RegLog::on_logButton_clicked() {
    Login log;
    log.setModal(true);
    this->hide();
    log.exec();
}
