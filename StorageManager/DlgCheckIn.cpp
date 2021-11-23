#include "DlgCheckIn.h"

DlgCheckIn::DlgCheckIn(QString strItemID, \
					   QString strItemName, \
					   int iCurrentQuantity,
					   QString strItemPosition1, \
					   QString strItemPosition2, \
					   QString strItemPosition3, \
					   QString strItemUnit, \
					   QString strComments):
	QDialog(Q_NULLPTR)
{
	ui.setupUi(this);

	this->setWindowTitle(u8"Èë¿â");

	ui.labelItemID->setText(strItemID);
	ui.labelItemName->setText(strItemName);
	ui.labelCurrentQuantityUnit->setText(QString::number(iCurrentQuantity).append(strItemUnit));
	ui.labelItemPosition1->setText(strItemPosition1);
	ui.labelItemPosition2->setText(strItemPosition2);
	ui.labelItemPosition3->setText(strItemPosition3);
	ui.sbItemQuantity->setValue(0);
	ui.sbItemQuantity->setMinimum(0);
	ui.labelItemUnit->setText(strItemUnit);
	ui.labelComments->setText(strComments);
}

int DlgCheckIn::GetQuantityCheckIn()
{
	return ui.sbItemQuantity->value();
}
