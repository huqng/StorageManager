#include "DlgAddItem.h"

DlgAddItem::DlgAddItem(int iItemID):
	m_bNameValid(true),
	m_bQuantityValid(true),
	m_bUnitValid(true),
	m_bPosition1Valid(true),
	m_bPosition2Valid(true),
	m_bPosition3Valid(true),
	m_bCommentValid(true)
{
	ui.setupUi(this);
	ui.labelItemID->setText(QString::number(iItemID));

	ui.lineEditItemName->setText("name");
	ui.sbItemQuantity->setMaximum(999);
	ui.sbItemQuantity->setMinimum(1);
	ui.sbItemQuantity->setValue(1);
	ui.lineEditItemUnit->setText("unit");
	ui.lineEditItemPosition1->setText("position1");
	ui.lineEditItemPosition2->setText("position2");
	ui.lineEditItemPosition3->setText("position3");
	ui.lineEditComments->setText("comments");

	connect(ui.lineEditItemName, &QLineEdit::textEdited, this, &DlgAddItem::slotOnLineEditItemNameEdited);
	connect(ui.sbItemQuantity, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &DlgAddItem::slotOnSpinboxItemQuantityEdited);
	connect(ui.lineEditItemUnit, &QLineEdit::textEdited, this, &DlgAddItem::slotOnLineEditItemUnitEdited);
	connect(ui.lineEditItemPosition1, &QLineEdit::textEdited, this, &DlgAddItem::slotOnLineEditItemPosition1Edited);
	connect(ui.lineEditItemPosition2, &QLineEdit::textEdited, this, &DlgAddItem::slotOnLineEditItemPosition2Edited);
	connect(ui.lineEditItemPosition3, &QLineEdit::textEdited, this, &DlgAddItem::slotOnLineEditItemPosition3Edited);
	connect(ui.lineEditComments, &QLineEdit::textEdited, this, &DlgAddItem::slotOnLineEditCommentEdited);
}


void DlgAddItem::slotOnLineEditItemNameEdited(QString strText)
{
	if (!strText.isEmpty() && TextValid(strText))
	{
		m_bNameValid = true;
		CheckTextAllValid();
	}
	else
	{
		ui.okButton->setDisabled(true);
		m_bNameValid = false;
	}
}

void DlgAddItem::slotOnSpinboxItemQuantityEdited(int iCnt)
{
	if (iCnt > 0)
	{
		m_bQuantityValid = true;
		CheckTextAllValid();
	}
	else
	{
		ui.okButton->setDisabled(true);
		m_bQuantityValid = false;
	}
}

void DlgAddItem::slotOnLineEditItemUnitEdited(QString strText)
{
	if (!strText.isEmpty() && TextValid(strText))
	{
		m_bUnitValid = true;
		CheckTextAllValid();
	}
	else
	{
		ui.okButton->setDisabled(true);
		m_bUnitValid = false;
	}
}

void DlgAddItem::slotOnLineEditItemPosition1Edited(QString strText)
{
	if (!strText.isEmpty() && TextValid(strText))
	{
		m_bPosition1Valid = true;
		CheckTextAllValid();
	}
	else
	{
		ui.okButton->setDisabled(true);
		m_bPosition1Valid = false;
	}
}

void DlgAddItem::slotOnLineEditItemPosition2Edited(QString strText)
{
	if (!strText.isEmpty() && TextValid(strText))
	{
		m_bPosition2Valid = true;
		CheckTextAllValid();
	}
	else
	{
		ui.okButton->setDisabled(true);
		m_bPosition2Valid = false;
	}
}

void DlgAddItem::slotOnLineEditItemPosition3Edited(QString strText)
{
	if (!strText.isEmpty() && TextValid(strText))
	{
		m_bPosition3Valid = true;
		CheckTextAllValid();
	}
	else
	{
		ui.okButton->setDisabled(true);
		m_bPosition3Valid = false;
	}
}

void DlgAddItem::slotOnLineEditCommentEdited(QString strText)
{
	if (TextValid(strText))
	{
		m_bCommentValid = true;
		CheckTextAllValid();
	}
	else
	{
		ui.okButton->setDisabled(true);
		m_bCommentValid = false;
	}
}

bool DlgAddItem::TextValid(const QString& strText)
{
	for (QChar c : strText)
	{
		if (c <= 0xFF && !c.isDigit() && !c.isLetter())
		{
			return false;
		}
	}
	return true;
}

void DlgAddItem::CheckTextAllValid()
{
	if (m_bNameValid && \
		m_bQuantityValid && \
		m_bUnitValid && \
		m_bPosition1Valid && \
		m_bPosition2Valid && \
		m_bPosition3Valid && \
		m_bCommentValid)
	{
		ui.okButton->setEnabled(true);
	}
	else
	{
		ui.okButton->setDisabled(true);
	}
}

QString DlgAddItem::GetItemName()
{
	return ui.lineEditItemName->text();
}

int DlgAddItem::GetItemQuantity()
{
	return ui.sbItemQuantity->value();
}

QString DlgAddItem::GetItemUnit()
{
	return ui.lineEditItemUnit->text();
}

QString DlgAddItem::GetItemPosition1()
{
	return ui.lineEditItemPosition1->text();
}

QString DlgAddItem::GetItemPosition2()
{
	return ui.lineEditItemPosition2->text();
}

QString DlgAddItem::GetItemPosition3()
{
	return ui.lineEditItemPosition3->text();
}

QString DlgAddItem::GetComments()
{
	return ui.lineEditComments->text();
}
