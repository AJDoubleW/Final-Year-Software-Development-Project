/*Administrator Application Display Drivers Form*/

/*Project file headers*/
#include "modDri.h"
#include "dispDri.h"
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

int mdDriID;
string mdAlias;
string mdDOB;
string mdLicType;
string mdLicExp;

dispDri::dispDri() {
    widget.setupUi(this);
    widget.tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    widget.tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    widget.tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    fillTable();
}

dispDri::~dispDri() {
}

void dispDri::fillTable() {
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
        res = stmt->executeQuery("SELECT * FROM Driver");
        rows = static_cast<int>(res->rowsCount());
        if (rows == 0)
          QMessageBox::information(this, "No Drivers", "There are no drivers currently registered on the system.");
    
        widget.tableWidget->setRowCount(rows);
        
        /*Filling the table*/
        int i = 0;
        QTableWidgetItem *item;
        res = stmt->executeQuery("SELECT * FROM Driver");
        while (res->next()) {
            item = new QTableWidgetItem(QString("%1").arg(res->getInt("DriverID")));
            widget.tableWidget->setItem(i, 0, item);
            item = new QTableWidgetItem(QString("%1").arg(res->getString("Alias").c_str()));
            widget.tableWidget->setItem(i, 1, item);
            item = new QTableWidgetItem(QString("%1").arg(res->getString("DateOfBirth").c_str()));
            widget.tableWidget->setItem(i, 2, item);
            item = new QTableWidgetItem(QString("%1").arg(res->getString("LicenceType").c_str()));
            widget.tableWidget->setItem(i, 3, item);
            item = new QTableWidgetItem(QString("%1").arg(res->getString("LicenceExpDate").c_str()));
            widget.tableWidget->setItem(i, 4, item);
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

void dispDri::on_tableWidget_cellClicked() {
    mdDriID = widget.tableWidget->item(widget.tableWidget->currentRow(), 0)->text().toInt();
    mdAlias = widget.tableWidget->item(widget.tableWidget->currentRow(), 1)->text().toStdString();
    mdDOB = widget.tableWidget->item(widget.tableWidget->currentRow(), 2)->text().toStdString();
    mdLicType = widget.tableWidget->item(widget.tableWidget->currentRow(), 3)->text().toStdString();
    mdLicExp = widget.tableWidget->item(widget.tableWidget->currentRow(), 4)->text().toStdString();
    
}

void dispDri::on_modButton_clicked() {
    if (getLoa() == 0) {
        QMessageBox::critical(this, "Not Authorised", "You are not authorised to perform this action. You must be at least a level 1 administrator to modify a driver.");
        return;
    }
    
    if (mdDriID == 0) {
        QMessageBox::warning(this, "No Driver Selected", "You have not selected a driver to modify.");
        return;
    }
    
    modDri mod;
    mod.dri_tbe(mdDriID);
    mod.setModal(true);
    this->hide();
    mod.exec();
}

void dispDri::on_delButton_clicked() {
    ostringstream drisqlstream;
    string drisqlquery;
    
    if (getLoa() != 2) {
        QMessageBox::critical(this, "Not Authorised", "You are not authorised to perform this action. You must be a level 2 administrator to delete a driver.");
        return;
    }
    
    if (mdDriID == 0) {
        QMessageBox::warning(this, "No Driver Selected", "You have not selected a driver to delete.");
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
        
        drisqlstream << "DELETE FROM CompPerms WHERE CreatorID = '" << mdDriID << "' OR DriverID = '" << mdDriID << "'";
        drisqlquery = drisqlstream.str();
        stmt->execute(drisqlquery);
        
        drisqlstream.str("");
        drisqlstream << "DELETE FROM InsurancePolicy WHERE DriverID = '" << mdDriID << "'";
        drisqlquery = drisqlstream.str();
        stmt->execute(drisqlquery);
        drisqlstream.str("");
        
        drisqlstream << "DELETE FROM Alert WHERE DriverID = '" << mdDriID << "'";
        drisqlquery = drisqlstream.str();
        stmt->execute(drisqlquery);
        
        drisqlstream.str("");
        drisqlstream << "UPDATE Vehicle SET Owner = '0' WHERE DriverID = '" << mdDriID << "'";
        drisqlquery = drisqlstream.str();
        stmt->execute(drisqlquery);
        
        drisqlstream.str("");
        drisqlstream << "DELETE FROM Driver WHERE DriverID = '" << mdDriID << "'";
        drisqlquery = drisqlstream.str();
        stmt->execute(drisqlquery);
        
        QMessageBox::information(this, "Driver Deleted", "You have deleted this driver from the system.");
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

void dispDri::on_homeButton_clicked() {
    MainMenu mainmenu;
    mainmenu.setModal(true);
    this->hide();
    mainmenu.exec();
}