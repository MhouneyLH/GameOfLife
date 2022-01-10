#ifndef GAMEOFLIFEMODEL_H1B7AD7A5506F4C6995FCE4824C51B436
#define GAMEOFLIFEMODEL_H1B7AD7A5506F4C6995FCE4824C51B436

#include <QAbstractTableModel>
#include <QByteArray>
#include <QPoint>
#include <QVector>

class GameOfLifeModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(quint32 livingCellsAtBeginningAsPercentage READ getLivingCellsAtBeginningAsPercentage WRITE setLivingCellsAtBeginningAsPercentage NOTIFY livingCellsAtBeginningAsPercentageChanged)
    Q_PROPERTY(quint32 loopCount READ getLoopCount WRITE setLoopCount NOTIFY loopCountChanged)
    Q_PROPERTY(quint32 stepCount READ getStepCount NOTIFY stepCountChanged)
    Q_PROPERTY(quint32 delay READ getDelay WRITE setDelay NOTIFY delayChanged)

public:
    explicit GameOfLifeModel(bool editable = true, QObject* parent = nullptr);

    // Model-Enum
    enum Roles
    {
        CellRole
    };
    Q_ENUM(Roles)

    // Cell-Enum
    enum CellStates : quint32
    {
        TwoNeighbours = 2U,
        ThreeNeighbours = 3U,
    };
    Q_ENUM(CellStates)

    /////////////////////////////////////////////////////////
    /// MODEL-FUNCTIONS
    /////////////////////////////////////////////////////////
    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::DisplayRole) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    bool isEditable() const;

    /////////////////////////////////////////////////////////
    /// GAME-MECHANIC-FUNCTIONS
    /////////////////////////////////////////////////////////
    // generates a random pattern
    Q_INVOKABLE void generatePattern();
    // saves the pattern into a file
    Q_INVOKABLE bool saveFile(const QString fileName);
    // displaying the current pattern as 0 and 1 in a QByteArray
    QByteArray savePattern();
    // loads the pattern from a file
    Q_INVOKABLE bool loadFile(const QString fileName);
    // read out the file and recreate the pattern of 0 and 1 into a normal pattern
    Q_INVOKABLE void loadPattern(const QString& plainText);
    // clears the current pattern
    Q_INVOKABLE void clearPattern();

    // starts an infinite loop
    Q_INVOKABLE void startInfiniteLoop();
    // starts a loop with a specific amount of loopSteps
    Q_INVOKABLE void startLoop();
    // stops every current loop
    Q_INVOKABLE void stopLoop();
    // calculates the pattern of each next step
    Q_INVOKABLE void nextStep();

Q_SIGNALS:
    void livingCellsAtBeginningAsPercentageChanged();
    void loopCountChanged();
    void stepCountChanged();
    void delayChanged();

private:
    /////////////////////////////////////////////////////////
    /// GETTER AND SETTER
    /////////////////////////////////////////////////////////
    int getLivingCellsAtBeginningAsPercentage() const;
    void setLivingCellsAtBeginningAsPercentage(int newLivingCellsAtBeginningAsPercentage);
    quint32 getLoopCount() const;
    void setLoopCount(quint32 newLoopCount);
    quint32 getStepCount() const;
    quint32 getDelay() const;
    void setDelay(quint32 newDelay);

    /////////////////////////////////////////////////////////
    /// CELL-CALCULATIONS
    /////////////////////////////////////////////////////////
    // calculates the living cellNeighbours of a cell
    int cellNeighboursCount(const QPoint& cellCoordinates) const;
    // checks if the given cellCoordinates are valid or not
    static bool areCellCoordinatesValid(const QPoint& cellCoordinates);
    // calculates the real coordinates based on a given index
    static QPoint cellCoordinatesFromIndex(int cellIndex);
    // calculates the index based on a given QPoint
    static int cellIndex(const QPoint& cellCoordinates);

private:
    static constexpr int width = 20;
    static constexpr int height = 20;
    static constexpr int size = width * height;
    static constexpr quint32 m_mutationConstant = 1000U;

    typedef QVector<int> StateContainer;

    StateContainer m_currentStateContainer;
    quint32 m_livingCellsAtBeginningAsPercentage = 50U;
    quint32 m_loopCount = 100U;
    quint32 m_stepCount = 0U;
    quint32 m_delay = 0U;

    bool m_loopIsStopping = false;
    bool m_isEditable = true;
};

#endif // GAMEOFLIFEMODEL_H1B7AD7A5506F4C6995FCE4824C51B436
