/*Backup Password Form*/

/*Project file headers*/
#include "Ve_sim.h"
#include "backPassForm.h"
#include "RegForm.h"
#include "encrypt.hpp"

/*C standard headers*/
#include <stdio.h> //allows commands such as printf to be used
#include <iostream> //allows cin
#include <string> //allows strings
#include <ctype.h> //checks datatype of characters - whitelist
#include <cstring> //allows convert to cstring
#include <cstdlib> //allkinds - system/exit etc...
#include <sstream> //for creation of sql query string

/*MySQL headers*/
#include "mysql_connection.h"
#include "mysql_driver.h"
#include "examples/examples.h"
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

/*QT headers*/
#include <qt4/QtGui/qmessagebox.h>
#include <valarray>

using namespace std;

string attalias;
string attpass;
int passattempts = 0;

backPassForm::backPassForm() {
    widget.setupUi(this);
    
}

backPassForm::~backPassForm() {
}


void backPassForm::on_submitButton_clicked() {
    char *str_tbt;
    RegForm regf;
    Ve_sim vs;
    Encrypt enc;
    
    attalias = widget.aliasEdit->text().toStdString();
    attpass = widget.passEdit->text().toStdString();
    
    str_tbt = new char[attalias.length() + 1];
    strcpy(str_tbt, attalias.c_str());
    
    if(!(regf.whiteString(str_tbt))) {
        passattempts++;
        chk_att();
        QMessageBox::warning(this, "Invalid Alias", QString("The Alias you entered is invalid. Alias are alphanumeric only.\nYou have %1 attempts remaining.").arg(3-passattempts));
        return;
    }
    
    str_tbt = new char[attpass.length() + 1];
    strcpy(str_tbt, attpass.c_str());
    
    if(!(regf.whiteString(str_tbt))) {
        passattempts++;
        chk_att();
        QMessageBox::warning(this, "Invalid Password", QString("The Password you entered is invalid. Passwords are alphanumeric only.\nYou have %1 attempts remaining.").arg(3-passattempts));
        return;
    }
    
    try {
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
        sql::ResultSet *res;
 
        /* Create a connection */
        driver = sql::mysql::get_driver_instance();
        con = driver->connect("localhost", "root", "vehiclesecure");

        /* Connect to the MySQL database */
        stmt = con->createStatement();
        stmt->execute("USE FinalYearProject");
        
        char *alias;
        alias = new char[attalias.length() + 1];
        strcpy(alias, attalias.c_str());
        char *pass;
        pass = new char[attpass.length() + 1];
        strcpy(pass, attpass.c_str());
        
        string encpass = enc.enctext(pass, alias);
        
        res = stmt->executeQuery("SELECT DriverID, Alias, Backup_Password FROM Driver");
        bool chk_usr = false;
        bool chk_pass = false;
        while (res->next()) {
            if (res->getString("Alias") == attalias) {
                chk_usr = true;
                if (res->getString("Backup_Password") == encpass) {
                    chk_pass = true;
                    vs.setDriverID(res->getInt("DriverID"));
                    validbp = true;
                    this->hide();
                    return;
                    
                }
            }
            
        }
        
        if (!chk_usr) {
            passattempts++;
            chk_att();
            QMessageBox::warning(this, "Incorrect Alias", QString("That alias does not belong to a registered driver.\nYou have %1 attempts remaining.").arg(3-passattempts));
            return;
        }
        
        if (!chk_pass) {
            passattempts++;
            chk_att();
            QMessageBox::warning(this, "Incorrect Password", QString("That backup password is incorrect.\nYou have %1 attempts remaining.").arg(3-passattempts));
            return;
        }
        
           /* output any SQL error*/
    } catch (sql::SQLException &e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }
    
}

void backPassForm::chk_att() {
    if (passattempts == 3) {
        validbp = false;
        this->hide();
        return;
    }
}
