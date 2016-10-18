#ifndef CPICTURESTHREAD_H
#define CPICTURESTHREAD_H


#include "cmessagedialog.h"
#include "cserie.h"

#include <QThread>
#include <QMutexLocker>
#include <QList>


class cPicturesThread : public QThread
{
	Q_OBJECT

public:
	explicit				cPicturesThread();

	void					setData(cMessageDialog *lpMessageDialog, const QList<cSerie*>& list);
public slots:
	void					stop();

signals:
	void					picturesMessage(const QString& szMessage, const qint32 &iProgress);
	void					picturesAppendMessage(const QString& szMessage);

private:
	QMutex					m_mutex;
	bool					m_bStop;
	QList<cSerie*>			m_items;
	QWidget*				m_lpParent;

	void					run();
};

#endif // CPICTURESTHREAD_H
