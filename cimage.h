#ifndef CIMAGE_H
#define CIMAGE_H


#include <QPixmap>


class cImage
{
public:
	cImage();
	QPixmap	getImage(const QString& szFileName);
private:
	QPixmap	downloadFile(const QString& szFileName);
};

#endif // CIMAGE_H
