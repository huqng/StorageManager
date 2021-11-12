#include "MainWindow.h"

#include "DlgCheckIn.h"
#include "DlgCheckOut.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    this->resize(1000, 600);

    ui.btnCheckIn->setDisabled(true);
    ui.btnCheckOut->setDisabled(true);
    ui.btnDeleteItem->setDisabled(true);
    ui.tableWidget->setSelectionBehavior(QTableWidget::SelectRows);
    ui.tableWidget->setEditTriggers(QTableWidget::NoEditTriggers);
    ui.tableWidget->setSelectionMode(QTableWidget::SingleSelection);

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

    connect(ui.btnCheckIn, &QPushButton::clicked, this, &MainWindow::slotOnCheckInClicked);
    connect(ui.btnCheckOut, &QPushButton::clicked, this, &MainWindow::slotOnCheckOutClicked);
    connect(ui.btnSearch, &QPushButton::clicked, this, &MainWindow::slotOnSearchClicked);
    connect(ui.btnAddNewItem, &QPushButton::clicked, this, &MainWindow::slotOnAddNewItemClicked);
    connect(ui.btnDeleteItem, &QPushButton::clicked, this, &MainWindow::slotOnDeleteItemClicked);
    connect(ui.tableWidget, &QTableWidget::itemSelectionChanged, this, &MainWindow::slotOnItemSelected);
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
    ui.tableWidget->clearContents();

    QString strNameLike = ui.lineEdit->text();
    QList<QStringList> listResult =  m_mgr.Select(strNameLike);
    ui.tableWidget->setRowCount(listResult.at(0).size());
    
    for (int i = 0; i < listResult.size(); i++)
    {
        QStringList strlistCol = listResult.at(i);
        for (int j = 0; j < strlistCol.size(); j++)
        {
            ui.tableWidget->setItem(j, i, new QTableWidgetItem(strlistCol.at(j)));
        }
    }   
}

void MainWindow::slotOnAddNewItemClicked()
{

}

void MainWindow::slotOnDeleteItemClicked()
{

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
