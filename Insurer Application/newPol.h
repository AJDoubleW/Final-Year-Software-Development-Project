/*Insurance app new policy form header*/

#ifndef _NEWPOL_H
#define _NEWPOL_H

#include "ui_newPol.h"

class newPol : public QDialog {
    Q_OBJECT
public:
    newPol();
    virtual ~newPol();
    bool validDate(int, int, int);
    bool validLength(int, int);
    void create_new_pol();
    bool display_ex_pol();
    bool calculate_length(int, int, int);
private:
    Ui::newPol widget;
private slots:
     void on_homeButton_clicked();
     void on_submitButton_clicked();
};

#endif /* _NEWPOL_H */
