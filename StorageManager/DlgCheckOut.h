#pragma once

#include "ui_DlgCheckOut.h"

#include <qdialog.h>
#include "qstring.h"

class DlgCheckOut : public QDialog
{
	Q_OBJECT
private:
	Ui::DlgCheckOutUi ui;
public:
	DlgCheckOut(QString strItemID, QString strItemName, int iCurrentQuantity, QString strItemPosition1, QString strItemPosition2, QString strItemPosition3, QString strItemUnit, QString strComments);
	int GetQuantityCheckOut();
};

