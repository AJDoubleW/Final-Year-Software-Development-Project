/*Insurance app View Requests form header*/

#ifndef _VIEWREQ_H
#define _VIEWREQ_H

#include "ui_viewReq.h"

class viewReq : public QDialog {
    Q_OBJECT
public:
    viewReq();
    virtual ~viewReq();
    void fillTable();
private:
    Ui::viewReq widget;
    
private slots:
   void on_homeButton_clicked();
   void on_tableWidget_cellClicked();
   void on_denyButton_clicked();
   void on_appButton_clicked();
   
};

#endif /* _VIEWREQ_H */
