/*Driver Registration Qt Form*/

/*Project file headers*/
#include "RegForm.h"
#include "MainMenu.h"
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

int Age;
string regAlias;
string regPass;
string regLicType;
string regDOB;
string regLicExp;
ostringstream regsqlstream;
string regsqlquery;

RegForm::RegForm() {
    widget.setupUi(this);
    
    /*Adding selection options for Combo boxes*/
    widget.dayCB->addItem("Day");
    widget.expDayCB->addItem("Day");
    for (int i = 1; i < 32; i++) {
        widget.dayCB->addItem(QString::number(i));
        widget.expDayCB->addItem(QString::number(i));
    }
    widget.yearCB->addItem("Year");
    for (int i = 2016; i > 1899; i--) {
        widget.yearCB->addItem(QString::number(i));
    }
    widget.expYearCB->addItem("Year");
    for (int i = 2016; i < 2030; i++) {
        widget.expYearCB->addItem(QString::number(i));
    }
    widget.monCB->addItem("Month");
    widget.expMonCB->addItem("Month");
    for (int i = 1; i < 13; i++) {
        widget.monCB->addItem(QString::number(i));
        widget.expMonCB->addItem(QString::number(i));
    }
}

RegForm::~RegForm() {
}

void RegForm::on_homeButton_clicked() {
    MainMenu mainmenu;
    mainmenu.setModal(true);
    this->hide();
    mainmenu.exec();
}

void RegForm::on_scanButton_clicked() {
    char *str_tbt;
    regAlias = widget.alias->text().toStdString();
    regPass = widget.backPass->text().toStdString();
    
    if (widget.fullButton->isChecked())
        regLicType = "Full";
    else if (widget.provButton->isChecked())
        regLicType = "Provisional";
    else
        regLicType = "None";
    
    regsqlstream.str("");
    regsqlstream << widget.yearCB->currentText().toStdString() << "-" << widget.monCB->currentText().toStdString() << "-"
                    << widget.dayCB->currentText().toStdString();
    regDOB = regsqlstream.str();
    
    regsqlstream.str("");
    regsqlstream << widget.expYearCB->currentText().toStdString() << "-" << widget.expMonCB->currentText().toStdString() << "-"
                    << widget.expDayCB->currentText().toStdString();
    regLicExp = regsqlstream.str();
    
    /*Testing entry fields for data*/
    if (regAlias.length() == 0) {
        QMessageBox::warning(this, "No Alias Entered", "You have not entered a driver alias. Please complete all fields.");
        return;
    }
    
    if (regPass.length() == 0) {
        QMessageBox::warning(this, "No Password Entered", "You have not entered a backup password. Please complete all fields.");
        return;
    }
    
    if ((widget.yearCB->currentText().toInt() == 0) || (widget.monCB->currentText().toInt() == 0) || (widget.dayCB->currentText().toInt() == 0)) {
        QMessageBox::warning(this, "No Date of Birth Entered", "You have not entered a complete date of birth. Please complete all fields.");
        return;
    }
    
    if (regLicType.length() == 0) {
        QMessageBox::warning(this, "No Licence Type Entered", "You have not entered a driver licence type. Please complete all fields.");
        return;
    }
    
    if (regLicType != "None") {
        if ((widget.expYearCB->currentText().toInt() == 0) || (widget.expMonCB->currentText().toInt() == 0) || (widget.expDayCB->currentText().toInt() == 0)) {
            QMessageBox::warning(this, "No Licence Expiry Date Entered", "You have not entered a complete licence expiry date. Please complete all fields.");
            return;
        }
    }
    
    str_tbt = new char[regAlias.length() + 1];
    strcpy(str_tbt, regAlias.c_str());
    
    if (!(len_chk(str_tbt))) {
        QMessageBox::StandardButton cont;
        cont = QMessageBox::warning(this, "Invalid Username Length", "That username is of invalid length. Usernames should be 8-32 characters long.\nTry again?",
                                   QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
       
        if (cont == QMessageBox::No) 
            exit(0);
        return;
    }
    
    if(!(whiteString(str_tbt))) {
        QMessageBox::StandardButton cont;
        cont = QMessageBox::warning(this, "Invalid Username", "That username is invalid. Usernames should be alphanumeric characters only.\nTry again?",
                                   QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
       
        if (cont == QMessageBox::No) 
            exit(0);
        return;       
    }
    
    str_tbt = new char[regPass.length() + 1];
    strcpy(str_tbt, regPass.c_str());
    
    if (!(len_chk(str_tbt))) {
        QMessageBox::StandardButton cont;
        cont = QMessageBox::warning(this, "Invalid Password Length", "That password is of invalid length. Passwords should be 8-32 characters long.\nTry again?",
                                   QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
       
        if (cont == QMessageBox::No) 
            exit(0);
        return;
    }
    
    if(!(whiteString(str_tbt))) {
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
    
    /*Checking all dates are valid*/
    if (!validDate(widget.dayCB->currentText().toInt(), widget.monCB->currentText().toInt(), widget.yearCB->currentText().toInt())) {
        QMessageBox::warning(this, "Invalid Date of Birth", "This date of birth is invalid as it is not a real date.");
        return;
    }
    
    if (!test_age(widget.dayCB->currentText().toInt(), widget.monCB->currentText().toInt(), widget.yearCB->currentText().toInt(), 17)) {
        QMessageBox::warning(this, "Invalid Date of Birth", "This date of birth is invalid. The driver would be under 17.");
        return;
    }
    
    if (regLicType != "None") {
        if (!validDate(widget.expDayCB->currentText().toInt(), widget.expMonCB->currentText().toInt(), widget.expYearCB->currentText().toInt())) {
        QMessageBox::warning(this, "Invalid Licence Expiry Date", "This licence expiry date is invalid as it is not a real date.");
        return;
        }
        
        if (!valid_exp_date(widget.expDayCB->currentText().toInt(), widget.expMonCB->currentText().toInt(), widget.expYearCB->currentText().toInt())) {
        QMessageBox::warning(this, "Invalid Licence Expiry Date", "This licence expiry date is invalid. The expiry date should be within 10 years.\n(Unless you are over 70, then within the next 3 years).");
        return;
        }
    }
    
    chk_reg();
}

void RegForm::chk_reg() {
    Encrypt enc;
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
        
        /*Check whether username already exists*/
        res = stmt->executeQuery("SELECT Alias FROM Driver");
        bool chk_usr = false;
        while (res->next()){
            if (res->getString("Alias") == regAlias)
                chk_usr = true;
        }
        delete res;
        
        if (chk_usr) {
            QMessageBox::warning(this, "Invalid Alias", "The alias you have entered already belongs to a different driver.");
            return; 
        }
        
        char *alias;
        alias = new char[regAlias.length() + 1];
        strcpy(alias, regAlias.c_str());
        char *pass;
        pass = new char[regPass.length() + 1];
        strcpy(pass, regPass.c_str());
        
        string encpass = enc.enctext(pass, alias);
                
        regsqlstream.str("");
        regsqlstream << "INSERT INTO Driver (Alias, Backup_Password, DateOfBirth, LicenceType, LicenceExpDate) VALUES ('" << regAlias << "','" << encpass <<
                    "','" << regDOB << "','" << regLicType << "','" << regLicExp << "')";
        regsqlquery = regsqlstream.str();
        
        stmt->execute(regsqlquery);
        
        QMessageBox::information(this, "Registration Successful", "Success! You are now on the VehicleSecure system and can start your vehicle!");
        on_homeButton_clicked();
        
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

bool RegForm::len_chk(char in_str[]) {
    if ((strlen(in_str) < 8) || (strlen(in_str) > 32)) {
        return false;
    } else{
        return true;
    }
}

bool RegForm::whiteString(char in_str[]) {
    for (int i=0; i<strlen(in_str); i++){
        if (!(isalnum(in_str[i]))) {
            return false;
        }
    }
    return true;
}

bool RegForm::pass_stren(char pass[]) {
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

bool RegForm::test_age(int day, int mon, int year, int limit) {
    //establishing the current Date	
    time_t currTime;
    struct tm *timeConv;
    time(&currTime);
    timeConv = localtime(&currTime);
    int cDay = timeConv->tm_mday;
    int cMon = timeConv->tm_mon + 1;
    int cYear = timeConv->tm_year + 1900;

    //calculating the user age
    Age = cYear - year;
    if ((cMon < mon) || ((cMon == mon) && (cDay < day))){
        Age -= 1; //if their birthday is later in the year
    }

    if (Age >= limit) {
        return true;
    }else {
        return false;
    }
}

bool RegForm::validDate(int day, int mon, int year) {
    //establishing the current date
    time_t currTime;
    struct tm *timeConv;
    time(&currTime);
    timeConv = localtime(&currTime);
    int cDay = timeConv->tm_mday;
    int cMon = timeConv->tm_mon + 1;
    int cYear = timeConv->tm_year + 1900;
    
    if (((mon == 2) && (day > 29)) || (((mon == 4) || (mon == 6) || (mon == 9) || (mon == 11)) && (day == 31)) 
            || ((mon == 2) && (year%4 != 0) && (day > 28)))
        return false;
    else 
        return true;
}

/* Testing the validity of the licence expiry date*/
bool RegForm::valid_exp_date(int day, int mon, int year) {
    //establishing the current Date	
    time_t currTime;
    struct tm *timeConv;
    time(&currTime);
    timeConv = localtime(&currTime);
    int cDay = timeConv->tm_mday;
    int cMon = timeConv->tm_mon + 1;
    int cYear = timeConv->tm_year + 1900;


    if ((year < cYear) || ((year == cYear) && ((mon < cMon) || ((mon == cMon) && (day < cDay)))))
        return false;
    else if (((year - cYear) > 10) || (((year - cYear) == 10) && ((mon > cMon) || ((mon == cMon) && (day > cDay))))) //driving licence photocards are only valid for 10 years
        return false;
    else if ((Age > 70) && (((year - cYear) > 3) || (((year - cYear) == 3) && ((mon > cMon) || ((mon == cMon) && (day > cDay)))))) //if over the age of 70, they are only valid for 3 years
        return false;
    else if ((Age > 60) && (((year - cYear) > (70 - Age)) || (((year - cYear) == (70-Age)) && ((mon > cMon) || ((mon == cMon) && (day > cDay))))))
        return false;
    return true;
}