#include <QtSql/QSqlQuery>
#include <source/Source.h>
#include <source/RoomManagementSystem.h>

RoomManagementSystem::RoomManagementSystem(Source *source) : source(source)
{
    // Initialize all rooms to unoccupied
    for (int i = 0; i < MAX_ROOMS; ++i)
    {
        rooms[i].occupied = false;
        rooms[i].patientId = -1; // -1 indicates no patient assigned
    }

    // Load room data from the database
    QSqlQuery *query = this->source->getQuery();
    query->exec("SELECT room_number, occupied, patient_id FROM room");
    while (query->next())
    {
        int roomNumber = query->value(0).toInt();
        bool occupied = query->value(1).toBool();
        int patientId = query->value(2).toInt();

        if (roomNumber >= 0 && roomNumber < MAX_ROOMS)
        {
            rooms[roomNumber].occupied = occupied;
            rooms[roomNumber].patientId = patientId;
            qDebug() << "Room" << roomNumber << "loaded with patient ID:" << patientId;
        }
    }
}

RoomManagementSystem::~RoomManagementSystem()
{
}

bool RoomManagementSystem::assignRoom(int patientId)
{
    QSqlQuery *query = this->source->getQuery();

    // Check if the patient exists
    query->prepare("SELECT patient_id FROM patient WHERE patient_id = ?");
    query->addBindValue(patientId);
    if (!query->exec())
    {
        qDebug() << "Error checking patient existence:" << query->lastError().text();
        return false;
    }
    if (!query->next())
    {
        qDebug() << "Patient ID" << patientId << "does not exist.";
        return false;
    }

    // Find the first unoccupied room
    for (int i = 0; i < MAX_ROOMS; ++i)
    {
        if (!rooms[i].occupied)
        {
            rooms[i].occupied = true;
            rooms[i].patientId = patientId;

            // Update the database
            QSqlQuery *query = this->source->getQuery();
            query->prepare("INSERT INTO room (room_number, occupied, patient_id) VALUES (?, ?, ?)");
            query->addBindValue(i);
            query->addBindValue(true);
            query->addBindValue(patientId);
            if (!query->exec())
            {
                qDebug() << "Error assigning room:" << query->lastError().text();
                return false;
            }
            return true;
        }
    }
    // If no room is available, handle the situation (e.g., log an error or notify the user)
    qDebug() << "No available rooms for patient ID:" << patientId;
    return false;
}

void RoomManagementSystem::evacuateRoom(int roomNumber)
{
    if (roomNumber < 0 || roomNumber >= MAX_ROOMS)
    {
        // Invalid room number or patient ID does not match
        qDebug() << "Invalid evacuation request for room:" << roomNumber;
        return;
    }

    // Evacuate the room
    rooms[roomNumber].occupied = false;
    rooms[roomNumber].patientId = -1; // Reset patient ID

    // Update the database
    QSqlQuery *query = this->source->getQuery();
    query->prepare("DELETE FROM room WHERE room_number = ?");
    query->addBindValue(roomNumber);
    if (!query->exec())
    {
        qDebug() << "Error evacuating room:" << query->lastError().text();
        return;
    }
}

int RoomManagementSystem::getRoomNumber(int patientId) const
{
    for (int i = 0; i < MAX_ROOMS; ++i)
    {
        if (rooms[i].patientId == patientId)
        {
            qDebug() << "Room number for patient ID:" << patientId << "is" << i;
            return i;
        }
    }
    qDebug() << "No room found for patient ID:" << patientId;
    return -1;
}

Room *RoomManagementSystem::getRoomDetails()
{
    return rooms;
}
