/*Administrator Application Modify Driver Form Header*/

#ifndef _MODDRI_H
#define _MODDRI_H

#include "ui_modDri.h"

class modDri : public QDialog {
    Q_OBJECT
public:
    modDri();
    virtual ~modDri();
    void dri_tbe(int);
    bool test_age(int, int, int);
    bool validExp(int, int, int);
    bool validDate(int, int,int);
private:
    Ui::modDri widget;
    
    private slots:
        void on_modButton_clicked();
        void on_homeButton_clicked();
};

#endif /* _MODDRI_H */
