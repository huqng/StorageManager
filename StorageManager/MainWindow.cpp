#include "MainWindow.h"

#include "DlgCheckIn.h"
#include "DlgCheckOut.h"
#include "DlgAddItem.h"

#include <qmessagebox.h>
#include <qsettings.h>
#include <qdebug.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    this->resize(1280, 720);
    this->setMinimumSize(1000, 600);
    this->setWindowTitle(u8"将就着用用的仓储管理系统");
    this->setWindowIcon(QIcon(":/new/prefix1/icon.ico"));

    /* buttons */
    ui.btnCheckIn->setDisabled(true);
    ui.btnCheckOut->setDisabled(true);
    ui.btnDeleteItem->setDisabled(true);
    ui.tableWidget->setSelectionBehavior(QTableWidget::SelectRows);
    ui.tableWidget->setEditTriggers(QTableWidget::NoEditTriggers);
    ui.tableWidget->setSelectionMode(QTableWidget::SingleSelection);

    /* headers */
    QStringList strlistColNames = {
        "ItemID",
        "ItemName",
        "ItemQuantity",
        "ItemUnit",
        "ItemPosition1",
        "ItemPosition2",
        "ItemPosition3",
        "Comments" };
    QStringList strlistColNamesCn = {
        u8"编号",
        u8"名称",
        u8"数量",
        u8"单位",
        u8"位置1",
        u8"位置2",
        u8"位置3",
        u8"备注"
    };
    m_mgr.SetColNames(strlistColNames);
    ui.cbSearchBy->addItems(strlistColNamesCn);

    m_mapCn2Eng[u8"编号"] = "ItemID";
    m_mapCn2Eng[u8"名称"] = "ItemName";
    m_mapCn2Eng[u8"数量"] = "ItemQuantity";
    m_mapCn2Eng[u8"单位"] = "ItemUnit";
    m_mapCn2Eng[u8"位置1"] = "ItemPosition1";
    m_mapCn2Eng[u8"位置2"] = "ItemPosition2";
    m_mapCn2Eng[u8"位置3"] = "ItemPosition3";
    m_mapCn2Eng[u8"备注"] = "Comments";
        

    /* combobox search by */

    QSettings iniReader("./data/config.ini", QSettings::Format::IniFormat);
    iniReader.beginGroup("File");
    QString strPath = iniReader.value("Filename").toString();
    m_mgr.OpenDataBase(strPath);
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
    QList<QStringList> listResult = m_mgr.SelectByColumnLike(m_mapCn2Eng[ui.cbSearchBy->currentText()], strNameLike);
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

    QString strNotice = QString(u8"确认删除 [").append(strItemID).append("] ").append(strItemName).append(u8"？");

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
