#pragma once
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QIcon>
#include <QSizePolicy>

class Source;

class MainWindow : public QWidget {
    Q_OBJECT
private:
    Source *source = nullptr;
    QLayout *contentLayout;
    
public:
    QWidget *contentArea;

    MainWindow(QWidget *parent = nullptr);
    void setContent(QWidget *newPage);
};
