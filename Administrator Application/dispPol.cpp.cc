/*Administrator Application Display Policies Form*/

/*Project file headers*/
#include "dispPol.h"
#include "modPol.h"
#include "MainMenu.h"
#include "loaGetSet.h"

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

ostringstream polsqlstream;
string polsqlquery;
int polDid;
int polVec;
string polAlias;
string polType;
string issueDate;
string expiryDate;

dispPol::dispPol() {
    widget.setupUi(this);
    widget.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    widget.tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    widget.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    fillTable();
}

dispPol::~dispPol() {
}

void dispPol::fillTable() {
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
        res = stmt->executeQuery("SELECT * FROM InsurancePolicy");
        rows = static_cast<int>(res->rowsCount());
        if (rows == 0) {
          QMessageBox::information(this, "No Insurance Policies", "There are no insurance policies.");
          //on_homeButton_clicked();
        }
        widget.tableWidget->setRowCount(rows);
        
        /*Filling the table*/
        int i = 0;
        int driID [rows];
        QTableWidgetItem *item;
        res = stmt->executeQuery("SELECT * FROM InsurancePolicy");
        while (res->next()) {
            driID[i] = res->getInt("DriverID");
            item = new QTableWidgetItem(QString("%1").arg(res->getInt("VehicleID")));
            widget.tableWidget->setItem(i, 1, item);
            item = new QTableWidgetItem(QString("%1").arg(res->getString("PolicyType").c_str()));
            widget.tableWidget->setItem(i, 2, item);
            item = new QTableWidgetItem(QString("%1").arg(res->getString("IssueDate").c_str()));
            widget.tableWidget->setItem(i, 3, item);
            item = new QTableWidgetItem(QString("%1").arg(res->getString("ExpiryDate").c_str()));
            widget.tableWidget->setItem(i, 4, item);
            i++;
        }
        
        for (i = 0; i < rows+1; i++) {
            polsqlstream.str("");
            polsqlstream << "SELECT Alias FROM Driver WHERE DriverID = '" << driID[i] << "'";
            polsqlquery = polsqlstream.str();
            res = stmt->executeQuery(polsqlquery);
            while (res->next()) {
                item = new QTableWidgetItem(QString("%1").arg(res->getString("Alias").c_str()));
                widget.tableWidget->setItem(i, 0, item); 
            }
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

void dispPol::on_tableWidget_cellClicked() {
    polAlias = widget.tableWidget->item(widget.tableWidget->currentRow(), 0)->text().toStdString();
    polVec = widget.tableWidget->item(widget.tableWidget->currentRow(), 1)->text().toInt();
    polType = widget.tableWidget->item(widget.tableWidget->currentRow(), 2)->text().toStdString();
    issueDate = widget.tableWidget->item(widget.tableWidget->currentRow(), 3)->text().toStdString();
    expiryDate = widget.tableWidget->item(widget.tableWidget->currentRow(), 4)->text().toStdString();
    
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
        
        polsqlstream.str("");
        polsqlstream << "SELECT DriverID FROM Driver WHERE Alias = '" << polAlias << "'";
        polsqlquery = polsqlstream.str();
        res = stmt->executeQuery(polsqlquery);
        while (res->next()) {
            polDid = res->getInt("DriverID");
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

void dispPol::on_modButton_clicked() {
    if (getLoa() == 0) {
        QMessageBox::critical(this, "Not Authorised", "You are not authorised to perform this action. You must be at least a level 1 administrator to modify a vehicle.");
        return;
    }
    
    if (polVec == 0) {
        QMessageBox::warning(this, "No Policy Selected", "You have not selected a policy to modify.");
        return;
    }
    
    char *polAliasCh = new char[polAlias.length() + 1];
    strcpy(polAliasCh, polAlias.c_str());
    char *polTypeCh = new char[polType.length() + 1];
    strcpy(polTypeCh, polType.c_str());
    char *issueDateCh = new char[issueDate.length() + 1];
    strcpy(issueDateCh, issueDate.c_str());
    char *expiryDateCh = new char[expiryDate.length() + 1];
    strcpy(expiryDateCh, expiryDate.c_str());
    
    modPol mpol;
    mpol.pol_tbe(polAliasCh, polVec, polTypeCh, issueDateCh, expiryDateCh);
    mpol.setModal(true);
    this->hide();
    mpol.exec();
}

void dispPol::on_delButton_clicked() {
    if (getLoa() != 2) {
        QMessageBox::critical(this, "Not Authorised", "You are not authorised to perform this action. You must be a level 2 administrator to delete a vehicle.");
        return;
    }
    
    if (polVec == 0) {
        QMessageBox::warning(this, "No Policy Selected", "You have not selected a policy to delete.");
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
        
        polsqlstream.str("");
        polsqlstream << "DELETE FROM InsurancePolicy WHERE DriverID = '" << polDid << "' AND VehicleID = '" << polVec << 
                    "' AND PolicyType = '" << polType << "' AND IssueDate = '" << issueDate << "' AND ExpiryDate = '" << expiryDate << "'";
        polsqlquery = polsqlstream.str();
        stmt->execute(polsqlquery);
        
        QMessageBox::information(this, "Policy Deleted", "You have deleted this policy from the system.");
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

void dispPol::on_homeButton_clicked() {
    MainMenu mainmenu;
    mainmenu.setModal(true);
    this->hide();
    mainmenu.exec();
}