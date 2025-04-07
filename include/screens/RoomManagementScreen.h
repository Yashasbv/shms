#pragma once
#include <QWidget>
#include <QTableWidget>

class Source;

class RoomManagementScreen : public QWidget
{
    Q_OBJECT
private:
    Source *source = nullptr;
    QTableWidget *table;

    void refreshTable();

private slots:
    void exportRoomManagement();
    void onRoomAssign();
    void onRoomEvacuate();

public:
    RoomManagementScreen(Source *source, QWidget *parent = nullptr);
};
