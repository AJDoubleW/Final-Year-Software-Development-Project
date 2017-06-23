/*Administrator Application Registration Authentication form header*/

#ifndef _REGAUTH_H
#define _REGAUTH_H

#include "ui_regAuth.h"

class regAuth : public QDialog {
    Q_OBJECT
public:
    regAuth();
    virtual ~regAuth();
    bool isauth;
    void chkAuth();
    void chk_auth_att();
private:
    Ui::regAuth widget;
private slots:
    void on_regButton_clicked();
    
};

#endif /* _REGAUTH_H */
