/*Insurer Login Form header*/

#ifndef _LOGIN_H
#define _LOGIN_H

#include "ui_Login.h"

class Login : public QDialog {
    Q_OBJECT
public:
    Login();
    virtual ~Login();
    void chkLogin();
    bool whiteString(char []);
    void chk_att();
    void display_inc_cred();
private:
    Ui::Login widget;
    
private slots:
    void on_logButton_clicked();
    void on_regButton_clicked();
};

#endif /* _LOGIN_H */
