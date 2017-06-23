/*Insurance main menu header*/

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
    void on_polButton_clicked();
    void on_reqButton_clicked();
        
};

#endif /* _MAINMENU_H */
