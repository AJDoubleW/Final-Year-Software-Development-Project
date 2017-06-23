/*Application launcher header*/

#ifndef _APPLAUNCHER_H
#define _APPLAUNCHER_H

#include "ui_Applauncher.h"

class Applauncher : public QDialog {
    Q_OBJECT
public:
    Applauncher();
    virtual ~Applauncher();
private:
    Ui::Applauncher widget;
    
private slots:
    void on_driLaunch_clicked();
    void on_insLaunch_clicked();
    void on_adminLaunch_clicked();
};

#endif /* _APPLAUNCHER_H */
