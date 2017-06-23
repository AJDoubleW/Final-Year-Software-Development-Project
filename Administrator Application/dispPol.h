/*Administrator Application Display Policies Form Header*/

#ifndef _DISPPOL_H
#define _DISPPOL_H

#include "ui_dispPol.h"

class dispPol : public QDialog {
    Q_OBJECT
public:
    dispPol();
    virtual ~dispPol();
    void fillTable();
private:
    Ui::dispPol widget;
    
private slots:
    void on_modButton_clicked();
    void on_tableWidget_cellClicked();
    void on_delButton_clicked();
    void on_homeButton_clicked();
};

#endif /* _DISPPOL_H */
