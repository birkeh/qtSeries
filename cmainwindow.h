#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H


#include "cserie.h"
#include "cupdatethread.h"
#include "cpicturesthread.h"

#include <QMainWindow>
#include <QList>
#include <QSqlDatabase>

#include <QCloseEvent>

#include <QTime>


namespace Ui {
class cMainWindow;
}

class cMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit cMainWindow(QWidget *parent = 0);
	~cMainWindow();

private slots:
	void				on_m_lpSeriesList_customContextMenuRequested(const QPoint &pos);
	void				onActionAdd();
	void				onActionUpdate();
	void				onActionDelete();
	void				onActionEdit();
	void				onActionGotoIMDB();
	void				onActionGotoDownload();
	void				onActionCopyDownload();
	void				onActionLoadPictures();
	void				on_m_lpSeriesList_doubleClicked(const QModelIndex &index);

	void				updateMessage(const QString& szMessage, const qint32 &iProgress);
	void				updateAppendMessage(const QString& szMessage);
	void				updateDone();

	void				picturesMessage(const QString& szMessage, const qint32 &iProgress);
	void				picturesAppendMessage(const QString& szMessage);
	void				picturesDone();
private:
	Ui::cMainWindow*	ui;
	cSerieList			m_serieList;
	QSqlDatabase		m_db;

	QString				m_szOldSelected;

	cMessageDialog*		m_lpMessageDialog;
	cUpdateThread*		m_lpUpdateThread;
	cPicturesThread*	m_lpPicturesThread;

	QTime				m_timer;

	void				initDB();
	void				loadDB();
	void				displaySeries();

	bool				runEdit(cSerie *lpSerie, QString& szDownload);
protected:
	void				closeEvent(QCloseEvent * event);
};

#endif // CMAINWINDOW_H