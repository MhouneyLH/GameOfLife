#include "QmlAdapter.h"

QmlAdapter::QmlAdapter()
{
}

QImage QmlAdapter::getImage() const
{
    return m_image;
}

quint32 QmlAdapter::getImageSizeX() const
{
    return m_imageSizeX;
}

quint32 QmlAdapter::getImageSizeY() const
{
    return m_imageSizeY;
}

void QmlAdapter::setImage(QImage image)
{
    if (m_image == image)
        return;

    m_image = image;
    Q_EMIT imageChanged();
}

void QmlAdapter::setImageSizeX(quint32 imageSizeX)
{
    if (m_imageSizeX == imageSizeX)
        return;

    m_imageSizeX = imageSizeX;
    Q_EMIT imageSizeXChanged();
}

void QmlAdapter::setImageSizeY(quint32 imageSizeY)
{
    if (m_imageSizeY == imageSizeY)
        return;

    m_imageSizeY = imageSizeY;
    Q_EMIT imageSizeYChanged();
}
