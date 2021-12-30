#include "QmlAdapter.h"

GameOfLifeModel* QmlAdapter::getGameOfLifeModel() const
{
    return m_gameOfLifeModel;
}

QmlAdapter::QmlAdapter()
{
    m_gameOfLifeModel = new GameOfLifeModel();
}

QmlAdapter::~QmlAdapter()
{
    delete m_gameOfLifeModel;
}

void QmlAdapter::setGameOfLifeModel(GameOfLifeModel* newGameOfLifeModel)
{
    if (m_gameOfLifeModel == newGameOfLifeModel)
    {
        return;
    }

    m_gameOfLifeModel = newGameOfLifeModel;
    Q_EMIT gameOfLifeModelChanged();
}
