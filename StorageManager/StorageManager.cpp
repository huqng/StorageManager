#include "StorageManager.h"

#include <qsqlerror.h>
#include <qsqlquery.h>
#include <qsqlrecord.h>
#include <qsqlfield.h>
#include <qmessagebox.h>

MdbControl::MdbControl()
{
}

bool MdbControl::IsOpen()
{
    return m_db.isOpen();
}

QString MdbControl::GetLastError()
{
    return m_db.lastError().text();
}

void MdbControl::Close()
{
    m_db.close();
}

void MdbControl::SetFilename(QString strFilename)
{
    m_db = QSqlDatabase::addDatabase("QODBC");//设置数据库驱动
    QString dsn = QString("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)}; FIL={MS Access}; DBQ=%1;").arg(strFilename);//连接字符串
    m_db.setDatabaseName(dsn);    //设置连接字符串
    m_db.setUserName("");         //设置登陆数据库的用户名
    m_db.setPassword("");         //设置密码
    m_db.open();    
}

QList<QStringList> MdbControl::Select(QString strSearchCondition)
{
    QSqlQuery q(m_db);
    bool ok = q.exec(QString("SELECT * FROM Storage WHERE ItemName LIKE '%") + strSearchCondition + "%';");

    QSqlRecord rec = q.record();
    int iColCnt = rec.count();

    QList<QStringList> listQueryResult;
    for (int i = 0; i < iColCnt; i++)
    {
        listQueryResult.append(QStringList());
    }

    while (q.next())
    {
        QSqlRecord rec = q.record();
        for (int i = 0; i < iColCnt; i++)
        {
            QVariant var = rec.value(i);
            if (var.type() == QMetaType::Int)
            {
                listQueryResult[i].append(QString::number(var.toInt()));
            }
            else
            {
                listQueryResult[i].append(var.toString());
            }
        }

    }

    return listQueryResult;
}

void MdbControl::Update(QString strItemID, int iNewQuantity)
{
    QSqlQuery q(m_db);
    QString strCmd = QString("UPDATE Storage SET ItemQuantity = ") + QString::number(iNewQuantity) + " WHERE ItemID = " + strItemID + "";
    bool ok = q.exec(strCmd);
    if (!ok)
    {
        QString strLastError = q.lastError().text();
        abort();
    }
}

void MdbControl::Delete(QString strItemID)
{
    QSqlQuery q(m_db);
    QString strCmd = QString("DELETE FROM Storage WHERE ItemID = ").append(strItemID);
    bool ok = q.exec(strCmd);
    if (!ok)
    {
        QString strLastError = q.lastError().text();
        abort();
    }
}

void MdbControl::Insert(int iItemID, QString strItemName, int iItemQuantity, QString strItemUnit, QString strItemPosition1, QString strItemPosition2, QString strItemPosition3, QString strComments)
{
    QSqlQuery q(m_db);
    QString strCmd = QString("INSERT INTO Storage VALUES(")\
        .append(QString::number(iItemID)).append(", '")\
        .append(strItemName).append("', ")\
        .append(QString::number(iItemQuantity)).append(", '")\
        .append(strItemUnit).append("', '")\
        .append(strItemPosition1).append("', '")\
        .append(strItemPosition2).append("', '")\
        .append(strItemPosition3).append("', '")\
        .append(strComments).append("')");
    bool ok = q.exec(strCmd);
    if (!ok)
    {
        QString strLastError = q.lastError().text();
        abort();
    }
}

int MdbControl::GetMaxID()
{
    QSqlQuery q(m_db);
    bool ok = q.exec(QString("SELECT MAX(ItemID) FROM Storage"));
    q.next();
    return q.record().value(0).toInt();
}




CStorageManager::CStorageManager()
{

}

void CStorageManager::SetFilename(QString strFilename)
{
    m_MdbControl.SetFilename(strFilename);
    if (!m_MdbControl.IsOpen())
    {
        QString strLastError = m_MdbControl.GetLastError();
        QMessageBox messageBox;
        messageBox.setText(QString("Database error: \r\n").append(strLastError));
        messageBox.exec();
        exit(-1);
    }
}

void CStorageManager::SetColNames(QStringList strlistNames)
{
    m_strlistColNames = strlistNames;
}

QList<QStringList> CStorageManager::Select(QString strNameLike)
{
    m_strLastQuery = strNameLike;
    return m_MdbControl.Select(strNameLike);
}

void CStorageManager::Update(QString strItemID, int iNewQuantity)
{
    return m_MdbControl.Update(strItemID, iNewQuantity);
}

void CStorageManager::Delete(QString strItemID)
{
    return m_MdbControl.Delete(strItemID);
}

void CStorageManager::Insert(int iItemID, QString strItemName, int iItemQuantity, QString strItemUnit, QString strItemPosition1, QString strItemPosition2, QString strItemPosition3, QString strComments)
{
    // SELECT MAX(ItemID) from Storage
    return m_MdbControl.Insert(iItemID, strItemName, iItemQuantity, strItemUnit, strItemPosition1, strItemPosition2, strItemPosition3, strComments);
}

int CStorageManager::GetNextID()
{
    return 1 + m_MdbControl.GetMaxID();
}
