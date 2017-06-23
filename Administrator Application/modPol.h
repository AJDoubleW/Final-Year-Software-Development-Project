/*Administrator Application MOdify Policy Form Header*/

#ifndef _MODPOL_H
#define _MODPOL_H

#include "ui_modPol.h"

class modPol : public QDialog {
    Q_OBJECT
public:
    modPol();
    virtual ~modPol();
    void pol_tbe(char [], int, char [], char [], char []);
    bool validDates(int, int, int, int, int, int);
    bool calculate_length(int, int, int);
    bool display_ex_pol();
private:
    Ui::modPol widget;
    
private slots:
    void on_modButton_clicked();
    void on_homeButton_clicked();
};

#endif /* _MODPOL_H */
