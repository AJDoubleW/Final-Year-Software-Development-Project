/*Administrator application register/login form header*/

#ifndef _REGLOG_H
#define _REGLOG_H

#include "ui_RegLog.h"

class RegLog : public QDialog {
    Q_OBJECT
public:
    RegLog();
    virtual ~RegLog();
private:
    Ui::RegLog widget;
    
private slots:
    void on_regButton_clicked();
    void on_logButton_clicked();
};

#endif /* _REGLOG_H */
