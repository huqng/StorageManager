#pragma once

#include "ui_DlgAddItem.h"

#include <qmap.h>
#include <qlist.h>

class DlgAddItem: public QDialog
{
	Q_OBJECT
private:
	Ui::DlgAddItemUI ui;

	bool m_bNameValid;
	bool m_bQuantityValid;
	bool m_bUnitValid;
	bool m_bPosition1Valid;
	bool m_bPosition2Valid;
	bool m_bPosition3Valid;
	bool m_bCommentValid;

	bool IsValidID(const QString&);
	bool IsValidText(const QString&);
	void CheckTextAllValid();

public:
	DlgAddItem();

	void SetPositionOptions(QMap<QString, QStringList>);

	QString GetItemID();
	QString GetItemName();
	int GetItemQuantity();
	QString GetItemUnit();
	QString GetItemPosition1();
	QString GetItemPosition2();
	QString GetItemPosition3();
	QString GetComments();

public slots:
	void slotOnLineEditItemIDEdited(QString strText);
	void slotOnLineEditItemNameEdited(QString strText);
	void slotOnSpinboxItemQuantityEdited(int iCnt);
	void slotOnLineEditItemUnitEdited(QString strText);
	void slotOnComboBoxPosition1CurrentIndexChanged(int iIndex);
	void slotOnLineEditItemPosition3Edited(QString strText);
	void slotOnLineEditCommentEdited(QString strText);
};

