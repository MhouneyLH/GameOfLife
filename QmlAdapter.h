#ifndef QMLADAPTER_H4291BAF77F4649848DD8A6EFCA4223F1
#define QMLADAPTER_H4291BAF77F4649848DD8A6EFCA4223F1

#include <QObject>
#include <QImage>

class QmlAdapter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QImage image READ getImage WRITE setImage NOTIFY imageChanged)
    Q_PROPERTY(quint32 imageSizeX READ getImageSizeX WRITE setImageSizeX NOTIFY imageSizeXChanged)
    Q_PROPERTY(quint32 imageSizeY READ getImageSizeY WRITE setImageSizeY NOTIFY imageSizeYChanged)

public:
    QmlAdapter();

signals:
    void imageChanged();
    void imageSizeXChanged();
    void imageSizeYChanged();

private:
    QImage getImage() const;
    void setImage(QImage image);
    quint32 getImageSizeX() const;
    void setImageSizeX(quint32 imageSizeX);
    quint32 getImageSizeY() const;
    void setImageSizeY(quint32 imageSizeY);

private:
    QImage m_image;
    quint32 m_imageSizeX = 0U;
    quint32 m_imageSizeY = 0U;
};

#endif // QMLADAPTER_H4291BAF77F4649848DD8A6EFCA4223F1
