#ifndef CSEASONDELEGATE_H
#define CSEASONDELEGATE_H


#include <QStyledItemDelegate>


class cSeasonDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	cSeasonDelegate(QWidget *parent = 0) : QStyledItemDelegate(parent) {}

	void		paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
	QSize		sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
	//QWidget*	createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
	//void		setEditorData(QWidget *editor, const QModelIndex &index) const Q_DECL_OVERRIDE;
	//void		setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const Q_DECL_OVERRIDE;
};

#endif // CSEASONDELEGATE_H
