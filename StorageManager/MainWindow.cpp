#include "MainWindow.h"

#include "DlgCheckIn.h"
#include "DlgCheckOut.h"
#include "DlgAddItem.h";

#include <qmessagebox.h>
#include <qsettings.h>
#include <qdebug.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    this->resize(1280, 720);
    this->setMinimumSize(1000, 600);

    /* buttons */
    ui.btnCheckIn->setDisabled(true);
    ui.btnCheckOut->setDisabled(true);
    ui.btnDeleteItem->setDisabled(true);
    ui.tableWidget->setSelectionBehavior(QTableWidget::SelectRows);
    ui.tableWidget->setEditTriggers(QTableWidget::NoEditTriggers);
    ui.tableWidget->setSelectionMode(QTableWidget::SingleSelection);

    /* headers */
    QStringList strlistColNames;
    strlistColNames.append("ItemID");
    strlistColNames.append("ItemName");
    strlistColNames.append("ItemQuantity");
    strlistColNames.append("ItemUnit");
    strlistColNames.append("ItemPosition1");
    strlistColNames.append("ItemPosition1");
    strlistColNames.append("ItemPosition1");
    strlistColNames.append("Comments");    
    m_mgr.SetColNames(strlistColNames);
    QSettings iniReader("./config.ini", QSettings::Format::IniFormat);
    iniReader.beginGroup("File");
    m_mgr.SetFilename(iniReader.value("Filename").toString());
    iniReader.endGroup();

    /* read ini */
    iniReader.beginGroup("Position");
    QStringList strlistPositions = iniReader.allKeys();
    for (QString strPosition : strlistPositions)
    {
        QStringList strlistPosition2 = iniReader.value(strPosition).toStringList();
        m_mapPositions[strPosition] = strlistPosition2;
    }
    iniReader.endGroup();

    /* combobox search by */
    ui.cbSearchBy->addItems(strlistColNames);

    /* connection */
    connect(ui.btnCheckIn, &QPushButton::clicked, this, &MainWindow::slotOnCheckInClicked);
    connect(ui.btnCheckOut, &QPushButton::clicked, this, &MainWindow::slotOnCheckOutClicked);
    connect(ui.btnSearch, &QPushButton::clicked, this, &MainWindow::slotOnSearchClicked);
    connect(ui.btnAddNewItem, &QPushButton::clicked, this, &MainWindow::slotOnAddNewItemClicked);
    connect(ui.btnDeleteItem, &QPushButton::clicked, this, &MainWindow::slotOnDeleteItemClicked);
    connect(ui.tableWidget, &QTableWidget::itemSelectionChanged, this, &MainWindow::slotOnItemSelected);
    connect(ui.btnReset, &QPushButton::clicked, this, &MainWindow::slotOnResetClicked);
}

MainWindow::~MainWindow()
{

}

void MainWindow::slotOnCheckInClicked()
{
    int iRow = ui.tableWidget->selectedItems().at(0)->row();
    
    QString strItemID = ui.tableWidget->item(iRow, 0)->text();
    QString strItemName = ui.tableWidget->item(iRow, 1)->text();
    int iQuantity = ui.tableWidget->item(iRow, 2)->text().toInt();
    QString strItemUnit = ui.tableWidget->item(iRow, 3)->text();
    QString strItemPosition1 = ui.tableWidget->item(iRow, 4)->text();
    QString strItemPosition2 = ui.tableWidget->item(iRow, 5)->text();
    QString strItemPosition3 = ui.tableWidget->item(iRow, 6)->text();
    QString strComments = ui.tableWidget->item(iRow, 7)->text();


    DlgCheckIn dlg(strItemID, strItemName, iQuantity, strItemPosition1, strItemPosition2, strItemPosition3, strItemUnit, strComments);
    if (dlg.exec() == QDialog::Accepted)
    {
        int iQuantityCheckIn = dlg.GetQuantityCheckIn();
        int iNewQuantity = iQuantity + iQuantityCheckIn;
        m_mgr.Update(strItemID, iNewQuantity);
        ui.tableWidget->item(iRow, 2)->setText(QString::number(iNewQuantity));
    }
}

void MainWindow::slotOnCheckOutClicked()
{
    int iRow = ui.tableWidget->selectedItems().at(0)->row();

    QString strItemID = ui.tableWidget->item(iRow, 0)->text();
    QString strItemName = ui.tableWidget->item(iRow, 1)->text();
    int iQuantity = ui.tableWidget->item(iRow, 2)->text().toInt();
    QString strItemUnit = ui.tableWidget->item(iRow, 3)->text();
    QString strItemPosition1 = ui.tableWidget->item(iRow, 4)->text();
    QString strItemPosition2 = ui.tableWidget->item(iRow, 5)->text();
    QString strItemPosition3 = ui.tableWidget->item(iRow, 6)->text();
    QString strComments = ui.tableWidget->item(iRow, 7)->text();


    DlgCheckOut dlg(strItemID, strItemName, iQuantity, strItemPosition1, strItemPosition2, strItemPosition3, strItemUnit, strComments);
    if (dlg.exec() == QDialog::Accepted)
    {
        int iQuantityCheckOut = dlg.GetQuantityCheckOut();
        int iNewQuantity = iQuantity - iQuantityCheckOut;
        m_mgr.Update(strItemID, iNewQuantity);
        ui.tableWidget->item(iRow, 2)->setText(QString::number(iNewQuantity));
    }
}

void MainWindow::slotOnSearchClicked()
{
    ui.tableWidget->setSortingEnabled(false);
    ui.tableWidget->clearContents();
    
    QString strNameLike = ui.lineEdit->text();
    /* get data */
    QList<QStringList> listResult =  m_mgr.SelectByColumnLike(ui.cbSearchBy->currentText(), strNameLike);
    ui.tableWidget->setRowCount(listResult.at(0).size());
    
    /* display */
    for (int i = 0; i < listResult.size(); i++)
    {
        QStringList strlistCol = listResult.at(i);
        for (int j = 0; j < strlistCol.size(); j++)
        {
            ui.tableWidget->setItem(j, i, new QTableWidgetItem(strlistCol.at(j)));
        }
    }
    ui.tableWidget->setSortingEnabled(true);
}

void MainWindow::slotOnAddNewItemClicked()
{
    /* get 7 parameters */
    DlgAddItem dlg;
    dlg.SetPositionOptions(m_mapPositions);
    if (dlg.exec() == QDialog::Accepted)
    {
        QString iItemID = dlg.GetItemID();
        QString strItemName = dlg.GetItemName();
        int iItemQuantity = dlg.GetItemQuantity();;
        QString strItemUnit = dlg.GetItemUnit();
        QString strItemPosition1 = dlg.GetItemPosition1();
        QString strItemPosition2 = dlg.GetItemPosition2();
        QString strItemPosition3 = dlg.GetItemPosition3();;
        QString strComments = dlg.GetComments();

        m_mgr.Insert(iItemID, strItemName, iItemQuantity, strItemUnit, strItemPosition1, strItemPosition2, strItemPosition3, strComments);
    }
}

void MainWindow::slotOnDeleteItemClicked()
{
    int iRow = ui.tableWidget->selectedItems().at(0)->row();

    QString strItemID = ui.tableWidget->item(iRow, 0)->text();
    QString strItemName = ui.tableWidget->item(iRow, 1)->text();
    int iQuantity = ui.tableWidget->item(iRow, 2)->text().toInt();
    QString strItemUnit = ui.tableWidget->item(iRow, 3)->text();
    QString strItemPosition1 = ui.tableWidget->item(iRow, 4)->text();
    QString strItemPosition2 = ui.tableWidget->item(iRow, 5)->text();
    QString strItemPosition3 = ui.tableWidget->item(iRow, 6)->text();
    QString strComments = ui.tableWidget->item(iRow, 7)->text();

    QString strNotice = QString(u8"È·ÈÏÉ¾³ý [").append(strItemID).append("] ").append(strItemName).append(u8"£¿");

    QMessageBox qmb;
    qmb.addButton(QMessageBox::Ok);
    qmb.addButton(QMessageBox::Cancel);
    qmb.setText(strNotice);


    if (qmb.exec() == QMessageBox::Ok)
    {
        m_mgr.Delete(strItemID);
        ui.tableWidget->removeRow(iRow);
    }
    else
    {

    }
}

void MainWindow::slotOnItemSelected()
{
    if (ui.tableWidget->selectedItems().size() == 0)
    {
        ui.tableWidget->clearSelection();
        ui.btnCheckIn->setDisabled(true);
        ui.btnCheckOut->setDisabled(true);
        ui.btnDeleteItem->setDisabled(true);        
    }
    else
    {
        ui.btnCheckIn->setEnabled(true);
        ui.btnCheckOut->setEnabled(true);
        ui.btnDeleteItem->setEnabled(true);
    }
}

void MainWindow::slotOnResetClicked()
{
    ui.cbSearchBy->setCurrentIndex(0);
    ui.lineEdit->clear();
    ui.tableWidget->setRowCount(0);
    ui.tableWidget->clearContents();
}
