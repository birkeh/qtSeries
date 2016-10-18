/**
  * \class cMessageAnimateDialog
  *
  * \brief Class to display a modal message dialog.
  *
  * Class to display a modal message dialog. Title and message can be configured.
  * The dialog displays the message and adds up to 3 points zwice a second to the message.
  *
  * \ingroup KOOKY
  *
  * \author Herwig Birke
  *
  * \version 1.0
  *
  * \date $Date: 2016/02/09
  */

#ifndef CMESSAGEANIMATEDIALOG_H
#define CMESSAGEANIMATEDIALOG_H


#include <QDialog>
#include <QTimer>


namespace Ui {
class cMessageAnimateDialog;
}

class cMessageAnimateDialog : public QDialog
{
	Q_OBJECT

public:
	/**
	  * \brief constructor.
	  *
	  * \param parent
	  */
	explicit	cMessageAnimateDialog(QWidget *parent = 0);
	/**
	  * \brief destructor.
	  *
	  */
	~cMessageAnimateDialog();

	/**
	  * \brief Sets the title of the dialog.
	  *
	  * \param szTitle Title text
	  */
	void		setTitle(const QString& szTitle);
	/**
	  * \brief Sets the message of the dialog.
	  *
	  * \param szMessage Message text
	  */
	void		setMessage(const QString& szMessage);

private slots:
	/**
	  * \brief This function is called twice a second to update the displayed message.
	  *
	  */
	void		update();

private:
	Ui::cMessageAnimateDialog *ui; /*!< User interface of the dialog. */

protected:
	QString		m_szMessage; /*!< Message to be displayed */
	QTimer*		m_lpTimer; /*!< Timer object */
	int			m_iCount; /*!< Number of points currently being displayed */
	int			m_iDirection; /*!< Increase / decrease number of points */
};

#endif // CMESSAGEANIMATEDIALOG_H
