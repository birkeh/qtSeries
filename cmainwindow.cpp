#include "cmainwindow.h"
#include "ui_cmainwindow.h"

#include "cthetvdb.h"
#include "cseasondelegate.h"
#include "csearch.h"
#include "cmessageanimatedialog.h"
#include "cmessagedialog.h"
#include "cedit.h"
#include "cimage.h"

#include <QTreeWidgetItem>
#include <QDir>
#include <QFile>
#include <QSqlQuery>
#include <QSqlError>
#include <QDesktopServices>
#include <QClipboard>
#include <QUrl>
#include <QIcon>
#include <QThread>

#include <QSettings>

#include <QTime>
#include <QMessageBox>
#include <QtDebug>

#include <QElapsedTimer>

#include <QScrollBar>


cMainWindow::cMainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::cMainWindow),
	m_szOldSelected(""),
	m_lpMessageDialog(0),
	m_lpUpdateThread(0),
	m_lpPicturesThread(0),
	m_bProcessing(false)
{
	m_timer.start();

	cMessageAnimateDialog*	lpDialog	= new cMessageAnimateDialog(this);
	lpDialog->setTitle("Initializing");
	lpDialog->setMessage("Initializing");
	lpDialog->show();

	ui->setupUi(this);

	m_lpSeriesListModel	= new QStandardItemModel(0, 3);
	initDB();

	ui->m_lpSeriesList1->setModel(m_lpSeriesListModel);
	ui->m_lpSeriesList1->setItemDelegate(new cSeasonDelegate(ui->m_lpSeriesList1));
	ui->m_lpSeriesList2->setModel(m_lpSeriesListModel);
	ui->m_lpSeriesList2->setItemDelegate(new cSeasonDelegate(ui->m_lpSeriesList2));

	loadDB();
	displaySeries();

	QSettings	settings;
	qint16		iX		= settings.value("main/x", QVariant::fromValue(-1)).toInt();
	qint16		iY		= settings.value("main/y", QVariant::fromValue(-1)).toInt();
	qint16		iWidth	= settings.value("main/width", QVariant::fromValue(-1)).toInt();
	qint16		iHeight	= settings.value("main/height", QVariant::fromValue(-1)).toInt();

	if(iWidth != -1 && iHeight != -1)
		resize(iWidth, iHeight);
	if(iX != -1 && iY != -1)
		move(iX, iY);

	delete lpDialog;

	connect(ui->m_lpSeriesList1->selectionModel(), &QItemSelectionModel::selectionChanged, this, &cMainWindow::selectionChanged1);
	connect(ui->m_lpSeriesList2->selectionModel(), &QItemSelectionModel::selectionChanged, this, &cMainWindow::selectionChanged2);

	connect(ui->m_lpSeriesList1->verticalScrollBar(), &QScrollBar::valueChanged, this, &cMainWindow::scrollbarValueChanged1);
	connect(ui->m_lpSeriesList2->verticalScrollBar(), &QScrollBar::valueChanged, this, &cMainWindow::scrollbarValueChanged2);
}

cMainWindow::~cMainWindow()
{
	delete ui;
}

void cMainWindow::closeEvent(QCloseEvent *event)
{
	QSettings	settings;
	settings.setValue("main/width", QVariant::fromValue(size().width()));
	settings.setValue("main/hight", QVariant::fromValue(size().height()));
	settings.setValue("main/x", QVariant::fromValue(x()));
	settings.setValue("main/y", QVariant::fromValue(y()));
	if(this->isMaximized())
		settings.setValue("main/maximized", QVariant::fromValue(true));
	else
		settings.setValue("main/maximized", QVariant::fromValue(false));

	event->accept();
}

void cMainWindow::initDB()
{
	QDir	dir;
	dir.mkpath(QDir::homePath() + QDir::separator() + "qtseries" + QDir::separator());
	QString	szDBPath	= QDir::homePath() + QDir::separator() + "qtseries" + QDir::separator() + "qtseries.db";

	m_db	= QSqlDatabase::addDatabase("QSQLITE");
	m_db.setHostName("localhost");
	m_db.setDatabaseName(szDBPath);
	if(!m_db.open())
		return;

	QSqlQuery	query;

	if(!m_db.tables().count())
	{
		query.exec("CREATE TABLE serie ("
					"   seriesID      INTEGER,"
					"   seriesName    STRING,"
					"   language      STRING,"
					"   banner        STRING,"
					"   overview      TEXT,"
					"   firstAired    DATE,"
					"   network       STRING,"
					"   imdbid        STRING,"
					"   id            INTEGER,"
					"   contentRating STRING,"
					"   rating        DOUBLE,"
					"   ratingCount   INTEGER,"
					"   runtime       INTEGER,"
					"   status        STRING,"
					"   download      STRING,"
					"   cliffhanger   BOOL,"
					"   actor         STRING,"
					"   genre         STRING);");

		query.exec("CREATE TABLE episode ("
					"	episodeID      INTEGER,"
					"	episodeName    STRING,"
					"	episodeNumber  INTEGER,"
					"	firstAired     DATE,"
					"	imdbid         STRING,"
					"	language       STRING,"
					"	overview       TEXT,"
					"	productioncode STRING,"
					"	rating         DOUBLE,"
					"	ratingCount    INTEGER,"
					"	seasonNumber   INTEGER,"
					"	seasonID       INTEGER,"
					"	seriesID       INTEGER,"
					"	state          INT,"
					"   filename       STRING,"
					"   thumb_height   INT,"
					"   thumb_width    INT,"
					"   director       STRING,"
					"   gueststars     STRING,"
					"   episode_writer STRING);");
	}
}

void cMainWindow::loadDB()
{
	m_serieList.clear();

	QSqlQuery	query;

	qint32		iOldSeriesID	= -1;
	qint32		iOldSeasonID	= -1;
	qint32		iSeriesID;
	qint32		iSeasonID;
	cSeason*	lpSeason;
	cEpisode*	lpEpisode;

	if(query.exec("SELECT serie.seriesID, serie.seriesName, serie.language, serie.banner, serie.overview, serie.firstAired, serie.network, serie.imdbid, serie.id, serie.contentRating, serie.rating, serie.ratingCount, serie.runtime, serie.status, serie.download, serie.cliffhanger, serie.actor, serie.genre, episode.episodeID, episode.episodeName, episode.episodeNumber, episode.firstAired, episode.imdbid, episode.language, episode.overview, episode.productioncode, episode.rating, episode.ratingCount, episode.seasonNumber, episode.seasonID, episode.seriesID, episode.state, episode.filename, episode.thumb_height, episode.thumb_width, episode.director, episode.gueststars, episode.episode_writer FROM serie, episode WHERE serie.id = episode.seriesID ORDER BY serie.seriesName, episode.seasonNumber, episode.episodeNumber;"))
	{
		while(query.next())
		{
			iSeriesID	= query.value( 8).toInt();
			iSeasonID	= query.value(29).toInt();

			if(iSeriesID != iOldSeriesID)
			{
				m_serieList.add(iSeriesID);
				iOldSeriesID	= iSeriesID;
			}

			cSerie*	lpSerie	= m_serieList.add(iSeriesID);
			lpSerie->setSeriesID(query.value(0).toInt());
			lpSerie->setSeriesName(query.value(1).toString());
			lpSerie->setLanguage(query.value(2).toString());
			lpSerie->setBanner(query.value(3).toString());
			lpSerie->setOverview(query.value(4).toString());
			lpSerie->setFirstAired(query.value(5).toDate());
			lpSerie->setNetwork(query.value(6).toString());
			lpSerie->setIMDBID(query.value(7).toString());
			lpSerie->setContentRating(query.value(9).toString());
			lpSerie->setRating(query.value(10).toDouble());
			lpSerie->setRatingCount(query.value(11).toInt());
			lpSerie->setRuntime(query.value(12).toInt());
			lpSerie->setStatus(query.value(13).toString());
			lpSerie->setDownload(query.value(14).toString());
			lpSerie->setCliffhanger(query.value(15).toBool());
			lpSerie->setActors(query.value(16).toString().split(","));
			lpSerie->setGenre(query.value(17).toString().split(","));

			if(iSeasonID != iOldSeasonID)
			{
				lpSeason		= lpSerie->addSeason(query.value(28).toInt());
				iOldSeasonID	= iSeasonID;
			}

			lpEpisode	= lpSeason->addEpisode(query.value(20).toInt());
			lpEpisode->setID(query.value(18).toInt());
			lpEpisode->setEpisodeName(query.value(19).toString());
			lpEpisode->setFirstAired(query.value(21).toDate());
			lpEpisode->setIMDBID(query.value(22).toString());
			lpEpisode->setLanguage(query.value(23).toString());
			lpEpisode->setOverview(query.value(24).toString());
			lpEpisode->setProductionCode(query.value(25).toString());
			lpEpisode->setRating(query.value(26).toDouble());
			lpEpisode->setRatingCount(query.value(27).toInt());
			lpEpisode->setSeasonNumber(query.value(28).toInt());
			lpEpisode->setSeasonID(query.value(29).toInt());
			lpEpisode->setSeriesID(query.value(30).toInt());
			lpEpisode->setState((cEpisode::State)query.value(31).toInt());
			lpEpisode->setFileName(query.value(32).toString());
			lpEpisode->setThumbHeight(query.value(33).toInt());
			lpEpisode->setThumbWidth(query.value(34).toInt());
			lpEpisode->setDirector(query.value(35).toString().split(","));
			lpEpisode->setGuestStars(query.value(36).toString().split(","));
			lpEpisode->setWriter(query.value(37).toString().split(","));
		}
	}
}

void cMainWindow::displaySeries()
{
	m_lpSeriesListModel->clear();

	qint16	iMin	= m_serieList.minSeason();
	qint16	iMax	= m_serieList.maxSeason();

	m_lpSeriesListModel->setColumnCount(iMax-iMin+2);

	QStringList	header;
	header << "Nr" << "Serie" << "Year";

	for(int z = 0;z <= iMax;z++)
		header.append(QString("Season %1").arg(z));

	m_lpSeriesListModel->setHorizontalHeaderLabels(header);
	m_lpSeriesListModel->setHeaderData(0, Qt::Horizontal, QVariant(iMin), Qt::UserRole);

	QFont	font	= ui->m_lpSeriesList1->font();
	QFont	fontI	= ui->m_lpSeriesList1->font();

	font.setBold(true);
	fontI.setItalic(true);

	QIcon		icon(":/128279.png");
	QModelIndex	selected;

	for(int serie = 0;serie < m_serieList.count();serie++)
	{
		QList<QStandardItem*>	lpItems;

		for(int z = 0;z < header.count();z++)
			lpItems.append(new QStandardItem);

		cSerie*	lpSerie	= m_serieList.at(serie);

		lpItems.at(0)->setText(QString("%1").arg(serie+1));
		lpItems.at(0)->setTextAlignment(Qt::AlignRight);
		lpItems.at(0)->setData(QVariant::fromValue(lpSerie), Qt::UserRole);

		lpItems.at(1)->setText(lpSerie->seriesName());
		lpItems.at(1)->setTextAlignment(Qt::AlignLeft);

		lpItems.at(2)->setText(lpSerie->firstAired().toString("yyyy"));
		lpItems.at(2)->setTextAlignment(Qt::AlignRight);

		bool		bHasInit	= false;
		bool		bHasProg	= false;
		bool		bHasDone	= false;

		QString		szOpen;

		QList<cSeason*>	seasonList	= lpSerie->seasonList();
		for(int season = 0;season < seasonList.count();season++)
		{
			QString		szInit		= "";
			QString		szProg		= "";
			QString		szDone		= "";

			cSeason*	lpSeason	= seasonList.at(season);
			lpItems.at(lpSeason->number()+3-iMin)->setData(QVariant::fromValue(lpSeason), Qt::UserRole);

			for(int y = 0;y < lpSeason->episodeList().count();y++)
			{
				if(lpSeason->episodeList().at(y)->state() == cEpisode::StateInit)
				{
					if(szInit.isEmpty())
						szInit.append(QString("%1").arg(lpSeason->episodeList().at(y)->episodeNumber()));
					else
						szInit.append(QString(", %1").arg(lpSeason->episodeList().at(y)->episodeNumber()));

					if(lpSeason->number())
						bHasInit	= true;
				}
				else if(lpSeason->episodeList().at(y)->state() == cEpisode::StateProgress)
				{
					if(szProg.isEmpty())
						szProg.append(QString("%1").arg(lpSeason->episodeList().at(y)->episodeNumber()));
					else
						szProg.append(QString(", %1").arg(lpSeason->episodeList().at(y)->episodeNumber()));

					if(lpSeason->number())
						bHasProg	= true;
				}
				else if(lpSeason->episodeList().at(y)->state() == cEpisode::StateDone)
				{
					if(szDone.isEmpty())
						szDone.append(QString("%1").arg(lpSeason->episodeList().at(y)->episodeNumber()));
					else
						szDone.append(QString(", %1").arg(lpSeason->episodeList().at(y)->episodeNumber()));

					if(lpSeason->number())
						bHasDone	= true;
				}
			}

			QString	szTooltip;

			if(szInit.isEmpty())
				szTooltip.append("open: none\n");
			else
				szTooltip.append("open: " + szInit + "\n");

			if(lpSeason->number())
			{
				if(szOpen.length())
					szOpen	+= "\n";

				if(szInit.isEmpty())
					szOpen	+= QString("Season %1: none").arg(lpSeason->number(), 2, 10, QChar('0'));
				else
					szOpen	+= QString("Season %1: %2").arg(lpSeason->number(), 2, 10, QChar('0')).arg(szInit);
			}

			if(szProg.isEmpty())
				szTooltip.append("in progress: none\n");
			else
				szTooltip.append("in progress: " + szProg + "\n");

			if(szDone.isEmpty())
				szTooltip.append("done: none");
			else
				szTooltip.append("done: " + szDone);

			if(szTooltip.isEmpty())
				lpItems.at(lpSeason->number()+3-iMin)->setToolTip(szTooltip);
			else
				lpItems.at(lpSeason->number()+3-iMin)->setToolTip(szTooltip);
		}

		if(lpSerie->status().compare("Ended", Qt::CaseInsensitive))
		{
			lpItems.at(0)->setFont(font);
			lpItems.at(1)->setFont(font);
			lpItems.at(2)->setFont(font);
		}

		if(lpSerie->cliffhanger())
		{
			lpItems.at(0)->setFont(fontI);
			lpItems.at(1)->setFont(fontI);
			lpItems.at(2)->setFont(fontI);
			lpItems.at(0)->setForeground(QBrush(Qt::red));
			lpItems.at(1)->setForeground(QBrush(Qt::red));
			lpItems.at(2)->setForeground(QBrush(Qt::red));
		}

		if(bHasProg)
		{
			lpItems.at(0)->setForeground(QBrush(Qt::white));
			lpItems.at(1)->setForeground(QBrush(Qt::white));
			lpItems.at(2)->setForeground(QBrush(Qt::white));
			lpItems.at(0)->setBackground(QBrush(Qt::blue));
			lpItems.at(1)->setBackground(QBrush(Qt::blue));
			lpItems.at(2)->setBackground(QBrush(Qt::blue));
		}
		else if(bHasInit)
		{
			lpItems.at(0)->setBackground(QBrush(Qt::lightGray));
			lpItems.at(1)->setBackground(QBrush(Qt::lightGray));
			lpItems.at(2)->setBackground(QBrush(Qt::lightGray));
		}
		else
		{
			lpItems.at(0)->setBackground(QBrush(Qt::green));
			lpItems.at(1)->setBackground(QBrush(Qt::green));
			lpItems.at(2)->setBackground(QBrush(Qt::green));
		}

		if(lpSerie->download().length())
			lpItems.at(0)->setIcon(icon);
		m_lpSeriesListModel->appendRow(lpItems);

		if(!m_szOldSelected.isEmpty())
		{
			if(!m_szOldSelected.compare(lpSerie->seriesName()))
			{
				selected	= m_lpSeriesListModel->indexFromItem(lpItems.at(0));
				ui->m_lpSeriesList1->selectionModel()->select(selected, QItemSelectionModel::Select | QItemSelectionModel::Rows);
			}
		}

		lpItems.at(0)->setToolTip(szOpen);
		lpItems.at(1)->setToolTip(szOpen);
		lpItems.at(2)->setToolTip(szOpen);
	}

	for(int z = 2;z >= 0;z--)
		ui->m_lpSeriesList1->resizeColumnToContents(z);

	for(int z = 3;z < m_lpSeriesListModel->columnCount();z++)
		ui->m_lpSeriesList2->resizeColumnToContents(z);

	if(selected.isValid())
	{
		ui->m_lpSeriesList1->scrollTo(selected, QAbstractItemView::PositionAtCenter);
		ui->m_lpSeriesList2->scrollTo(selected, QAbstractItemView::PositionAtCenter);
	}

	m_szOldSelected	= "";

	for(int z = 3;z < m_lpSeriesListModel->columnCount();z++)
		ui->m_lpSeriesList1->setColumnHidden(z, true);

	ui->m_lpSeriesList2->setColumnHidden(0, true);
	ui->m_lpSeriesList2->setColumnHidden(1, true);
	ui->m_lpSeriesList2->setColumnHidden(2, true);
}

void cMainWindow::on_m_lpSeriesList1_customContextMenuRequested(const QPoint &pos)
{
	QMenu*	lpMenu	= new QMenu(this);

	lpMenu->addAction("add", this, SLOT(onActionAdd()));

	if(ui->m_lpSeriesList1->selectionModel()->selectedRows().count() == 1)
	{
		cSerie*	lpSerie	= m_lpSeriesListModel->itemFromIndex(ui->m_lpSeriesList1->selectionModel()->selectedRows().at(0))->data(Qt::UserRole).value<cSerie*>();
		if(lpSerie)
		{
			lpMenu->addAction("update", this, SLOT(onActionUpdate()));
			lpMenu->addAction("delete", this, SLOT(onActionDelete()));
			lpMenu->addAction("edit", this, SLOT(onActionEdit()));
			lpMenu->addSeparator();

			if(!lpSerie->imdbID().isEmpty())
				lpMenu->addAction("open IMDB", this, SLOT(onActionGotoIMDB()));

			if(!lpSerie->download().isEmpty())
			{
				lpMenu->addAction("open download link", this, SLOT(onActionGotoDownload()));
				lpMenu->addAction("copy download link", this, SLOT(onActionCopyDownload()));
			}
			lpMenu->addSeparator();
			lpMenu->addAction("load images", this, SLOT(onActionLoadPictures()));
		}
	}
	else if(ui->m_lpSeriesList1->selectionModel()->selectedRows().count())
	{
		lpMenu->addAction("update", this, SLOT(onActionUpdate()));
		lpMenu->addAction("delete", this, SLOT(onActionDelete()));
		lpMenu->addSeparator();
		lpMenu->addAction("load images", this, SLOT(onActionLoadPictures()));
	}

	lpMenu->popup(ui->m_lpSeriesList1->viewport()->mapToGlobal(pos));
}

void cMainWindow::on_m_lpSeriesList2_customContextMenuRequested(const QPoint &pos)
{
	QMenu*	lpMenu	= new QMenu(this);

	lpMenu->addAction("add", this, SLOT(onActionAdd()));

	if(ui->m_lpSeriesList2->selectionModel()->selectedRows().count() == 1)
	{
		cSerie*	lpSerie	= m_lpSeriesListModel->itemFromIndex(ui->m_lpSeriesList2->selectionModel()->selectedRows().at(0))->data(Qt::UserRole).value<cSerie*>();
		if(lpSerie)
		{
			lpMenu->addAction("update", this, SLOT(onActionUpdate()));
			lpMenu->addAction("delete", this, SLOT(onActionDelete()));
			lpMenu->addAction("edit", this, SLOT(onActionEdit()));
			lpMenu->addSeparator();

			if(!lpSerie->imdbID().isEmpty())
				lpMenu->addAction("open IMDB", this, SLOT(onActionGotoIMDB()));

			if(!lpSerie->download().isEmpty())
			{
				lpMenu->addAction("open download link", this, SLOT(onActionGotoDownload()));
				lpMenu->addAction("copy download link", this, SLOT(onActionCopyDownload()));
			}
			lpMenu->addSeparator();
			lpMenu->addAction("load images", this, SLOT(onActionLoadPictures()));
		}
	}
	else if(ui->m_lpSeriesList2->selectionModel()->selectedRows().count())
	{
		lpMenu->addAction("update", this, SLOT(onActionUpdate()));
		lpMenu->addAction("delete", this, SLOT(onActionDelete()));
		lpMenu->addSeparator();
		lpMenu->addAction("load images", this, SLOT(onActionLoadPictures()));
	}

	lpMenu->popup(ui->m_lpSeriesList2->viewport()->mapToGlobal(pos));
}

static bool serieSort(cSerie* s1, cSerie* s2)
{
	QString str1	= s1->seriesName();
	QString	str2	= s2->seriesName();
	return(str1 < str2);
}

bool cMainWindow::runEdit(cSerie* lpSerie, QString& szDownload)
{
	cEdit*	lpEdit	= new cEdit;

	cMessageAnimateDialog*	lpDialog	= new cMessageAnimateDialog(this);
	lpDialog->setTitle("Edit");
	lpDialog->setMessage("Loading");
	lpDialog->show();

	lpEdit->setSerie(lpSerie);

	QSettings	settings;
	qint16		iX		= settings.value("edit/x", QVariant::fromValue(-1)).toInt();
	qint16		iY		= settings.value("edit/y", QVariant::fromValue(-1)).toInt();
	qint16		iWidth	= settings.value("edit/width", QVariant::fromValue(-1)).toInt();
	qint16		iHeight	= settings.value("edit/height", QVariant::fromValue(-1)).toInt();

	if(iX != -1 && iY != -1)
		lpEdit->move(iX, iY);
	if(iWidth != -1 && iHeight != -1)
		lpEdit->resize(iWidth, iHeight);

	delete lpDialog;

	qint16	ret	= lpEdit->exec();

	settings.setValue("edit/width", QVariant::fromValue(lpEdit->size().width()));
	settings.setValue("edit/height", QVariant::fromValue(lpEdit->size().height()));
	settings.setValue("edit/x", QVariant::fromValue(lpEdit->x()));
	settings.setValue("edit/y", QVariant::fromValue(lpEdit->y()));
	if(this->isMaximized())
		settings.setValue("edit/maximized", QVariant::fromValue(true));
	else
		settings.setValue("edit/maximized", QVariant::fromValue(false));

	if(ret == QDialog::Rejected)
	{
		delete lpEdit;

		int	x;
		QList<cSeason*>	seasonList	= lpSerie->seasonList();
		for(x = 0;x < seasonList.count();x++)
		{
			cSeason*	lpSeason	= seasonList.at(x);

			QList<cEpisode*>	episodeList	= lpSeason->episodeList();
			for(int y = 0;y < episodeList.count();y++)
			{
				cEpisode*	lpEpisode	= episodeList.at(y);
				if(lpEpisode)
				{
					lpEpisode->setButton1(0);
					lpEpisode->setButton2(0);
					lpEpisode->setButton3(0);
				}
			}
		}

		return(false);
	}

	szDownload	= lpEdit->download();

	lpSerie->setDownload(szDownload);
	lpSerie->updateState();

	delete lpEdit;

	return(true);
}

void cMainWindow::onActionAdd()
{
	cSearch*	lpSearch	= new cSearch(this);
	if(lpSearch->exec() == QDialog::Rejected)
	{
		delete lpSearch;
		return;
	}


	qint32	id				= lpSearch->id();
	QString	szPlaceholder	= lpSearch->placeholderName();
	bool	bPlaceholder	= lpSearch->placeholder();
	qint16	iYear			= lpSearch->year();

	delete lpSearch;

	cSerie*	lpSerie			= 0;

	cMessageAnimateDialog*	lpDialog	= new cMessageAnimateDialog(this);
	lpDialog->setTitle("Refresh");
	lpDialog->setMessage("Loading");
	lpDialog->show();

	if(!bPlaceholder)
	{
		if(id == -1)
			return;

		cTheTVDB		tvDB;

		lpSerie	= tvDB.load(id, "de");

		delete lpDialog;

		QString	szDownload;
		if(!runEdit(lpSerie, szDownload))
			return;

		lpDialog	= new cMessageAnimateDialog(this);
		lpDialog->setTitle("Update");
		lpDialog->setMessage("Updating");
		lpDialog->show();

//		lpSerie->setDownload(szDownload);
//		lpSerie->updateState();

		lpSerie->save(m_db);

	}
	else
	{
		lpSerie	= new cSerie;
		lpSerie->setSeriesName(szPlaceholder);

		qint32	iMax	= 0;
		QSqlQuery	query;
		if(query.exec("SELECT MAX(id) FROM serie;"))
		{
			query.next();
			if(query.isValid())
				iMax	= query.value(0).toInt();
		}
		if(iMax < 1000000)
			iMax	= 1000000;
		else
			iMax++;
		lpSerie->setID(iMax);
		lpSerie->setFirstAired(QDate(iYear, 1, 1));
		lpSerie->save(m_db);
	}
	m_serieList.add(lpSerie);
	std::sort(m_serieList.begin(), m_serieList.end(), serieSort);

	m_szOldSelected	= lpSerie->seriesName();
	displaySeries();

	delete lpDialog;
}

void cMainWindow::onActionUpdate()
{
	if(ui->m_lpSeriesList1->selectionModel()->selectedRows().count())
	{
		if(ui->m_lpSeriesList1->selectionModel()->selectedRows().count() == 1)
			m_szOldSelected	= m_lpSeriesListModel->itemFromIndex(ui->m_lpSeriesList1->selectionModel()->selectedIndexes().at(1))->text();

		m_lpMessageDialog	= new cMessageDialog(this);
		m_lpMessageDialog->setWindowTitle("Update");
		m_lpMessageDialog->setMessage("Updating");
		m_lpMessageDialog->setProgress(0, ui->m_lpSeriesList1->selectionModel()->selectedRows().count());
		m_lpMessageDialog->show();

		m_lpUpdateThread		= new cUpdateThread;
		m_lpUpdateThread->setData(m_lpMessageDialog, ui->m_lpSeriesList1->selectionModel()->selectedRows(), m_db);

		connect(m_lpUpdateThread, SIGNAL(finished()), this, SLOT(updateDone()));
		connect(m_lpUpdateThread, SIGNAL(updateMessage(QString,qint32)), this, SLOT(updateMessage(QString,qint32)));
		connect(m_lpUpdateThread, SIGNAL(updateAppendMessage(QString)), this, SLOT(updateAppendMessage(QString)));

		m_lpUpdateThread->start();
	}
}

void cMainWindow::updateMessage(const QString& szMessage, const qint32& iProgress)
{
	m_lpMessageDialog->setMessage(szMessage);
	m_lpMessageDialog->setProgress(iProgress);
}

void cMainWindow::updateAppendMessage(const QString& szMessage)
{
	m_lpMessageDialog->addMessage(szMessage);
}

void cMainWindow::updateDone()
{
	if(m_lpUpdateThread)
		delete m_lpUpdateThread;
	m_lpUpdateThread	= 0;

	loadDB();
	displaySeries();

	if(m_lpMessageDialog)
		delete m_lpMessageDialog;
	m_lpMessageDialog	= 0;
}

void cMainWindow::onActionDelete()
{
	if(QMessageBox::question(this, "Delete Serie", "Are you sure?") == QMessageBox::No)
		return;

	cSerie*	lpSerie	= m_lpSeriesListModel->itemFromIndex(ui->m_lpSeriesList1->selectionModel()->selectedRows().at(0))->data(Qt::UserRole).value<cSerie*>();
	if(!lpSerie)
		return;

	cMessageAnimateDialog*	lpDialog	= new cMessageAnimateDialog(this);
	lpDialog->setTitle("Delete");
	lpDialog->setMessage("Deleting");
	lpDialog->show();

	lpSerie->del(m_db);

	loadDB();
	displaySeries();

	delete lpDialog;
}

void cMainWindow::onActionEdit()
{
	cSerie*	lpSerie	= m_lpSeriesListModel->itemFromIndex(ui->m_lpSeriesList1->selectionModel()->selectedRows().at(0))->data(Qt::UserRole).value<cSerie*>();
	if(!lpSerie)
		return;

	QString	szDownload;
	if(!runEdit(lpSerie, szDownload))
		return;

	cMessageAnimateDialog*	lpDialog	= new cMessageAnimateDialog(this);
	lpDialog->setTitle("Update");
	lpDialog->setMessage("Updating");
	lpDialog->show();

	//lpSerie->setDownload(szDownload);
	//lpSerie->updateState();

	lpSerie->del(m_db);
	lpSerie->save(m_db);

	m_szOldSelected	= lpSerie->seriesName();
	loadDB();
	displaySeries();

	delete lpDialog;
}

void cMainWindow::on_m_lpSeriesList1_doubleClicked(const QModelIndex &/*index*/)
{
	onActionEdit();
}


void cMainWindow::on_m_lpSeriesList2_doubleClicked(const QModelIndex &/*index*/)
{
	onActionEdit();
}

void cMainWindow::onActionGotoIMDB()
{
	if(ui->m_lpSeriesList1->selectionModel()->selectedRows().count())
	{
		cSerie*	lpSerie	= m_lpSeriesListModel->itemFromIndex(ui->m_lpSeriesList1->selectionModel()->selectedRows().at(0))->data(Qt::UserRole).value<cSerie*>();
		if(lpSerie)
		{
			QString	link	= QString("http://www.imdb.com/title/%1").arg(lpSerie->imdbID());
			QDesktopServices::openUrl(QUrl(link));
		}
	}
}

void cMainWindow::onActionGotoDownload()
{
	if(ui->m_lpSeriesList1->selectionModel()->selectedRows().count())
	{
		cSerie*	lpSerie	= m_lpSeriesListModel->itemFromIndex(ui->m_lpSeriesList1->selectionModel()->selectedRows().at(0))->data(Qt::UserRole).value<cSerie*>();
		if(lpSerie)
		{
			QString	link	= lpSerie->download();
			QDesktopServices::openUrl(QUrl(link));
		}
	}
}

void cMainWindow::onActionCopyDownload()
{
	if(ui->m_lpSeriesList1->selectionModel()->selectedRows().count())
	{
		cSerie*	lpSerie	= m_lpSeriesListModel->itemFromIndex(ui->m_lpSeriesList1->selectionModel()->selectedRows().at(0))->data(Qt::UserRole).value<cSerie*>();
		if(lpSerie)
		{
			QString		link		= lpSerie->download();
			QClipboard*	lpClipboard	= QApplication::clipboard();
			lpClipboard->setText(link);
		}
	}
}

void cMainWindow::onActionLoadPictures()
{
	QList<cSerie*>	serieList;

	if(ui->m_lpSeriesList1->selectionModel()->selectedRows().count())
	{
		for(int z = 0;z < ui->m_lpSeriesList1->selectionModel()->selectedRows().count();z++)
			serieList.append(m_lpSeriesListModel->itemFromIndex(ui->m_lpSeriesList1->selectionModel()->selectedRows().at(z))->data(Qt::UserRole).value<cSerie*>());
	}

	if(!serieList.count())
		return;

	qint16	iTotal		= 0;

	for(int z = 0;z < serieList.count();z++)
	{
		cSerie*	lpSerie	= serieList.at(z);
		for(int y = 0;y < lpSerie->seasonList().count();y++)
			iTotal += lpSerie->seasonList().at(y)->episodeCount();
	}

	m_lpMessageDialog	= new cMessageDialog(this);
	m_lpMessageDialog->setWindowTitle("Images");
	m_lpMessageDialog->setMessage("loading Images");
	m_lpMessageDialog->setProgress(0, iTotal-1);
	m_lpMessageDialog->show();

	m_lpPicturesThread		= new cPicturesThread;
	m_lpPicturesThread->setData(m_lpMessageDialog, serieList);

	connect(m_lpPicturesThread, SIGNAL(finished()), this, SLOT(picturesDone()));
	connect(m_lpPicturesThread, SIGNAL(picturesMessage(QString,qint32)), this, SLOT(picturesMessage(QString,qint32)));
	connect(m_lpPicturesThread, SIGNAL(picturesAppendMessage(QString)), this, SLOT(picturesAppendMessage(QString)));

	m_lpPicturesThread->start();
}

void cMainWindow::picturesMessage(const QString& szMessage, const qint32& iProgress)
{
	m_lpMessageDialog->setMessage(szMessage);
	m_lpMessageDialog->setProgress(iProgress);
}

void cMainWindow::picturesAppendMessage(const QString& szMessage)
{
	m_lpMessageDialog->addMessage(szMessage);
}

void cMainWindow::picturesDone()
{
	if(m_lpPicturesThread)
		delete m_lpPicturesThread;
	m_lpPicturesThread	= 0;

	if(m_lpMessageDialog)
		delete m_lpMessageDialog;
	m_lpMessageDialog	= 0;
}

void cMainWindow::on_m_lpSeriesList1_pressed(const QModelIndex &index)
{
	switch(QGuiApplication::mouseButtons())
	{
	case Qt::MiddleButton:
		if(ui->m_lpSeriesList1->selectionModel()->selectedRows().count() == 1)
		{
			cSerie*	lpSerie	= m_lpSeriesListModel->itemFromIndex(ui->m_lpSeriesList1->selectionModel()->selectedRows().at(0))->data(Qt::UserRole).value<cSerie*>();
			if(lpSerie)
			{
				if(!lpSerie->download().isEmpty())
					onActionGotoDownload();
			}
		}
		break;
	default:
		break;
	}
}

void cMainWindow::on_m_lpSeriesList2_pressed(const QModelIndex &index)
{
	switch(QGuiApplication::mouseButtons())
	{
	case Qt::MiddleButton:
		if(ui->m_lpSeriesList2->selectionModel()->selectedRows().count() == 1)
		{
			cSerie*	lpSerie	= m_lpSeriesListModel->itemFromIndex(ui->m_lpSeriesList2->selectionModel()->selectedRows().at(0))->data(Qt::UserRole).value<cSerie*>();
			if(lpSerie)
			{
				if(!lpSerie->download().isEmpty())
					onActionGotoDownload();
			}
		}
		break;
	default:
		break;
	}
}

void cMainWindow::selectionChanged1(const QItemSelection& /*selected*/, const QItemSelection& /*deselected*/)
{
	if(m_bProcessing)
		return;

	m_bProcessing	= true;

	QItemSelectionModel*	lp1			= ui->m_lpSeriesList1->selectionModel();
	QItemSelectionModel*	lp2			= ui->m_lpSeriesList2->selectionModel();
	QModelIndexList			selected1	= lp1->selectedIndexes();

	lp2->clearSelection();

	for(int z = 0;z < selected1.count();z++)
	{

		QStandardItem*	lpItem	= m_lpSeriesListModel->itemFromIndex(selected1.at(z));
		QModelIndex		index	= lpItem->index();
		lp2->select(index, QItemSelectionModel::Select);
	}

	m_bProcessing	= false;
}

void cMainWindow::selectionChanged2(const QItemSelection& /*selected*/, const QItemSelection& /*deselected*/)
{
	if(m_bProcessing)
		return;

	m_bProcessing	= true;

	QItemSelectionModel*	lp1			= ui->m_lpSeriesList1->selectionModel();
	QItemSelectionModel*	lp2			= ui->m_lpSeriesList2->selectionModel();
	QModelIndexList			selected2	= lp2->selectedIndexes();

	lp1->clearSelection();

	for(int z = 0;z < selected2.count();z++)
	{
		QStandardItem*	lpItem	= m_lpSeriesListModel->itemFromIndex(selected2.at(z));
		QModelIndex		index	= lpItem->index();
		lp1->select(index, QItemSelectionModel::Select);
	}

	m_bProcessing	= false;
}

void cMainWindow::scrollbarValueChanged1(int value)
{
	if(m_bProcessing)
		return;

	m_bProcessing	= true;

	ui->m_lpSeriesList2->verticalScrollBar()->setValue(value);

	m_bProcessing	= false;
}

void cMainWindow::scrollbarValueChanged2(int value)
{
	if(m_bProcessing)
		return;

	m_bProcessing	= true;

	ui->m_lpSeriesList1->verticalScrollBar()->setValue(value);

	m_bProcessing	= false;
}
