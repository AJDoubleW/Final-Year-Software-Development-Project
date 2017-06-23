/*Administrator Application Login Form*/

/*Project file headers*/
#include "Login.h"
#include "regForm.h"
#include "MainMenu.h"
#include "loaGetSet.h"
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

using namespace std;

string username;
string basepassword;
string l2password;
int logattempts = 0;
int l2attempts = 0;
int logloa;

Login::Login() {
    widget.setupUi(this);
}

Login::~Login() {
}

void Login::on_logButton_clicked() {
    bool validWS = true;
    char *str_tbt;
    username = widget.userEdit->text().toStdString();
    basepassword = widget.passEdit->text().toStdString();
    
    str_tbt = new char[username.length() + 1];
    strcpy(str_tbt, username.c_str());
    
    if(!(whiteString(str_tbt))) {
       validWS = false;
       logattempts++;
       chk_att();
       QMessageBox::StandardButton cont;
       cont = QMessageBox::warning(this, "Invalid Username", QString("That username is invalid. "
               "You have %1 attempts left.Try again?").arg(3-logattempts),
               QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
       
       if (cont == QMessageBox::No) 
           exit(0);
       
    }
    
    str_tbt = new char[basepassword.length() + 1];
    strcpy(str_tbt, basepassword.c_str());
    
    if(!(whiteString(str_tbt))) {
       validWS = false;
       logattempts++;
       chk_att();
       QMessageBox::StandardButton cont;
       cont = QMessageBox::warning(this, "Invalid Password", QString("That password is invalid. You have %1 attempts left.Try again?").arg(3-logattempts),
                                    QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
       
       if (cont == QMessageBox::No)
           exit(0);
       
    }
    
    if (validWS)
        chkLogin();
}
    

void Login::chkLogin() {
    char *str_tbt;
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
        res = stmt->executeQuery("SELECT Username FROM Admin");
        bool chk_usr = false;
        while (res->next()){
            if (res->getString("Username") == username)
                chk_usr = true;
        }
        
        
       /*if there is a user match - find the password and level of access associated with the entered user*/
        if (chk_usr) {
            char *user;
            user = new char[username.length() + 1];
            strcpy(user, username.c_str());
            char *basepass;
            basepass = new char[basepassword.length() + 1];
            strcpy(basepass, basepassword.c_str());

            string encpass = enc.enctext(basepass, user);
            
            ostringstream sqlstream; //utilising a string stream to create the sql query
            sqlstream << "SELECT BasePassword, LevelOfAccess FROM Admin WHERE Username = '" << username << "'";
            string sqlstring = sqlstream.str();
            res = stmt->executeQuery(sqlstring);
            bool chk_pass = false;
            while (res->next()) {
                if (res->getString("BasePassword") == encpass) {
                    chk_pass = true;
                    logloa = res->getInt("LevelOfAccess");
                }
            }
            
            /*if the password checks out, take the user to the main menu*/
            if (chk_pass && (logloa != 2)) {
                QMessageBox::information(this, "Login Successful", QString("Welcome %1!").arg(username.c_str()));
                setLoa(logloa);
                MainMenu mainmenu;
                mainmenu.setModal(true);
                this->hide();
                mainmenu.exec();
            } else if (chk_pass && (logloa == 2)) {
                while (l2attempts < 2) {
                    l2password = QInputDialog::getText(this, "Level 2 Password", "Please enter your level 2 password: ", QLineEdit::Password).toStdString();
                    str_tbt = new char[l2password.length() + 1];
                    strcpy(str_tbt, l2password.c_str());
                    
                    if (!(whiteString(str_tbt))) {
                        l2attempts++;
                        chk_l2_att();
                        QMessageBox::StandardButton cont;
                        cont = QMessageBox::warning(this, "Invalid Password", QString("That password is invalid. You have %1 attempts left.Try again?").arg(3-logattempts),
                                    QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
       
                        if (cont == QMessageBox::No)
                            exit(0);
                        
                    }
                    
                    char *l2pass;
                    l2pass = new char[l2password.length() + 1];
                    strcpy(l2pass, l2password.c_str());                    
                    string encl2pass = enc.enctext(l2pass, basepass);
                    
                    bool chk_l2_pass = false;
                    sqlstream.str("");
                    sqlstream << "SELECT Level2Password FROM Admin WHERE Username = '" << username << "'";
                    sqlstring = sqlstream.str();
                    res = stmt->executeQuery(sqlstring);
                    while (res->next()) {
                        if (res->getString("Level2Password") == encl2pass)
                            chk_l2_pass = true;
                    }
                    
                    if (chk_l2_pass) {
                        QMessageBox::information(this, "Login Successful", QString("Welcome %1!").arg(username.c_str()));
                        setLoa(logloa);
                        MainMenu mainmenu;
                        mainmenu.setModal(true);
                        this->hide();
                        mainmenu.exec();
                        break;
                    } else {
                        l2attempts++;
                        chk_l2_att();
                        QMessageBox::StandardButton cont;
                        cont = QMessageBox::warning(this, "Incorrect Password", "That password is incorrect. You have 1 attempt left.Try again?",
                                    QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
       
                        if (cont == QMessageBox::No)
                            exit(0);
                    }
                }
            } else {
                display_inc_cred();
            }
        } else {
            display_inc_cred();
        }
        
        delete res;
        delete stmt;
        delete con;
        
        
    } catch (sql::SQLException &e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    } 
}


/* Method to ensure no malicious characters are inputted */
bool Login::whiteString(char in_str[]) {
    for (int i=0; i<strlen(in_str); i++){
        if (!(isalnum(in_str[i]))) {
            return false;
        }
    }
    return true;
}

/*Method checking whether the maximum limit of login attempts has been reached*/
void Login::chk_att() {
    if (logattempts == 3) {
        QMessageBox::critical(this,"Attempt Limit Reached", "You have attempted to login with invalid/incorrect credentials too many times. "
                            "\nThe system will now exit.");
        exit(0);
    }
}

void Login::display_inc_cred() {
    logattempts++;
    chk_att();
    QMessageBox::warning(this, "Invalid Credentials", QString("That username/password combination is incorrect.\nYou have %1 attempts remaining.Try again?").arg(3-logattempts),
                        QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
}

void Login::chk_l2_att() {
    if (l2attempts == 2) {
        QMessageBox::critical(this,"Attempt Limit Reached", "You have attempted to login with invalid/incorrect credentials too many times. "
                            "\nThe system will now exit.");
        exit(0);
    }
}

void Login::on_regButton_clicked() {
    regForm regf;
    regf.setModal(true);
    this->hide();
    regf.exec();
}