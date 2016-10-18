#include "cmessagedialog.h"
#include "ui_cmessagedialog.h"

#include <QThread>


cMessageDialog::cMessageDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::cMessageDialog)
{
	ui->setupUi(this);
}

cMessageDialog::~cMessageDialog()
{
	delete ui;
}

void cMessageDialog::setMessage(const QString& szMessage)
{
	ui->m_lpMessage->setText(szMessage);
	QThread::msleep(100);
}

void cMessageDialog::addMessage(const QString& szMessage)
{
	ui->m_lpMessage->setText(ui->m_lpMessage->text() + szMessage);
}

void cMessageDialog::setProgress(qint32 iMin, qint32 iMax)
{
	ui->m_lpProgressBar->setMinimum(iMin);
	ui->m_lpProgressBar->setMaximum(iMax);
	ui->m_lpProgressBar->setValue(0);
	QThread::msleep(100);
}

void cMessageDialog::setProgress(qint32 iValue)
{
	ui->m_lpProgressBar->setValue(iValue);
	QThread::msleep(100);
}

QPushButton* cMessageDialog::cancelButton()
{
	return(ui->m_lpCancel);
}
