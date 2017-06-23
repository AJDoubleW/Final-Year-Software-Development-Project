/*Administrator Application Display Users Form Header*/

#ifndef _DISPUSR_H
#define _DISPUSR_H

#include "ui_dispUsr.h"

class dispUsr : public QDialog {
    Q_OBJECT
public:
    dispUsr();
    virtual ~dispUsr();
    void fillTable();
private:
    Ui::dispUsr widget;
    
private slots:
    void on_delButton_clicked();
    void on_modButton_clicked();
    void on_tableWidget_cellClicked();
    void on_homeButton_clicked();
};

#endif /* _DISPUSR_H */
