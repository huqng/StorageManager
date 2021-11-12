#pragma once

#include "ui_MainWindow.h"

#include "StorageManager.h"

#include <QtWidgets/QMainWindow>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);
    virtual ~MainWindow() override;

private:
    Ui::MainWindowClass ui;
    CStorageManager m_mgr;

public slots:
    void slotOnCheckInClicked();
    void slotOnCheckOutClicked();
    void slotOnSearchClicked();
    void slotOnAddNewItemClicked();
    void slotOnDeleteItemClicked();

    void slotOnItemSelected();
};
