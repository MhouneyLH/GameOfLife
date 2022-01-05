#ifndef GAMEOFLIFEMODEL_H1B7AD7A5506F4C6995FCE4824C51B436
#define GAMEOFLIFEMODEL_H1B7AD7A5506F4C6995FCE4824C51B436

#include <QAbstractTableModel>
#include <QByteArray>
#include <QPoint>
#include <QVector>

class GameOfLifeModel : public QAbstractTableModel
{
    Q_OBJECT
    // @TODO: adding the funtions to just have to set the default values in the .cpp-file instead of setting them also in the .qml-file
    Q_PROPERTY(quint32 livingCellsAtBeginningAsPercentage READ getLivingCellsAtBeginningAsPercentage WRITE setLivingCellsAtBeginningAsPercentage NOTIFY livingCellsAtBeginningAsPercentageChanged)
    Q_PROPERTY(quint32 loopCount READ getLoopCount WRITE setLoopCount NOTIFY loopCountChanged)
    Q_PROPERTY(quint32 stepCount READ getStepCount NOTIFY stepCountChanged)
    Q_PROPERTY(quint32 delay READ getDelay WRITE setDelay NOTIFY delayChanged)

public:
    explicit GameOfLifeModel(bool editable = true, QObject* parent = nullptr);

    enum Roles
    {
        CellRole
    };
    Q_ENUM(Roles)

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
    Q_INVOKABLE void generatePattern();
    // calculates the next Pattern
    Q_INVOKABLE void nextStep();
    Q_INVOKABLE void startInfiniteLoop();
    // starts a loop with a specific amount of steps
    Q_INVOKABLE void startLoop();
    // loads a file with a pattern in it
    Q_INVOKABLE bool loadFile(const QString& fileName);
    // reads out the pattern from the file
    Q_INVOKABLE void loadPattern(const QString& plainText);
    Q_INVOKABLE bool saveFile(const QString& fileName);
    Q_INVOKABLE void clearPattern();
    Q_INVOKABLE void stopLoop();
    QByteArray savePattern();

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
    int cellNeighboursCount(const QPoint& cellCoordinates) const;
    static bool areCellCoordinatesValid(const QPoint& cellCoordinates);
    static QPoint cellCoordinatesFromIndex(int cellIndex);
    static int cellIndex(const QPoint& cellCoordinates);

private:
    bool m_isEditable = true;

    static constexpr int width = 3;
    static constexpr int height = 3;
    static constexpr int size = width * height;
    typedef QVector<int> StateContainer;

    StateContainer m_currentStateContainer;
    quint32 m_livingCellsAtBeginningAsPercentage = 50U;
    quint32 m_loopCount = 100U;
    quint32 m_stepCount = 0U;
    bool m_loopIsStopping = false;
    quint32 m_delay = 0U;
    static constexpr quint32 m_mutationConstant = 500U;
};

#endif // GAMEOFLIFEMODEL_H1B7AD7A5506F4C6995FCE4824C51B436
