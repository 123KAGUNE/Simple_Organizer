#pragma once

#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>
#include "organizer.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void addRule();
    void removeRule();
    void organizeFolder();
    void refreshTable(const QString &filter = QString());

private:
    Organizer *organizer;
    QTableWidget *ruleTable;
    QLineEdit *searchLine;

    void setupUI();
};