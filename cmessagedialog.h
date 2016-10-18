#ifndef CMESSAGEDIALOG_H
#define CMESSAGEDIALOG_H

#include <QDialog>

namespace Ui {
class cMessageDialog;
}

class cMessageDialog : public QDialog
{
	Q_OBJECT

public:
	explicit cMessageDialog(QWidget *parent = 0);
	~cMessageDialog();

	void			setMessage(const QString& szMessage);
	void			addMessage(const QString& szMessage);
	void			setProgress(qint32 iMin, qint32 iMax);
	void			setProgress(qint32 iValue);

	QPushButton*	cancelButton();
private:
	Ui::cMessageDialog *ui;
};

#endif // CMESSAGEDIALOG_H
