#ifndef QMLADAPTER_H4291BAF77F4649848DD8A6EFCA4223F1
#define QMLADAPTER_H4291BAF77F4649848DD8A6EFCA4223F1

#include "GameOfLifeModel.h"
#include <QObject>
#include <QImage>

class QmlAdapter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(GameOfLifeModel* gameOfLifeModel READ getGameOfLifeModel CONSTANT)

public:
    QmlAdapter();
    ~QmlAdapter();

private:
    GameOfLifeModel* getGameOfLifeModel() const;

private:
    GameOfLifeModel* m_gameOfLifeModel = nullptr;
};

#endif // QMLADAPTER_H4291BAF77F4649848DD8A6EFCA4223F1
