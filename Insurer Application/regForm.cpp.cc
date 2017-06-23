/*Insurer Registration Form*/

/*Project file headers*/
#include "regForm.h"
#include "Login.h"
#include "RegLog.h"
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

using namespace std;

string attuser;
string attpass;

regForm::regForm() {
    widget.setupUi(this);
}


regForm::~regForm() {
}

void regForm::on_regButton_clicked() {
    bool validWS = true;
    char *str_tbt;
    Login log;
    attuser = widget.userEdit->text().toStdString();
    attpass = widget.passEdit->text().toStdString();
    
    str_tbt = new char[attuser.length() + 1];
    strcpy(str_tbt, attuser.c_str());
    
    if (!(len_chk(str_tbt))) {
        validWS = false;
        QMessageBox::StandardButton cont;
        cont = QMessageBox::warning(this, "Invalid Username Length", "That username is of invalid length. Usernames should be 8-32 characters long.\nTry again?",
                                   QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
       
        if (cont == QMessageBox::No) 
            exit(0);
    }
    
    if(!(log.whiteString(str_tbt))) {
        validWS = false;
        QMessageBox::StandardButton cont;
        cont = QMessageBox::warning(this, "Invalid Username", "That username is invalid. Usernames should be alphanumeric characters only.\nTry again?",
                                   QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
       
        if (cont == QMessageBox::No) 
            exit(0);
       
    }
    
    str_tbt = new char[attpass.length() + 1];
    strcpy(str_tbt, attpass.c_str());
    
    if (!(len_chk(str_tbt))) {
        validWS = false;
        QMessageBox::StandardButton cont;
        cont = QMessageBox::warning(this, "Invalid Password Length", "That password is of invalid length. Passwords should be 8-32 characters long.\nTry again?",
                                   QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
       
        if (cont == QMessageBox::No) 
            exit(0);
    }
    
    if(!(log.whiteString(str_tbt))) {
        validWS = false;
        QMessageBox::StandardButton cont;
        cont = QMessageBox::warning(this, "Invalid Password", "That password is invalid. Passwords should be alphanumeric characters only.\nTry again?",
                                    QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
       
        if (cont == QMessageBox::No)
            exit(0);
       
    }
    
    if(!(pass_stren(str_tbt))) {
        validWS = false;
        QMessageBox::StandardButton cont;
        cont = QMessageBox::warning(this, "Password Too Weak", "That password is too weak. Passwords should contain at least 1 uppercase letter, 1 lowercase letter and 1 digit.\nTry again?",
                                    QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
       
        if (cont == QMessageBox::No)
            exit(0);
       
    }
    
    if (validWS)
        chk_reg();
}
    

void regForm::chk_reg() {
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
        
        /*Check whether username already exists*/
        res = stmt->executeQuery("SELECT Username FROM Insurer");
        bool chk_usr = false;
        while (res->next()){
            if (res->getString("Username") == attuser)
                chk_usr = true;
        }
        delete res;
        
        /*If the username exists, display error */
        if(chk_usr) {
            QMessageBox::StandardButton cont;
            cont = QMessageBox::warning(this,"Username Already Exists", "A user with that username already exists.\nTry again?",
                                QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
            
            if (cont == QMessageBox::No)
                exit(0);
            
            
        } else {
            char *user;
            user = new char[attuser.length() + 1];
            strcpy(user, attuser.c_str());
            char *pass;
            pass = new char[attpass.length() + 1];
            strcpy(pass, attpass.c_str());

            string encpass = enc.enctext(pass, user);
            
            ostringstream sqlstream; //utilising a string stream to create the sql query
            sqlstream << "INSERT INTO Insurer (Username,Password) VALUES ('" << attuser << "','" << encpass << "');";
            string sqlstring = sqlstream.str();
            stmt->execute(sqlstring);

            QMessageBox::information(this, "Registration Complete", "Registration Complete! Please log in to use the system.");
            RegLog reglog;
            reglog.setModal(true);
            this->hide();
            reglog.exec();
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