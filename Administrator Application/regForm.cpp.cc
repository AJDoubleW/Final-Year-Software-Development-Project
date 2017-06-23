/*Administrator application register form*/

/*Project file headers*/
#include "regForm.h"
#include "Login.h"
#include "RegLog.h"
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

string attuser;
string attbasepass;
string attl2pass;
int regloa;


regForm::regForm() {
    widget.setupUi(this);
}

regForm::~regForm() {
}

void regForm::on_regButton_clicked() {
    char *str_tbt;
    Login log;
    attuser = widget.userEdit->text().toStdString();
    attbasepass = widget.passEdit->text().toStdString();
    regloa = widget.loaSB->value();
    
    str_tbt = new char[attuser.length() + 1];
    strcpy(str_tbt, attuser.c_str());
    
    if (!(len_chk(str_tbt))) {
        QMessageBox::StandardButton cont;
        cont = QMessageBox::warning(this, "Invalid Username Length", "That username is of invalid length. Usernames should be 8-32 characters long.\nTry again?",
                                   QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
       
        if (cont == QMessageBox::No) 
            exit(0);
        return;
    }
    
    if(!(log.whiteString(str_tbt))) {
        QMessageBox::StandardButton cont;
        cont = QMessageBox::warning(this, "Invalid Username", "That username is invalid. Usernames should be alphanumeric characters only.\nTry again?",
                                   QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
       
        if (cont == QMessageBox::No) 
            exit(0);
        return;
    }
    
    str_tbt = new char[attbasepass.length() + 1];
    strcpy(str_tbt, attbasepass.c_str());
    
    if (!(len_chk(str_tbt))) {
        QMessageBox::StandardButton cont;
        cont = QMessageBox::warning(this, "Invalid Password Length", "That password is of invalid length. Passwords should be 8-32 characters long.\nTry again?",
                                   QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
       
        if (cont == QMessageBox::No) 
            exit(0);
        return;
    }
    
    if(!(log.whiteString(str_tbt))) {
        QMessageBox::StandardButton cont;
        cont = QMessageBox::warning(this, "Invalid Password", "That password is invalid. Passwords should be alphanumeric characters only.\nTry again?",
                                    QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
       
        if (cont == QMessageBox::No)
            exit(0);
        return;
    }
    
    if(!(pass_stren(str_tbt))) {
        QMessageBox::StandardButton cont;
        cont = QMessageBox::warning(this, "Password Too Weak", "That password is too weak. Passwords should contain at least 1 uppercase letter, 1 lowercase letter and 1 digit.\nTry again?",
                                    QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
       
        if (cont == QMessageBox::No)
            exit(0);
        return;
    }
    
    if (regloa == 2) {
        attl2pass = QInputDialog::getText(this, "Level 2 Password", "Please enter your level 2 password: ", QLineEdit::Password).toStdString();
        str_tbt = new char[attl2pass.length() + 1];
        strcpy(str_tbt, attl2pass.c_str());
        
        if (!(len_chk(str_tbt))) {
            QMessageBox::StandardButton cont;
            cont = QMessageBox::warning(this, "Invalid Password Length", "That password is of invalid length. Passwords should be 8-32 characters long.\nTry again?",
                                       QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

            if (cont == QMessageBox::No) 
                exit(0);
            return;
        }

        if(!(log.whiteString(str_tbt))) {
            QMessageBox::StandardButton cont;
            cont = QMessageBox::warning(this, "Invalid Password", "That password is invalid. Passwords should be alphanumeric characters only.\nTry again?",
                                        QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

            if (cont == QMessageBox::No)
                exit(0);
            return;
        }

        if(!(pass_stren(str_tbt))) {
            QMessageBox::StandardButton cont;
            cont = QMessageBox::warning(this, "Password Too Weak", "That password is too weak. Passwords should contain at least 1 uppercase letter, 1 lowercase letter and 1 digit.\nTry again?",
                                        QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

            if (cont == QMessageBox::No)
                exit(0);
            return;
        }
    }
    
    regAdmin();
    
}

void regForm::regAdmin() {
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
            if (res->getString("Username") == attuser)
                chk_usr = true;
        }
        
        if (chk_usr) {
            QMessageBox::StandardButton cont;
            cont = QMessageBox::warning(this, "Username Already Taken", "There is already a user with that username.\nTry again?",
                                QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            
            if (cont == QMessageBox::No)
                exit(0); 
        } else {
            regAuth auth;
            auth.setModal(true);
            auth.exec();
            if (auth.isauth) {
                char *user;
                user = new char[attuser.length() + 1];
                strcpy(user, attuser.c_str());
                char *basepass;
                basepass = new char[attbasepass.length() + 1];
                strcpy(basepass, attbasepass.c_str());

                string encpass = enc.enctext(basepass, user);
                
                ostringstream sqlstream;
                string sqlquery;
                if (regloa == 2) {
                    char *l2pass;
                    l2pass = new char[attl2pass.length() + 1];
                    strcpy(l2pass, attl2pass.c_str());                    
                    string encl2pass = enc.enctext(l2pass, basepass);
                    
                    sqlstream << "INSERT INTO Admin (Username,BasePassword,LevelOfAccess,Level2Password) VALUES ('" << attuser << "','" <<
                        encpass << "','" << regloa << "','" << encl2pass << "')";
                } else {
                    sqlstream << "INSERT INTO Admin (Username,BasePassword,LevelOfAccess,Level2Password) VALUES ('" << attuser << "','" <<
                        encpass << "','" << regloa << "', NULL)"; 
                }
                sqlquery = sqlstream.str();
                stmt->execute(sqlquery);
                QMessageBox::information(this, "Registration Complete", "Registration Complete!\nPlease log in to use the system");
                Login log;
                log.setModal(true);
                this->hide();
                log.exec();
            } else {
                QMessageBox::critical(this, "Could Not Authenticate", "Your registration could not be authenticated. The system will now exit.");
                exit(0);
            }
        }
        
    } catch (sql::SQLException &e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << EXAMPLE_FUNCTION << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }
}

/*length check of string method - if not between 8 and 32, return false*/
bool regForm::len_chk(char in_str[]){
    if ((strlen(in_str) < 8) || (strlen(in_str) > 32)) {
        return false;
    } else{
        return true;
    }
}

/*method to ensure the strength of the password*/
bool regForm::pass_stren(char pass[]){
    //declaration of conditions that must be met - ex -> exists
	bool exUpper = false,
	exLower = false,
	exDigit = false;

	//parses array to check whether each character is upper, lower or digit
	for(int i=0; i < strlen(pass); i++) {
		if (isupper(pass[i])) {
			exUpper = true;
		} else if (islower(pass[i])) {
			exLower = true;
		} else if (isdigit(pass[i])) {
			exDigit = true;
		}
	}
        
        //determination of validity
	if(exUpper && exLower && exDigit) {
		return true;
	} else {		
		return false;
	}
}
