/*Backup Password From header*/

#ifndef _BACKPASSFORM_H
#define _BACKPASSFORM_H

#include "ui_backPassForm.h"

class backPassForm : public QDialog {
    Q_OBJECT
public:
    backPassForm();
    virtual ~backPassForm();
    bool validbp;
    void chk_att();
private:
    Ui::backPassForm widget;
    
private slots:
    void on_submitButton_clicked();
};

#endif /* _BACKPASSFORM_H */
