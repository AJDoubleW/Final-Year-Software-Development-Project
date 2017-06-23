/*Administrator Application Display Vehicles Form Header*/

#ifndef _DISPVEC_H
#define _DISPVEC_H

#include "ui_dispVec.h"

class dispVec : public QDialog {
    Q_OBJECT
public:
    dispVec();
    virtual ~dispVec();
    void fillTable();
private:
    Ui::dispVec widget;
    
private slots:
    void on_modButton_clicked();
    void on_tableWidget_cellClicked();
    void on_delButton_clicked();
    void on_homeButton_clicked();
    
};

#endif /* _DISPVEC_H */
