/*START VEHICLE HEADER*/

#ifndef _VE_SIM_H
#define _VE_SIM_H

#include "ui_Ve_sim.h"

class Ve_sim : public QDialog {
    Q_OBJECT
public:
    Ve_sim();
    virtual ~Ve_sim();
    void create_alert();
    void setDriverID(int);
    void sd_check();
    bool calculate_length(int, int, int);
    bool chk_safety();
    bool all_belts(int []);
    bool all_doors(int []);
private:
    Ui::Ve_sim widget;
    
private slots:
    void on_homeButton_clicked();
    void on_scanButton_clicked();
    void on_frontLeftDoor_clicked();
    void on_frontRightDoor_clicked();
    void on_backLeftDoor_clicked();
    void on_backRightDoor_clicked();
    void on_passenger1_clicked();
    void on_passenger2_clicked();
    void on_passenger3_clicked();
    void on_passenger4_clicked();
    void on_passenger5_clicked();
        
};

#endif /* _VE_SIM_H */
