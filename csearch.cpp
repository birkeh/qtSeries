#include "csearch.h"
#include "ui_csearch.h"

#include "cthetvdbv2.h"

#include "cserie.h"
#include "cmessageanimatedialog.h"

#include <QList>


cSearch::cSearch(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::cSearch)
{
	ui->setupUi(this);
	ui->m_lpSearchButton->setEnabled(false);
	ui->m_lpTabWidget->setCurrentIndex(0);
}

cSearch::~cSearch()
{
	delete ui;
}

void cSearch::on_m_lpSearch_textChanged(const QString& /*arg1*/)
{
	if(ui->m_lpSearch->text().isEmpty())
		ui->m_lpSearchButton->setEnabled(false);
	else
		ui->m_lpSearchButton->setEnabled(true);
}

void cSearch::on_m_lpSearchButton_clicked()
{
	cMessageAnimateDialog*	lpDialog	= new cMessageAnimateDialog(this);
	lpDialog->setTitle("Search");
	lpDialog->setMessage("Searching");
	lpDialog->show();

	cTheTVDBV2			theTVDBV2;
	QList<cSerie*>		serieList2	= theTVDBV2.search(ui->m_lpSearch->text());

	ui->m_lpResults->clear();

	for(int z = 0;z < serieList2.count();z++)
	{
		cSerie*	lpSerie	= serieList2.at(z);
		QTreeWidgetItem*	lpNew		= new QTreeWidgetItem(ui->m_lpResults);
		lpNew->setText(0, lpSerie->seriesName());
		lpNew->setText(2, QString("%1").arg(lpSerie->firstAired().year()));
		lpNew->setData(0, Qt::UserRole, QVariant::fromValue(lpSerie->id()));
		ui->m_lpResults->addTopLevelItem(lpNew);
	}
	ui->m_lpResults->resizeColumnToContents(0);
	ui->m_lpResults->resizeColumnToContents(1);

	ui->m_lpResults->sortItems(0, Qt::AscendingOrder);

	delete lpDialog;
}

qint32 cSearch::id()
{
	if(!ui->m_lpResults->selectedItems().count())
	{
		qint32	iID	= ui->m_lpSearch->text().toInt();
		if(iID)
			return(iID);
		return(-1);
	}
	return(ui->m_lpResults->selectedItems().at(0)->data(0, Qt::UserRole).toInt());
}

QString cSearch::placeholderName()
{
	return(ui->m_lpPlaceholderName->text());
}

bool cSearch::placeholder()
{
	return(ui->m_lpTabWidget->currentIndex() == 1);
}

qint16 cSearch::year()
{
	return(ui->m_lpYear->value());
}
