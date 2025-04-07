#include <QStyle.h>

#include <mainwindow.h>
#include <source/source.h>
#include <screens/PatientScreen.h>
#include <screens/AppointmentScreen.h>
#include <screens/EmergencyAppointmentScreen.h>
#include <screens/RoomManagementScreen.h>
#include <screens/SuggestDoctorScreen.h>

static void buttonChanges(QPushButton *btn, bool value) {
    btn->setProperty("selected", value);
    btn->style()->unpolish(btn);
    btn->style()->polish(btn);
}

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    source = new Source();

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Sidebar Layout
    QWidget *sidebar = new QWidget;
    QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebar);
    sidebarLayout->setAlignment(Qt::AlignTop);
    sidebarLayout->setSpacing(10);
    sidebarLayout->setContentsMargins(10, 10, 10, 10);
    sidebar->setFixedWidth(200);

    QString btnStyle = R"(
        QPushButton {
            background-color: #2e3b4e;
            color: white;
            font-size: 14px;
            padding: 12px;
            border: none;
            border-radius: 8px;
            text-align: left;
        }
        QPushButton[selected="true"] {
            background-color: #41555e;
            color: white;
        }
        QPushButton:hover {
            background-color: #3f4f63;
        }
    )";

    QPushButton *btn1 = new QPushButton(" Patient Details");
    QPushButton *btn2 = new QPushButton(" Appointment");
    QPushButton *btn3 = new QPushButton(" Emergency Appointment");
    QPushButton *btn4 = new QPushButton(" Room Management");
    QPushButton *btn5 = new QPushButton(" Suggest doctor");

    btn1->setStyleSheet(btnStyle);
    btn2->setStyleSheet(btnStyle);
    btn3->setStyleSheet(btnStyle);
    btn4->setStyleSheet(btnStyle);
    btn5->setStyleSheet(btnStyle);

    sidebarLayout->addWidget(btn1);
    sidebarLayout->addWidget(btn2);
    sidebarLayout->addWidget(btn3);
    sidebarLayout->addWidget(btn4);
    sidebarLayout->addWidget(btn5);
    sidebarLayout->addStretch();

    connect(btn1, &QPushButton::clicked, this, [=](){
        buttonChanges(btn1, false);
        buttonChanges(btn2, false);
        buttonChanges(btn3, false);
        buttonChanges(btn4, false);
        buttonChanges(btn5, false);

        buttonChanges(btn1, true);
        setContent(new PatientScreen(source, this, contentArea));
    });

    connect(btn2, &QPushButton::clicked, this, [=](){
        buttonChanges(btn1, false);
        buttonChanges(btn2, false);
        buttonChanges(btn3, false);
        buttonChanges(btn4, false);
        buttonChanges(btn5, false);

        buttonChanges(btn2, true);
        setContent(new AppointmentScreen(source, contentArea));
    });

    connect(btn3, &QPushButton::clicked, this, [=](){
        buttonChanges(btn1, false);
        buttonChanges(btn2, false);
        buttonChanges(btn3, false);
        buttonChanges(btn4, false);
        buttonChanges(btn5, false);

        buttonChanges(btn3, true);
        setContent(new EmergencyAppointmentScreen(source, contentArea));
    });

    connect(btn4, &QPushButton::clicked, this, [=](){
        buttonChanges(btn1, false);
        buttonChanges(btn2, false);
        buttonChanges(btn3, false);
        buttonChanges(btn4, false);
        buttonChanges(btn5, false);

        buttonChanges(btn4, true);
        setContent(new RoomManagementScreen(source, contentArea));
    });

    connect(btn5, &QPushButton::clicked, this, [=](){
        buttonChanges(btn1, false);
        buttonChanges(btn2, false);
        buttonChanges(btn3, false);
        buttonChanges(btn4, false);
        buttonChanges(btn5, false);

        buttonChanges(btn5, true);
        setContent(new SuggestDoctorScreen(contentArea));
    });

    sidebar->setStyleSheet("background-color: #263238;");

    contentArea = new QWidget;
    contentLayout = new QVBoxLayout(contentArea);
    contentLayout->setContentsMargins(10, 10, 10, 10);

    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(contentArea, 1);

    btn1->setProperty("selected", true);
    setContent(new PatientScreen(source, this, contentArea));
}

void MainWindow::setContent(QWidget *newPage)
{
    // Remove old widgets
    QLayoutItem *item;
    while ((item = contentLayout->takeAt(0)) != nullptr)
    {
        delete item->widget();
        delete item;
    }
    contentLayout->addWidget(newPage);
}
