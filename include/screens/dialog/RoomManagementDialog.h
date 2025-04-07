#pragma once

#include <QDialog>
#include <QLineEdit>
#include <source/RoomManagementSystem.h>

class RoomManagementDialog : public QDialog
{
    Q_OBJECT

    QLineEdit *patientIdInput;

public:
    RoomManagementDialog(QWidget *parent = nullptr);
    Room getFields() const;
};
