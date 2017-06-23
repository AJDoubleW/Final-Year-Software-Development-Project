/********************************************************************************
** Form generated from reading UI file 'Applauncher.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_APPLAUNCHER_H
#define UI_APPLAUNCHER_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_Applauncher
{
public:
    QPushButton *driLaunch;
    QPushButton *insLaunch;
    QPushButton *adminLaunch;
    QLabel *titleLabel;
    QLabel *subtitleLabel;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;

    void setupUi(QDialog *Applauncher)
    {
        if (Applauncher->objectName().isEmpty())
            Applauncher->setObjectName(QString::fromUtf8("Applauncher"));
        Applauncher->resize(1280, 720);
        driLaunch = new QPushButton(Applauncher);
        driLaunch->setObjectName(QString::fromUtf8("driLaunch"));
        driLaunch->setGeometry(QRect(63, 510, 300, 150));
        QPalette palette;
        QBrush brush(QColor(7, 19, 239, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(3, 67, 16, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Text, brush1);
        QBrush brush2(QColor(7, 8, 76, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush2);
        QBrush brush3(QColor(159, 158, 158, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush3);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush3);
        driLaunch->setPalette(palette);
        QFont font;
        font.setFamily(QString::fromUtf8("Loma"));
        font.setPointSize(26);
        font.setBold(true);
        font.setItalic(false);
        font.setWeight(75);
        driLaunch->setFont(font);
        driLaunch->setMouseTracking(false);
        driLaunch->setFocusPolicy(Qt::ClickFocus);
        driLaunch->setContextMenuPolicy(Qt::DefaultContextMenu);
        driLaunch->setStyleSheet(QString::fromUtf8("QPushButton:hover#driLaunch {\n"
"background-color:#aaaaaa;\n"
"color:#222222;\n"
"border-radius: 4px;\n"
"}"));
        driLaunch->setAutoDefault(false);
        driLaunch->setDefault(false);
        driLaunch->setFlat(false);
        insLaunch = new QPushButton(Applauncher);
        insLaunch->setObjectName(QString::fromUtf8("insLaunch"));
        insLaunch->setGeometry(QRect(490, 510, 300, 150));
        QPalette palette1;
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Active, QPalette::Text, brush1);
        palette1.setBrush(QPalette::Active, QPalette::ButtonText, brush2);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::Text, brush1);
        palette1.setBrush(QPalette::Inactive, QPalette::ButtonText, brush2);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        palette1.setBrush(QPalette::Disabled, QPalette::Text, brush3);
        palette1.setBrush(QPalette::Disabled, QPalette::ButtonText, brush3);
        insLaunch->setPalette(palette1);
        insLaunch->setFont(font);
        insLaunch->setFocusPolicy(Qt::ClickFocus);
        insLaunch->setStyleSheet(QString::fromUtf8("QPushButton:hover#insLaunch {\n"
"background-color:#aaaaaa;\n"
"color:#222222;\n"
"border-radius:4px;\n"
"}"));
        insLaunch->setAutoDefault(false);
        adminLaunch = new QPushButton(Applauncher);
        adminLaunch->setObjectName(QString::fromUtf8("adminLaunch"));
        adminLaunch->setGeometry(QRect(917, 510, 300, 150));
        QPalette palette2;
        palette2.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette2.setBrush(QPalette::Active, QPalette::Text, brush1);
        palette2.setBrush(QPalette::Active, QPalette::ButtonText, brush2);
        palette2.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::Text, brush1);
        palette2.setBrush(QPalette::Inactive, QPalette::ButtonText, brush2);
        palette2.setBrush(QPalette::Disabled, QPalette::WindowText, brush3);
        palette2.setBrush(QPalette::Disabled, QPalette::Text, brush3);
        palette2.setBrush(QPalette::Disabled, QPalette::ButtonText, brush3);
        adminLaunch->setPalette(palette2);
        adminLaunch->setFont(font);
        adminLaunch->setFocusPolicy(Qt::ClickFocus);
        adminLaunch->setStyleSheet(QString::fromUtf8("QPushButton:hover#adminLaunch {\n"
"background-color:#aaaaaa;\n"
"color:#222222;\n"
"border-radius:4px;\n"
"}"));
        adminLaunch->setAutoDefault(false);
        titleLabel = new QLabel(Applauncher);
        titleLabel->setObjectName(QString::fromUtf8("titleLabel"));
        titleLabel->setGeometry(QRect(395, 150, 490, 90));
        QFont font1;
        font1.setFamily(QString::fromUtf8("Loma"));
        font1.setPointSize(56);
        font1.setBold(true);
        font1.setItalic(false);
        font1.setWeight(75);
        titleLabel->setFont(font1);
        titleLabel->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 0)"));
        subtitleLabel = new QLabel(Applauncher);
        subtitleLabel->setObjectName(QString::fromUtf8("subtitleLabel"));
        subtitleLabel->setGeometry(QRect(525, 239, 230, 30));
        QFont font2;
        font2.setFamily(QString::fromUtf8("Loma"));
        font2.setPointSize(18);
        font2.setBold(true);
        font2.setItalic(true);
        font2.setWeight(75);
        subtitleLabel->setFont(font2);
        subtitleLabel->setStyleSheet(QString::fromUtf8("color: rgb(255,255,0)"));
        label = new QLabel(Applauncher);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(0, 0, 1280, 720));
        label->setPixmap(QPixmap(QString::fromUtf8(":/images/road-traffic-car-business.jpg")));
        label_2 = new QLabel(Applauncher);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(50, 50, 100, 100));
        label_2->setPixmap(QPixmap(QString::fromUtf8(":/images/idea 1.png")));
        label_3 = new QLabel(Applauncher);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(50, 50, 100, 100));
        label_3->setAutoFillBackground(false);
        label_3->setStyleSheet(QString::fromUtf8("QLabel#label_3 {\n"
"background-color:#ffff00;\n"
"}"));
        label->raise();
        driLaunch->raise();
        insLaunch->raise();
        adminLaunch->raise();
        titleLabel->raise();
        subtitleLabel->raise();
        label_3->raise();
        label_2->raise();

        retranslateUi(Applauncher);

        QMetaObject::connectSlotsByName(Applauncher);
    } // setupUi

    void retranslateUi(QDialog *Applauncher)
    {
        Applauncher->setWindowTitle(QApplication::translate("Applauncher", "Applauncher", 0, QApplication::UnicodeUTF8));
        driLaunch->setText(QApplication::translate("Applauncher", "Driver", 0, QApplication::UnicodeUTF8));
        insLaunch->setText(QApplication::translate("Applauncher", "Insurer", 0, QApplication::UnicodeUTF8));
        adminLaunch->setText(QApplication::translate("Applauncher", "Administrator", 0, QApplication::UnicodeUTF8));
        titleLabel->setText(QApplication::translate("Applauncher", "VehicleSecure", 0, QApplication::UnicodeUTF8));
        subtitleLabel->setText(QApplication::translate("Applauncher", "Launcher Application", 0, QApplication::UnicodeUTF8));
        label->setText(QString());
        label_2->setText(QString());
        label_3->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class Applauncher: public Ui_Applauncher {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_APPLAUNCHER_H
