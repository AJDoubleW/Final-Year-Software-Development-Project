/*Comprehensive Permission Form header*/

#ifndef _COMPREQFORM_H
#define _COMPREQFORM_H

#include "ui_compReqForm.h"

class compReqForm : public QDialog {
    Q_OBJECT
public:
    compReqForm();
    virtual ~compReqForm();
    bool orderDate(int, int, int, int, int, int);
    void create_req();
private:
    Ui::compReqForm widget;
    
private slots:
    void on_homeButton_clicked();
    void on_submitButton_clicked();
        
};

#endif /* _COMPREQFORM_H */
