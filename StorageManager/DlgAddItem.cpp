#include "DlgAddItem.h"

DlgAddItem::DlgAddItem():
	m_bNameValid(true),
	m_bQuantityValid(true),
	m_bUnitValid(true),
	m_bPosition1Valid(true),
	m_bPosition2Valid(true),
	m_bPosition3Valid(true),
	m_bCommentValid(true)
{
	ui.setupUi(this);
	
	this->setWindowTitle(u8"添加");

	ui.lineEditItemID->setText("0");
	ui.lineEditItemName->setText("name");
	ui.sbItemQuantity->setMaximum(9999);
	ui.sbItemQuantity->setMinimum(1);
	ui.sbItemQuantity->setValue(1);
	ui.lineEditItemUnit->setText("unit");
	
	ui.lineEditItemPosition3->setText("position3");
	ui.lineEditComments->setText("comments");

	connect(ui.lineEditItemID, &QLineEdit::textEdited, this, &DlgAddItem::slotOnLineEditItemIDEdited);
	connect(ui.lineEditItemName, &QLineEdit::textEdited, this, &DlgAddItem::slotOnLineEditItemNameEdited);
	connect(ui.sbItemQuantity, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &DlgAddItem::slotOnSpinboxItemQuantityEdited);
	connect(ui.lineEditItemUnit, &QLineEdit::textEdited, this, &DlgAddItem::slotOnLineEditItemUnitEdited);
	connect(ui.lineEditItemPosition3, &QLineEdit::textEdited, this, &DlgAddItem::slotOnLineEditItemPosition3Edited);
	connect(ui.lineEditComments, &QLineEdit::textEdited, this, &DlgAddItem::slotOnLineEditCommentEdited);
	connect(ui.cbPosition1, static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &DlgAddItem::slotOnComboBoxPosition1CurrentIndexChanged);
}


/* lineEdit编辑后检查文本合法性 */
void DlgAddItem::slotOnLineEditItemIDEdited(QString strText)
{
	if (!strText.isEmpty() && IsValidID(strText))
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

void DlgAddItem::slotOnLineEditItemNameEdited(QString strText)
{
	if (!strText.isEmpty() && IsValidText(strText))
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
	if (!strText.isEmpty() && IsValidText(strText))
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

void DlgAddItem::slotOnLineEditItemPosition3Edited(QString strText)
{
	if (!strText.isEmpty() && IsValidText(strText))
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
	if (IsValidText(strText))
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

void DlgAddItem::slotOnComboBoxPosition1CurrentIndexChanged(int iIndex)
{
	ui.cbPosition2->clear();
	ui.cbPosition2->addItems(ui.cbPosition1->currentData().toStringList());
}

bool DlgAddItem::IsValidID(const QString& strID)
{
	return IsValidText(strID);
}

bool DlgAddItem::IsValidText(const QString& strText)
{
	for (QChar c : strText)
	{
		if (c.unicode() <= 0xFF && !c.isDigit() && !c.isLetter() && c.unicode() != ' ')
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


void DlgAddItem::SetPositionOptions(QMap<QString, QStringList> mapPositions)
{
	QMap<QString, QStringList>::Iterator iter = mapPositions.begin();
	while (iter != mapPositions.end())
	{
		ui.cbPosition1->addItem(iter.key(), QVariant(iter.value()));
		++iter;
	}
}

QString DlgAddItem::GetItemID()
{
	return ui.lineEditItemID->text();
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
	return ui.cbPosition1->currentText();
}

QString DlgAddItem::GetItemPosition2()
{
	return ui.cbPosition2->currentText();
}

QString DlgAddItem::GetItemPosition3()
{
	return ui.lineEditItemPosition3->text();
}

QString DlgAddItem::GetComments()
{
	return ui.lineEditComments->text();
}
