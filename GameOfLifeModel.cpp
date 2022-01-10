#include "GameOfLifeModel.h"
#include <QRandomGenerator>
#include <QFile>
#include <QRect>
#include <QSize>
#include <QThread>
#include <QtTest/QtTest>
#include <QDateTime>
#include <QDebug>

/////////////////////////////////////////////////////////
/// CONSTANTS
/////////////////////////////////////////////////////////
static const char IDENTIFIER_LIVING_CELL = '1';
static const char IDENTIFIER_DEAD_CELL = '0';
static const char IDENTIFIER_NEXT_LINE = '\n';

GameOfLifeModel::GameOfLifeModel(bool editable, QObject* parent)
    : QAbstractTableModel(parent)
    , m_isEditable(editable)
{
    m_currentStateContainer.resize(size, false);
}

bool GameOfLifeModel::isEditable() const
{
    return m_isEditable;
}

QHash<int, QByteArray> GameOfLifeModel::roleNames() const
{
    return {
        {CellRole, "value"}};
}

int GameOfLifeModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
    {
        return 0;
    }
    return height;
}

int GameOfLifeModel::columnCount(const QModelIndex& parent) const
{
    if (parent.isValid())
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
    return QVariant(m_currentStateContainer[cellIndex({index.column(),
                                                       index.row()})]);
}

bool GameOfLifeModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (role != CellRole || data(index, role) == value) // so it can't be set or it doesn't need to be set
    {
        return false;
    }

    m_currentStateContainer[cellIndex({index.column(), index.row()})] = value.toBool();
    Q_EMIT dataChanged(index, index, {role});

    return true;
}

Qt::ItemFlags GameOfLifeModel::flags(const QModelIndex& index) const
{
    Q_UNUSED(index)

    Qt::ItemFlags flag;

    if (m_isEditable)
    {
        flag = Qt::ItemIsEditable;
    }

    return flag;
}

void GameOfLifeModel::generatePattern()
{
    clearPattern();

    for (int i = 0; i < size; i++)
    {
        const quint32 randomNumber = QRandomGenerator::global()->generate() % 100;

        if (randomNumber < m_livingCellsAtBeginningAsPercentage)
        {
            m_currentStateContainer[i] = true;
        }
    }

    m_loopIsStopping = false;
    Q_EMIT dataChanged(index(0, 0), index(height - 1, width - 1), {CellRole});
}

void GameOfLifeModel::nextStep()
{
    qint64 startTime = QDateTime::currentMSecsSinceEpoch();

    StateContainer newStateContainer(size);

    for (int i = 0; i < size; i++)
    {
        const quint32 randomNumber = QRandomGenerator::global()->generate() % m_mutationConstant;

        bool currentState = m_currentStateContainer[i];
        int cellNeighboursCount = this->cellNeighboursCount(cellCoordinatesFromIndex(i));

        if (currentState)
        {
            newStateContainer[i] = cellNeighboursCount == TwoNeighbours || cellNeighboursCount == ThreeNeighbours; // 2 or 3, so the cell lives
        }
        else
        {
            newStateContainer[i] = cellNeighboursCount == ThreeNeighbours || randomNumber == 0U; // 3 so the cell gets alive or because of the mutationConstant
        }
    }

    m_currentStateContainer = std::move(newStateContainer);
    Q_EMIT dataChanged(index(0, 0), index(height - 1, width - 1), {CellRole});

    m_stepCount++;
    Q_EMIT stepCountChanged();

    qDebug() << QDateTime::currentMSecsSinceEpoch() - startTime << "ms";
}

void GameOfLifeModel::startInfiniteLoop()
{
    m_loopIsStopping = false;
    while (!m_loopIsStopping)
    {
        nextStep();
        QTest::qWait(m_delay);
    }
}

void GameOfLifeModel::startLoop()
{
    m_loopIsStopping = false;
    for (quint32 i = 0; i < m_loopCount && !m_loopIsStopping; i++)
    {
        nextStep();
        QTest::qWait(m_delay);
    }
}

bool GameOfLifeModel::loadFile(const QString fileName)
{
    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly))
    {
        return false;
    }

    QTextStream in(&file);
    loadPattern(in.readAll());
    file.close();

    return true;
}

void GameOfLifeModel::loadPattern(const QString& plainText)
{
    clearPattern();

    QStringList rows = plainText.split(IDENTIFIER_NEXT_LINE);
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
            m_currentStateContainer[cellIndex(cellPosition)] = line[x] == '1';
        }
    }

    Q_EMIT dataChanged(index(0, 0), index(height - 1, width - 1), {CellRole});
}

bool GameOfLifeModel::saveFile(const QString fileName)
{
    QFile file(fileName);

    if (!file.open(QIODevice::WriteOnly))
    {
        return false;
    }

    file.write(savePattern());
    file.close();

    return true;
}

void GameOfLifeModel::clearPattern()
{
    m_stepCount = 0U;
    Q_EMIT stepCountChanged();

    m_currentStateContainer.fill(false);
    Q_EMIT dataChanged(index(0, 0), index(height - 1, width - 1), {CellRole});
}

void GameOfLifeModel::stopLoop()
{
    m_loopIsStopping = true;
}

QByteArray GameOfLifeModel::savePattern()
{
    QByteArray rows;

    for (int i = 0; i < m_currentStateContainer.size(); i++)
    {
        if (i % width == 0 && i != 0)
        {
            rows.append(IDENTIFIER_NEXT_LINE);
        }

        if (m_currentStateContainer[i])
        {
            rows.append(IDENTIFIER_LIVING_CELL);
        }
        else
        {
            rows.append(IDENTIFIER_DEAD_CELL);
        }
    }

    return rows;
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

            if (m_currentStateContainer[cellIndex(neighbourPosition)])
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

int GameOfLifeModel::cellIndex(const QPoint& cellCoordinates)
{
    return cellCoordinates.y() * width + cellCoordinates.x();
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

quint32 GameOfLifeModel::getLoopCount() const
{
    return m_loopCount;
}

quint32 GameOfLifeModel::getDelay() const
{
    return m_delay;
}

void GameOfLifeModel::setDelay(quint32 newDelay)
{
    if (m_delay == newDelay)
    {
        return;
    }

    m_delay = newDelay;
    Q_EMIT delayChanged();
}
