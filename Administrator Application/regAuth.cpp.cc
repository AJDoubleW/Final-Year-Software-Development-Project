/*Administrator Application Registration Authentication form*/

/*Project file headers*/
#include "regForm.h"
#include "Login.h"
#include "regAuth.h"
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
#include <QInputDialog>
#include <valarray>

using namespace std;

string l2username;
string l2basepass;
string l2l2pass;
int authattempts = 0;

regAuth::regAuth() {
    widget.setupUi(this);
    isauth = false;
}

regAuth::~regAuth() {
}

void regAuth::on_regButton_clicked() {
    /*setting text*/
    l2username = widget.userEdit->text().toStdString();
    l2basepass = widget.passEdit->text().toStdString();
    l2l2pass = widget.l2passEdit->text().toStdString();
    char *str_tbt;
    Login log;
    
    str_tbt = new char[l2username.length() + 1];
    strcpy(str_tbt, l2username.c_str());
    
    if(!(log.whiteString(str_tbt))) {
       authattempts++;
       chk_auth_att();
       QMessageBox::StandardButton cont;
       cont = QMessageBox::warning(this, "Invalid Username", "That username is invalid.You have 1 attempt left.\nTry again?",
                                    QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
       
       if (cont == QMessageBox::No) 
           exit(0);
       return;
    }
    
    str_tbt = new char[l2basepass.length() + 1];
    strcpy(str_tbt, l2basepass.c_str());
    
    if(!(log.whiteString(str_tbt))) {
       authattempts++;
       chk_auth_att();
       QMessageBox::StandardButton cont;
       cont = QMessageBox::warning(this, "Invalid Base Password", "That password is invalid. You have 1 attempt left.Try again?",
                                    QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
       
       if (cont == QMessageBox::No)
           exit(0);
       return;
    }
    
    str_tbt = new char[l2l2pass.length() + 1];
    strcpy(str_tbt, l2l2pass.c_str());

    if (!(log.whiteString(str_tbt))) {
        authattempts++;
        chk_auth_att();
        QMessageBox::StandardButton cont;
        cont = QMessageBox::warning(this, "Invalid Level 2 Password", "That password is invalid. You have 1 attempt left.Try again?",
                    QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

        if (cont == QMessageBox::No)
            exit(0);
        return;
    }
    
    chkAuth(); 
}

void regAuth::chkAuth() {
    int loa;
    Encrypt enc;
    try {
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
        sql::ResultSet *res;
 
        /* Create a connection */
        driver = sql::mysql::get_driver_instance();
        con = driver->connect("localhost", "root", "vehiclesecure");

        /* Connect to the MySQL test database */
        stmt = con->createStatement();
        stmt->execute("USE FinalYearProject");
        
        /*finding the user with entered username*/
        res = stmt->executeQuery("SELECT Username, LevelOfAccess FROM Admin");
        bool chk_usr = false;
        while (res->next()){
            if (res->getString("Username") == l2username) {
                chk_usr = true;
                loa = res->getInt("LevelOfAccess");
            }
        }
        
        if(!chk_usr) {
            authattempts++;
            chk_auth_att();
            QMessageBox::StandardButton cont;
            cont = QMessageBox::warning(this, "Invalid Username", "The username you have entered does not exist. You have 1 attempt left.\nTry again?",
                            QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            
            if(cont == QMessageBox::No)
                exit(0);
            
            return;
        }
        
        if(loa != 2) {
            QMessageBox::warning(this, "Authentication Not Authorised", "You do not have a high enough access level to authenticate a registration.\nThe system will now exit.");
            exit(0);
        }
        
        char *user;
        user = new char[l2username.length() + 1];
        strcpy(user, l2username.c_str());
        char *basepass;
        basepass = new char[l2basepass.length() + 1];
        strcpy(basepass, l2basepass.c_str());

        string encpass = enc.enctext(basepass, user);
        
        char *l2pass;
        l2pass = new char[l2l2pass.length() + 1];
        strcpy(l2pass, l2l2pass.c_str());                    
        string encl2pass = enc.enctext(l2pass, basepass);
        
        ostringstream sqlstream;
        string sqlquery;
        sqlstream << "SELECT BasePassword, Level2Password FROM Admin WHERE Username = '" << l2username << "'";
        sqlquery = sqlstream.str();
        
        res = stmt->executeQuery(sqlquery);
        bool val_pass = false;
        while (res->next()) {
            if ((res->getString("BasePassword") == encpass) && (res->getString("Level2Password") == encl2pass))
                val_pass = true;
        }
        
        if(!val_pass) {
            authattempts++;
            chk_auth_att();
            QMessageBox::StandardButton cont;
            cont = QMessageBox::warning(this, "Invalid Credentials", "The username/password combination you have entered is incorrect. You have 1 attempt left\nTry again?",
                            QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            
            if(cont == QMessageBox::No)
                exit(0);
            
            return;
        } else {
            isauth = true;
            this->hide();
        }
        
    } catch (sql::SQLException &e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }
               
}
    
void regAuth::chk_auth_att() {
    if (authattempts == 2) {
        QMessageBox::critical(this, "Could Not Authenticate", "The registration process could not be authenticated. The system will now exit.");
        exit(0);
    }
}