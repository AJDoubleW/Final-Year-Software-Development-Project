/*Administrator Application Modify Vehicle Form Header*/

#ifndef _MODVEC_H
#define _MODVEC_H

#include "ui_modVec.h"

class modVec : public QDialog {
    Q_OBJECT
public:
    modVec();
    virtual ~modVec();
    void vec_tbe(int);
private:
    Ui::modVec widget;
private slots:
    void on_modButton_clicked();
    void on_homeButton_clicked();
};

#endif /* _MODVEC_H */
