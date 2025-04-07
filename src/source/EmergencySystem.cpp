#include <QtSql/QSqlQuery>
#include <source/Source.h>
#include <source/EmergencySystem.h>

EmergencySystem::EmergencySystem(Source *source) : source(source)
{
    head = SLIST_HEAD_INITIALIZER(head);

    // Initialize the patient management system
    QSqlQuery *query = this->source->getQuery();
    query->exec("SELECT emergency_id, patient_id FROM emergency order by emergency_id");
    while (query->next())
    {
        int emergencyId = query->value(0).toInt();
        int patientId = query->value(1).toInt();

        Emergency *newEmergency = new Emergency{emergencyId, patientId, nullptr};
        SLIST_INSERT_HEAD(&head, newEmergency, next);
        qDebug() << "Emergency ID:" << emergencyId << ", Patient ID:" << patientId;
    }
    qDebug() << "Emergencys loaded from database.";
}

EmergencySystem::~EmergencySystem()
{
    // Clean up the linked list of emergencys
    Emergency *tmp1, *tmp2;
    SLIST_FOREACH_SAFE(tmp1, &head, next, tmp2)
    {
        delete tmp1;
    }
    qDebug() << "EmergencySystem destroyed.";
}

void EmergencySystem::scheduleEmergency(int patientId)
{
    // Schedule a new emergency for a patient
    int emergencyId = 0;
    QSqlQuery *query = this->source->getQuery();

    // Check if the patient exists
    query->prepare("SELECT patient_id FROM patient WHERE patient_id = ?");
    query->addBindValue(patientId);
    if (!query->exec())
    {
        qDebug() << "Error checking patient existence:" << query->lastError().text();
        return;
    }
    if (!query->next())
    {
        qDebug() << "Patient ID" << patientId << "does not exist.";
        return;
    }

    query->prepare("INSERT INTO emergency (patient_id) VALUES (?)");
    query->addBindValue(patientId);
    if (!query->exec())
    {
        qDebug() << "Error scheduling emergency:" << query->lastError().text();
        return;
    }

    query->prepare("SELECT last_insert_rowid()");
    if (!query->exec())
    {
        qDebug() << "Error getting last insert ID:" << query->lastError().text();
        return;
    }
    if (query->next())
    {
        emergencyId = query->value(0).toInt();
    }

    Emergency *newEmergency = new Emergency{emergencyId, patientId, nullptr};
    SLIST_INSERT_HEAD(&head, newEmergency, next);
    qDebug() << "Scheduled emergency with ID:" << emergencyId << ", Patient ID:" << patientId;
}

void EmergencySystem::cancelEmergency(int emergencyId)
{
    // Cancel an emergency
    QSqlQuery *query = this->source->getQuery();

    query->prepare("DELETE FROM emergency WHERE emergency_id = ?");
    query->addBindValue(emergencyId);
    if (!query->exec())
    {
        qDebug() << "Error canceling emergency:" << query->lastError().text();
        return;
    }

    Emergency *tmp1, *tmp2;
    SLIST_FOREACH_SAFE(tmp1, &head, next, tmp2)
    {
        if (tmp1->emergencyId == emergencyId)
        {
            SLIST_REMOVE(&head, tmp1, Emergency, next);
            delete tmp1;
            break;
        }
    }
    qDebug() << "Canceled emergency with ID:" << emergencyId;
}

Emergency *EmergencySystem::viewEmergency(int emergencyId) const
{
    // View an emergency's information
    Emergency *current;
    SLIST_FOREACH(current, &head, next)
    {
        if (current->emergencyId == emergencyId)
        {
            return current;
        }
    }
    return nullptr;
}

Emergency *EmergencySystem::listAllEmergencies(Emergency *current) const
{
    // Get the next emergency in the fifo
    // Yield logic
    if (current == nullptr)
    {
        return SLIST_FIRST(&head);
    }
    return SLIST_NEXT(current, next);
}

Emergency *EmergencySystem::getNextEmergency()
{
    // Get the next emergency in the fifo
    return SLIST_FIRST(&head);
}
