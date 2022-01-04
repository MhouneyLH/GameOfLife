#include "GameOfLifeModel.h"
#include <QRandomGenerator>
#include <QFile>
#include <QRect>
#include <QSize>
#include <QThread>
#include <QtTest/QtTest>

GameOfLifeModel::GameOfLifeModel(QObject* parent)
    : QAbstractTableModel(parent)
{
    clearPattern();
}

QHash<int, QByteArray> GameOfLifeModel::roleNames() const
{
    return {
        {CellRole, "value"}};
}

int GameOfLifeModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) // @TODO: what happens here???
    {
        return 0;
    }
    return height;
}

int GameOfLifeModel::columnCount(const QModelIndex& parent) const
{
    if (parent.isValid()) // @TODO: what happens here???
    {
        return 0;
    }
    return width;
}

QVariant GameOfLifeModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || role != CellRole)
    {
        return QVariant();
    }
    return QVariant(m_currentState[cellIndex({index.column(),
                                              index.row()})]);
}

bool GameOfLifeModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (role != CellRole || data(index, role) == value) // so it can't be set or it doesn't need to be set
    {
        return false;
    }

    m_currentState[cellIndex({index.column(), index.row()})] = value.toBool();
    Q_EMIT dataChanged(index, index, {role});

    return true;
}

Qt::ItemFlags GameOfLifeModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
    {
        return Qt::NoItemFlags;
    }
    return Qt::ItemIsEditable;
}

void GameOfLifeModel::generatePattern()
{
    setStepCount(0U);
    clearPattern();

    for (std::size_t i = 0; i < size; i++)
    {
        const quint32 randomNumber = (QRandomGenerator::global()->generate() % 100) + 1;

        if (randomNumber < m_livingCellsAtBeginningAsPercentage)
        {
            m_currentState[i] = true;
        }
    }

    m_loopIsStopping = false;
    Q_EMIT dataChanged(index(0, 0), index(height - 1, width - 1), {CellRole});
}

void GameOfLifeModel::nextStep()
{
    StateContainer newValues;

    for (std::size_t i = 0; i < size; i++)
    {
        bool currentState = m_currentState[i];
        int cellNeighboursCount = this->cellNeighboursCount(cellCoordinatesFromIndex(static_cast<int>(i)));

        newValues[i] = currentState == true ? cellNeighboursCount == 2 || cellNeighboursCount == 3 : cellNeighboursCount == 3;
    }

    m_currentState = std::move(newValues);
    setStepCount(m_stepCount + 1);
    Q_EMIT dataChanged(index(0, 0), index(height - 1, width - 1), {CellRole});
}

// @TODO: a conditon, when stopping, then another loop is possible

void GameOfLifeModel::startInfiniteLoop()
{
    m_loopIsStopping = false;
    while (!m_loopIsStopping)
    {
        nextStep();
        QTest::qWait(1);
    }
}

void GameOfLifeModel::startLoop()
{
    m_loopIsStopping = false;
    for (quint32 i = 0; i < m_loopCount && !m_loopIsStopping; i++)
    {
        nextStep();
        QTest::qWait(1);
    }
}

bool GameOfLifeModel::loadFile(const QString& fileName)
{
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly))
    {
        return false;
    }

    QTextStream in(&file);
    loadPattern(in.readAll());

    return true;
}

void GameOfLifeModel::loadPattern(const QString& plainText)
{
    clearPattern();

    QStringList rows = plainText.split('\n');
    QSize patternSize(0, rows.count());

    // knowing the length of each single row
    for (QString row : rows)
    {
        if (row.size() > patternSize.width())
        {
            patternSize.setWidth(row.size());
        }
    }

    QPoint patternLocation((width - patternSize.width()) / 2, (height - patternSize.height()) / 2);

    for (int y = 0; y < patternSize.height(); y++)
    {
        const QString line = rows[y];

        for (int x = 0; x < line.length(); x++)
        {
            QPoint cellPosition(x + patternLocation.x(), y + patternLocation.y());
            m_currentState[cellIndex(cellPosition)] = line[x] == 'O';
        }
    }

    Q_EMIT dataChanged(index(0, 0), index(height - 1, width - 1), {CellRole});
}

void GameOfLifeModel::clearPattern()
{
    m_currentState.fill(false);
    setStepCount(0U);

    Q_EMIT dataChanged(index(0, 0), index(height - 1, width - 1), {CellRole});
}

void GameOfLifeModel::stopLoop()
{
    m_loopIsStopping = true;
}

int GameOfLifeModel::cellNeighboursCount(const QPoint& cellCoordinates) const
{
    int count = 0;

    for (int x = -1; x < 2; x++)
    {
        for (int y = -1; y < 2; y++)
        {
            if (x == 0 && y == 0)
            {
                continue;
            }

            const QPoint neighbourPosition{cellCoordinates.x() + x, cellCoordinates.y() + y};
            if (!areCellCoordinatesValid(neighbourPosition))
            {
                continue;
            }

            if (m_currentState[cellIndex(neighbourPosition)])
            {
                count++;
            }

            if (count > 3)
            {
                return count;
            }
        }
    }

    return count;
}

bool GameOfLifeModel::areCellCoordinatesValid(const QPoint& cellCoordinates)
{
    return QRect(0, 0, width, height).contains(cellCoordinates);
}

QPoint GameOfLifeModel::cellCoordinatesFromIndex(int cellIndex)
{
    return {cellIndex % width, cellIndex / width};
}

size_t GameOfLifeModel::cellIndex(const QPoint& cellCoordinates)
{
    return std::size_t(cellCoordinates.y() * width + cellCoordinates.x());
}

int GameOfLifeModel::getLivingCellsAtBeginningAsPercentage() const
{
    return m_livingCellsAtBeginningAsPercentage;
}

void GameOfLifeModel::setLivingCellsAtBeginningAsPercentage(int newLivingCellsAtBeginningAsPercentage)
{
    if (m_livingCellsAtBeginningAsPercentage == newLivingCellsAtBeginningAsPercentage)
    {
        return;
    }

    m_livingCellsAtBeginningAsPercentage = newLivingCellsAtBeginningAsPercentage;
    Q_EMIT livingCellsAtBeginningAsPercentageChanged();
}

quint32 GameOfLifeModel::getLoopCount() const
{
    return m_loopCount;
}

void GameOfLifeModel::setLoopCount(quint32 newLoopCount)
{
    if (m_loopCount == newLoopCount)
    {
        return;
    }

    m_loopCount = newLoopCount;
    Q_EMIT loopCountChanged();
}

quint32 GameOfLifeModel::getStepCount() const
{
    return m_stepCount;
}

void GameOfLifeModel::setStepCount(quint32 newStepCount)
{
    if (m_stepCount == newStepCount)
    {
        return;
    }

    m_stepCount = newStepCount;
    Q_EMIT stepCountChanged();
}

bool GameOfLifeModel::getLoopIsStopping() const
{
    return m_loopIsStopping;
}

void GameOfLifeModel::setLoopIsStopping(bool newLoopIsStopping)
{
    if (m_loopIsStopping == newLoopIsStopping)
    {
        return;
    }

    m_loopIsStopping = newLoopIsStopping;
    Q_EMIT loopIsStoppingChanged();
}
