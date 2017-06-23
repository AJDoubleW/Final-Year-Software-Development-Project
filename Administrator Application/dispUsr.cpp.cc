/*Administrator Application Display Users Form*/

/*Project file headers*/
#include "dispUsr.h"
#include "MainMenu.h"
#include "loaGetSet.h"
#include "Login.h"

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

int userID;
string ins_username;
ostringstream usrsqlstream;
string usrsqlquery;

dispUsr::dispUsr() {
    widget.setupUi(this);
    widget.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    widget.tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    widget.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    fillTable();
}

dispUsr::~dispUsr() {
}

void dispUsr::fillTable() {
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
        
        /*Determining the number of rows*/
        int rows;
        res = stmt->executeQuery("SELECT * FROM Insurer");
        rows = static_cast<int>(res->rowsCount());
        if (rows == 0)
          QMessageBox::information(this, "No Insurers", "There are no insurers currently registered on the system.");
    
        widget.tableWidget->setRowCount(rows);
        
        /*Filling the table*/
        int i = 0;
        QTableWidgetItem *item;
        res = stmt->executeQuery("SELECT * FROM Insurer");
        while (res->next()) {
            item = new QTableWidgetItem(QString("%1").arg(res->getInt("InsurerID")));
            widget.tableWidget->setItem(i, 0, item);
            item = new QTableWidgetItem(QString("%1").arg(res->getString("Username").c_str()));
            widget.tableWidget->setItem(i, 1, item);
            i++;
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


void dispUsr::on_tableWidget_cellClicked() {
    userID = widget.tableWidget->item(widget.tableWidget->currentRow(), 0)->text().toInt();
    ins_username = widget.tableWidget->item(widget.tableWidget->currentRow(), 1)->text().toStdString();
}

void dispUsr::on_modButton_clicked() {
    string newuser;
    char *str_tbt;
    Login log;
    if (getLoa() == 0) {
        QMessageBox::critical(this, "Not Authorised", "You are not authorised to perform this action. You must be at least a level 1 administrator to modify an insurer.");
        return;
    }
    
    if (userID == 0) {
        QMessageBox::warning(this, "No Insurer Selected", "You have not selected an insurer to modify.");
        return;
    }
    
    newuser = QInputDialog::getText(this, "New Username", "Please enter a new username for the insurer: ").toStdString();
    str_tbt = new char[newuser.length() + 1];
    strcpy(str_tbt, newuser.c_str());

    if (!(log.whiteString(str_tbt))) {
        QMessageBox::warning(this, "Invalid Username", "That username is invalid. Usernames are alphanumeric only.");
        return;
    }
    
    try {
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
 
        /* Create a connection */
        driver = sql::mysql::get_driver_instance();
        con = driver->connect("localhost", "root", "vehiclesecure");

        /* Connect to the database */
        stmt = con->createStatement();
        stmt->execute("USE FinalYearProject");
        
        usrsqlstream.str("");
        usrsqlstream << "UPDATE Insurer SET Username = '" << newuser << "' WHERE InsurerID = '" << userID << "'";
        usrsqlquery = usrsqlstream.str();
        stmt->execute(usrsqlquery);
        
        QMessageBox::information(this, "Insurer Updated", "This driver has been updated!");
        fillTable();
        
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

void dispUsr::on_delButton_clicked() {
    if (getLoa() != 2) {
        QMessageBox::critical(this, "Not Authorised", "You are not authorised to perform this action. You must be a level 2 administrator to delete an insurer.");
        return;
    }
    
    if (userID == 0) {
        QMessageBox::warning(this, "No Insurer Selected", "You have not selected an insurer to delete.");
        return;
    }
    
    try {
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
 
        /* Create a connection */
        driver = sql::mysql::get_driver_instance();
        con = driver->connect("localhost", "root", "vehiclesecure");

        /* Connect to the database */
        stmt = con->createStatement();
        stmt->execute("USE FinalYearProject");
        
        usrsqlstream.str("");
        usrsqlstream << "DELETE FROM Insurer WHERE InsurerID = '" << userID << "'";
        usrsqlquery = usrsqlstream.str();
        stmt->execute(usrsqlquery);
        
        QMessageBox::information(this, "Insurer Deleted", "You have deleted this Insurer from the system.");
        fillTable();
        
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

void dispUsr::on_homeButton_clicked() {
    MainMenu mainmenu;
    mainmenu.setModal(true);
    this->hide();
    mainmenu.exec();
}