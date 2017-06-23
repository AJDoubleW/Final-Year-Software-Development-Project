/*Add New Vehicle Form Header*/

#ifndef _ADDVEC_H
#define _ADDVEC_H

#include "ui_addVec.h"

class addVec : public QDialog {
    Q_OBJECT
public:
    addVec();
    virtual ~addVec();
    void chk_addvec();
    bool mot_not_needed();
private:
    Ui::addVec widget;
    
private slots:
    void on_addButton_clicked();
    void on_homeButton_clicked();
};

#endif /* _ADDVEC_H */
