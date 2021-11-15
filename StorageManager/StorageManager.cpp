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

QList<QStringList> MdbControl::SelectByColumnLike(QString strColName, QString strSearchCondition)
{
    QSqlQuery q(m_db);
    bool ok = q.exec(QString("SELECT * FROM Storage WHERE ").append(strColName).append(" LIKE '%") + strSearchCondition + "%'; ");

    QSqlRecord rec = q.record();
    int iColCnt = rec.count();

    QList<QStringList> listQueryResult;
    for (int i = 0; i < iColCnt; i++)
    {
        listQueryResult.append(QStringList());
    }

    while (q.next())
    {
        for (int i = 0; i < iColCnt; i++)
        {
            QVariant var = q.value(i);
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
    QString strCmd = QString("DELETE FROM Storage WHERE ItemID = '").append(strItemID).append("'");
    bool ok = q.exec(strCmd);
    if (!ok)
    {
        QString strLastError = q.lastError().text();
        abort();
    }
}

void MdbControl::Insert(QString strItemID, QString strItemName, int iItemQuantity, QString strItemUnit, QString strItemPosition1, QString strItemPosition2, QString strItemPosition3, QString strComments)
{
    QSqlQuery q(m_db);
    QString strCmd = QString("INSERT INTO Storage VALUES('")\
        .append(strItemID).append("', '")\
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

bool MdbControl::ExistID(const QString& strID)
{
    QSqlQuery q(m_db);
    bool ok = q.exec(QString("SELECT count(*) FROM Storage WHERE ItemID = '") + strID + "';");
    q.next();
    if (q.value(0).toInt() > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
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

QList<QStringList> CStorageManager::SelectByColumnLike(QString strColName, QString strNameLike)
{
    m_strLastQuery = strNameLike;
    return m_MdbControl.SelectByColumnLike(strColName, strNameLike);
}

void CStorageManager::Update(QString strItemID, int iNewQuantity)
{
    return m_MdbControl.Update(strItemID, iNewQuantity);
}

void CStorageManager::Delete(QString strItemID)
{
    return m_MdbControl.Delete(strItemID);
}

void CStorageManager::Insert(QString strItemID, QString strItemName, int iItemQuantity, QString strItemUnit, QString strItemPosition1, QString strItemPosition2, QString strItemPosition3, QString strComments)
{
    // SELECT MAX(ItemID) from Storage
    if (m_MdbControl.ExistID(strItemID))
    {
        QMessageBox messageBox;
        messageBox.setText(QString(u8"错误：ID ").append(strItemID).append(u8" 已存在"));
        messageBox.exec();
    }
    else
    {       
        m_MdbControl.Insert(strItemID, strItemName, iItemQuantity, strItemUnit, strItemPosition1, strItemPosition2, strItemPosition3, strComments);
    }
}

int CStorageManager::GetNextID()
{
    return 1 + m_MdbControl.GetMaxID();
}
