/*Administrator Application Main Menu*/

#ifndef _MAINMENU_H
#define _MAINMENU_H

#include "ui_MainMenu.h"

class MainMenu : public QDialog {
    Q_OBJECT
public:
    MainMenu();
    virtual ~MainMenu();
private:
    Ui::MainMenu widget;
    
private slots:
    void on_vmsButton_clicked();
    void on_dmsButton_clicked();
    void on_imsButton_clicked();
    void on_alertButton_clicked();
};

#endif /* _MAINMENU_H */
