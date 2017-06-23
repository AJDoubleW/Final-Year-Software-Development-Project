/*Administrator application registration form header*/

#ifndef _REGFORM_H
#define _REGFORM_H

#include "ui_regForm.h"

class regForm : public QDialog {
    Q_OBJECT
public:
    regForm();
    virtual ~regForm();
    bool len_chk(char []);
    bool pass_stren(char []);
    void regAdmin();
private:
    Ui::regForm widget;
    
private slots:
    void on_regButton_clicked();
};

#endif /* _REGFORM_H */
