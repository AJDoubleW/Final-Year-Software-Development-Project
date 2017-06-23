/*Administrator Application Vehicle Management System Menu Form Header*/

#ifndef _VEMANSYS_H
#define _VEMANSYS_H

#include "ui_VeManSys.h"

class VeManSys : public QDialog {
    Q_OBJECT
public:
    VeManSys();
    virtual ~VeManSys();
private:
    Ui::VeManSys widget;
    
private slots:
    void on_addButton_clicked();
    void on_modButton_clicked();
    void on_homeButton_clicked();
};

#endif /* _VEMANSYS_H */
