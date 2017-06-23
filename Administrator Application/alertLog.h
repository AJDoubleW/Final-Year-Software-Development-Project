/*Administrator Application Alert Log Form Header*/

#ifndef _ALERTLOG_H
#define _ALERTLOG_H

#include "ui_alertLog.h"

class alertLog : public QDialog {
    Q_OBJECT
public:
    alertLog();
    virtual ~alertLog();
    void fillTable();
private:
    Ui::alertLog widget;
    
private slots:
    void on_tableWidget_cellClicked();
    void on_modButton_clicked();
    void on_delButton_clicked();
    void on_homeButton_clicked();
};

#endif /* _ALERTLOG_H */
