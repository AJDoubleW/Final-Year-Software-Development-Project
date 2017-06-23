/*Administrator Application Insurance Management System Form Header*/

#ifndef _INSMANSYS_H
#define _INSMANSYS_H

#include "ui_InsManSys.h"

class InsManSys : public QDialog {
    Q_OBJECT
public:
    InsManSys();
    virtual ~InsManSys();
private:
    Ui::InsManSys widget;
    
private slots:
    void on_polButton_clicked();
    void on_usrButton_clicked();
    void on_homeButton_clicked();
};

#endif /* _INSMANSYS_H */
