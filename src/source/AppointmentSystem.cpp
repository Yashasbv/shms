#include <QtSql/QSqlQuery>
#include <source/Source.h>
#include <source/AppointmentSystem.h>

AppointmentSystem::AppointmentSystem(Source *source) : source(source)
{
    head = SLIST_HEAD_INITIALIZER(head);

    // Initialize the patient management system
    QSqlQuery *query = this->source->getQuery();
    query->exec("SELECT appointment_id, patient_id FROM appointment order by appointment_id");
    while (query->next())
    {
        int appointmentId = query->value(0).toInt();
        int patientId = query->value(1).toInt();

        Appointment *newAppointment = new Appointment{appointmentId, patientId, nullptr};
        SLIST_INSERT_TAIL(&head, newAppointment, Appointment, next);
        qDebug() << "Appointment ID:" << appointmentId << ", Patient ID:" << patientId;
    }
    qDebug() << "Appointments loaded from database.";
}

AppointmentSystem::~AppointmentSystem()
{
    // Clean up the linked list of appointments
    Appointment *tmp1, *tmp2;
    SLIST_FOREACH_SAFE(tmp1, &head, next, tmp2)
    {
        delete tmp1;
    }
    qDebug() << "AppointmentSystem destroyed.";
}

void AppointmentSystem::scheduleAppointment(int patientId)
{
    // Schedule a new appointment for a patient
    int appointmentId = 0;
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

    query->prepare("INSERT INTO appointment (patient_id) VALUES (?)");
    query->addBindValue(patientId);
    if (!query->exec())
    {
        qDebug() << "Error scheduling appointment:" << query->lastError().text();
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
        appointmentId = query->value(0).toInt();
    }

    Appointment *newAppointment = new Appointment{appointmentId, patientId, nullptr};
    SLIST_INSERT_TAIL(&head, newAppointment, Appointment, next);
    qDebug() << "Scheduled appointment with ID:" << appointmentId << ", Patient ID:" << patientId;
}

void AppointmentSystem::cancelAppointment(int appointmentId)
{
    // Cancel an appointment
    QSqlQuery *query = this->source->getQuery();

    query->prepare("DELETE FROM appointment WHERE appointment_id = ?");
    query->addBindValue(appointmentId);
    if (!query->exec())
    {
        qDebug() << "Error canceling appointment:" << query->lastError().text();
        return;
    }

    Appointment *tmp1, *tmp2;
    SLIST_FOREACH_SAFE(tmp1, &head, next, tmp2)
    {
        if (tmp1->appointmentId == appointmentId)
        {
            SLIST_REMOVE(&head, tmp1, Appointment, next);
            delete tmp1;
            break;
        }
    }
    qDebug() << "Canceled appointment with ID:" << appointmentId;
}

Appointment *AppointmentSystem::viewAppointment(int appointmentId) const
{
    // View an appointment's information
    Appointment *current;
    SLIST_FOREACH(current, &head, next)
    {
        if (current->appointmentId == appointmentId)
        {
            return current;
        }
    }
    return nullptr;
}

Appointment *AppointmentSystem::listAllAppointments(Appointment *current) const
{
    // Get the next appointment in the fifo
    // Yield logic
    if (current == nullptr)
    {
        return SLIST_FIRST(&head);
    }
    return SLIST_NEXT(current, next);
}

Appointment *AppointmentSystem::getNextAppointment()
{
    // Get the next appointment in the fifo
    return SLIST_FIRST(&head);
}
