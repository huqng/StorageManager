#include "DlgCheckOut.h"

DlgCheckOut::DlgCheckOut(QString strItemID, \
	QString strItemName, \
	int iCurrentQuantity,
	QString strItemPosition1, \
	QString strItemPosition2, \
	QString strItemPosition3, \
	QString strItemUnit, \
	QString strComments) :
	QDialog(Q_NULLPTR)
{
	ui.setupUi(this);

	this->setWindowTitle(u8"³ö¿â");

	ui.labelItemID->setText(strItemID);
	ui.labelItemName->setText(strItemName);
	ui.labelCurrentQuantityUnit->setText(QString::number(iCurrentQuantity).append(" ").append(strItemUnit));
	ui.sbItemQuantity->setMaximum(iCurrentQuantity);
	ui.labelItemPosition1->setText(strItemPosition1);
	ui.labelItemPosition2->setText(strItemPosition2);
	ui.labelItemPosition3->setText(strItemPosition3);
	ui.sbItemQuantity->setValue(0);
	ui.sbItemQuantity->setMinimum(0);
	ui.labelItemUnit->setText(strItemUnit);
	ui.labelComments->setText(strComments);
}

int DlgCheckOut::GetQuantityCheckOut()
{
	return ui.sbItemQuantity->value();
}
