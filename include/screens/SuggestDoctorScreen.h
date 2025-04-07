#pragma once
#include <QWidget>
#include <QLabel>
#include <QLineEdit>

class Source;

class SuggestDoctorScreen : public QWidget
{
    Q_OBJECT

private:
    QLineEdit *symptomInput;
    QLabel *resultLabel;

private slots:
    void onSuggestClicked();

public:
    SuggestDoctorScreen(QWidget *parent = nullptr);
};
