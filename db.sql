-- patient table
-- appointment table
-- emergeny table
-- room table
-- medical history table

CREATE TABLE IF NOT EXISTS patient  (
    patient_id INTEGER PRIMARY KEY AUTOINCREMENT,
    name VARCHAR(50) NOT NULL,
    age INT NOT NULL,
    symptom VARCHAR(255) NOT NULL,
    severity INT NOT NULL,
    doctor VARCHAR(50) NOT NULL
);

CREATE TABLE IF NOT EXISTS appointment (
    appointment_id INTEGER PRIMARY KEY AUTOINCREMENT,
    patient_id INT NOT NULL,
    FOREIGN KEY (patient_id) REFERENCES patient(patient_id)
);

CREATE TABLE IF NOT EXISTS emergency (
    emergency_id INTEGER PRIMARY KEY AUTOINCREMENT,
    patient_id INT NOT NULL,
    FOREIGN KEY (patient_id) REFERENCES patient(patient_id)
);

CREATE TABLE IF NOT EXISTS room (
    room_id INTEGER PRIMARY KEY AUTOINCREMENT,
    room_number VARCHAR(10) NOT NULL,
    occupied BOOLEAN NOT NULL DEFAULT FALSE,
    patient_id INT,
    FOREIGN KEY (patient_id) REFERENCES patient(patient_id)
);

CREATE TABLE medical_history (
    history_id INTEGER PRIMARY KEY AUTOINCREMENT,
    patient_id INT NOT NULL,
    FOREIGN KEY (patient_id) REFERENCES patient(patient_id),
    diagnosis VARCHAR(255) NOT NULL,
    treatment VARCHAR(255) NOT NULL,
    date DATE NOT NULL
);
