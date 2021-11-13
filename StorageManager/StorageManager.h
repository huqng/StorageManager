#pragma once

#include <qstring.h>
#include <qsqldatabase.h>
#include <qlist.h>

class MdbControl
{
private:
    QSqlDatabase m_db;
public:
    MdbControl();
    bool IsOpen();
    QString GetLastError();
    void Close();

    void SetFilename(QString strFilename);
    QList<QStringList> SelectNameLike(QString strNameLike);
    void Update(QString strItemID, int iNewQuantity);
    void Delete(QString strItemID);
    void Insert(QString strItemID, QString strItemName, int iItemQuantity, QString strItemUnit, QString strItemPosition1, QString strItemPosition2, QString strItemPosition3, QString strComments);
    
    int GetMaxID();
    bool ExistID(const QString& ID);
};

class CStorageManager
{
private:
    MdbControl m_MdbControl;
    QStringList m_strlistColNames;
    QString m_strLastQuery;
public:
    CStorageManager();
    void SetFilename(QString strFilename);

    void SetColNames(QStringList strlistNames);
    QList<QStringList> Select(QString strNameLike);
    void Update(QString strItemID, int iNewQuantity);
    void Delete(QString strItemID);
    void Insert(QString strItemID, QString strItemName, int iItemQuantity, QString strItemUnit, QString strItemPosition1, QString strItemPosition2, QString strItemPosition3, QString strComments);
    
    int GetNextID();
};
