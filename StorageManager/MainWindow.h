#pragma once

#include "ui_MainWindow.h"

#include "StorageManager.h"

#include <QtWidgets/QMainWindow>
#include <qlist.h>
#include <qmap.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);
    virtual ~MainWindow() override;

private:
    Ui::MainWindowClass ui;
    CStorageManager m_mgr;
    QMap<QString, QStringList> m_mapPositions;
    QMap<QString, QString> m_mapCn2Eng;

public slots:
    void slotOnCheckInClicked();
    void slotOnCheckOutClicked();
    void slotOnSearchClicked();
    void slotOnAddNewItemClicked();
    void slotOnDeleteItemClicked();
    void slotOnResetClicked();

    void slotOnItemSelected();
};
