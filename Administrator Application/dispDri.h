/*Administrator Application Display Drivers Form Header*/

#ifndef _DISPDRI_H
#define _DISPDRI_H

#include "ui_dispDri.h"

class dispDri : public QDialog {
    Q_OBJECT
public:
    dispDri();
    virtual ~dispDri();
    void fillTable();
    
private:
    Ui::dispDri widget;
    
private slots:
    void on_modButton_clicked();
    void on_tableWidget_cellClicked();
    void on_delButton_clicked();
    void on_homeButton_clicked();
};

#endif /* _DISPDRI_H */
