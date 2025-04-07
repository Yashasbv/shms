#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QHeaderView>
#include <QFileDialog>
#include <QFile>

#include <source/source.h>
#include <source/PatientManagementSystem.h>
#include <screens/dialog/RoomManagementDialog.h>
#include <screens/RoomManagementScreen.h>

RoomManagementScreen::RoomManagementScreen(Source *source, QWidget *parent) : source(source), QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    QHBoxLayout *topBarLayout = new QHBoxLayout;

    QLabel *patientInfo = new QLabel("Room Management Details");
    patientInfo->setStyleSheet("font-weight: bold; font-size: 16px;");
    topBarLayout->addWidget(patientInfo);

    topBarLayout->addStretch();

    QPushButton *addBtn = new QPushButton;
    QIcon addIcon = style()->standardIcon(QStyle::SP_FileDialogNewFolder); // You can use a custom "+" icon
    addBtn->setIcon(addIcon);
    addBtn->setToolTip("Book room");
    addBtn->setFlat(true);
    topBarLayout->addWidget(addBtn);

    // Export icon button
    QPushButton *exportBtn = new QPushButton;
    QIcon exportIcon = style()->standardIcon(QStyle::SP_DialogSaveButton); // Looks like a save/download icon
    exportBtn->setIcon(exportIcon);
    exportBtn->setToolTip("Export as CSV");
    exportBtn->setFlat(true);
    topBarLayout->addWidget(exportBtn);

    table = new QTableWidget(0, 4);
    table->setHorizontalHeaderLabels({"Room ID", "Patient Id", "Patient Name", "Evacuate"});
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    table->verticalHeader()->setVisible(false);

    layout->addLayout(topBarLayout);
    layout->addWidget(table);

    refreshTable();

    connect(exportBtn, &QPushButton::clicked, this, &RoomManagementScreen::exportRoomManagement);
    connect(addBtn, &QPushButton::clicked, this, &RoomManagementScreen::onRoomAssign);
}

void RoomManagementScreen::exportRoomManagement()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Export Room Management"), "test.csv", tr("CSV Files (*.csv)"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << "Room ID,Patient Id,Patient Name\n";

    Room *rooms = source->roomManagementSystem->getRoomDetails();
    for (int i = 0; i < MAX_ROOMS; ++i)
    {
        if (rooms[i].occupied)
        {
            Patient *patient = source->patientManagementSystem->getPatient(rooms[i].patientId);
            out << i + 1 << "," << rooms[i].patientId << "," << patient->name << "\n";
        }
    }

    file.close();
}

void RoomManagementScreen::onRoomAssign()
{
    RoomManagementDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted)
    {
        Room a = dialog.getFields();
        source->roomManagementSystem->assignRoom(a.patientId);
        refreshTable();
    }
}

void RoomManagementScreen::onRoomEvacuate()
{
    int row = table->currentRow();
    if (row < 0)
        return;
    int id = table->item(row, 0)->text().toInt() - 1;
    source->roomManagementSystem->evacuateRoom(id);
    refreshTable();
}

void RoomManagementScreen::refreshTable()
{
    int index = 0;
    table->setRowCount(0);

    Room *rooms = source->roomManagementSystem->getRoomDetails();
    for (int i = 0; i < MAX_ROOMS; ++i)
    {
        if (rooms[i].occupied)
        {
            table->insertRow(index);
            table->setItem(index, 0, new QTableWidgetItem(QString::number(i + 1)));
            table->setItem(index, 1, new QTableWidgetItem(QString::number(rooms[i].patientId)));

            Patient *patient = source->patientManagementSystem->getPatient(rooms[i].patientId);
            if (patient)
            {
                table->setItem(index, 2, new QTableWidgetItem(patient->name));
            }

            // Delete icon button
            QPushButton *deleteBtn = new QPushButton;
            QIcon deleteIcon = style()->standardIcon(QStyle::SP_TrashIcon);
            deleteBtn->setIcon(deleteIcon);
            deleteBtn->setFlat(true);
            table->setCellWidget(index, 3, deleteBtn);

            index++;

            connect(deleteBtn, &QPushButton::clicked, this, &RoomManagementScreen::onRoomEvacuate);
        }
    }
}
