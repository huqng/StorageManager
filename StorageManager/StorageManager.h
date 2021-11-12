#pragma once

#include <qstring.h>
#include <qsqldatabase.h>
#include <qlist.h>

class MdbControl
{
private:
    QSqlDatabase m_db;
public:
    MdbControl(QString strFilename = "C:\\my\\test.mdb");
    bool IsOpen();
    QString GetLastError();
    void Close();

    QList<QStringList> Select(QString strNameLike);
    void Update(QString strItemID, int iNewQuantity);
    void Delete(QString strItemID);

};

class CStorageManager
{
private:
    MdbControl m_MdbControl;
    QStringList m_strlistColNames;
    QString m_strLastQuery;
public:
    CStorageManager();
    void SetColNames(QStringList strlistNames);
    QList<QStringList> Select(QString strNameLike);
    void Update(QString strItemID, int iNewQuantity);
    void Delete(QString strItemID);
};
