#include "StorageManager.h"

#include <qsqlerror.h>
#include <qsqlquery.h>
#include <qsqlrecord.h>
#include <qsqlfield.h>
#include <qmessagebox.h>

MdbControl::MdbControl(QString strFilename)
{
    m_db = QSqlDatabase::addDatabase("QODBC");//�������ݿ�����
    QString dsn = QString("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)}; FIL={MS Access}; DBQ=%1;").arg(strFilename);//�����ַ���
    m_db.setDatabaseName(dsn);    //���������ַ���
    m_db.setUserName("");         //���õ�½���ݿ���û���
    m_db.setPassword("");         //��������
    m_db.open();
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
    }
}




CStorageManager::CStorageManager()
{
    bool ok = m_MdbControl.IsOpen();
    if (!ok) {
        QString strLastError = m_MdbControl.GetLastError();
        QMessageBox messageBox;
        messageBox.setText(QString("Database error: \r\n").append(strLastError));
        messageBox.exec();
        m_MdbControl.Close();
    }
    else
    {

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
