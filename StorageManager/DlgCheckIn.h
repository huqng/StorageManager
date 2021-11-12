#pragma once

#include "ui_DlgCheckIn.h"

#include <qdialog.h>
#include "qstring.h"

class DlgCheckIn: public QDialog
{
	Q_OBJECT
private:
	Ui::DlgCheckInUi ui;
public:
	DlgCheckIn(QString strItemID, QString strItemName, int iCurrentQuantity, QString strItemPosition1, QString strItemPosition2, QString strItemPosition3, QString strItemUnit, QString strComments);
	int GetQuantityCheckIn();
};

