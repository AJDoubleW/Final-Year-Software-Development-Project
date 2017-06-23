/*Driver Registration Form header*/

#ifndef _REGFORM_H
#define _REGFORM_H

#include "ui_RegForm.h"

class RegForm : public QDialog {
    Q_OBJECT
public:
    RegForm();
    virtual ~RegForm();
    bool len_chk(char []);
    bool whiteString(char []);
    bool pass_stren(char []);
    bool test_age(int, int, int, int);
    bool validDate(int, int, int);
    bool valid_exp_date(int, int, int);
    void chk_reg();
private:
    Ui::RegForm widget;
    
private slots:
    void on_homeButton_clicked();
    void on_scanButton_clicked();
    //void saveInfo();
};

#endif /* _REGFORM_H */
