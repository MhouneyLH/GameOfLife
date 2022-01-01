#ifndef GAMEOFLIFEMODEL_H1B7AD7A5506F4C6995FCE4824C51B436
#define GAMEOFLIFEMODEL_H1B7AD7A5506F4C6995FCE4824C51B436

#include <QAbstractTableModel>
#include <QPoint>

class GameOfLifeModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(quint32 livingCellsAtBeginningAsPercentage READ getLivingCellsAtBeginningAsPercentage WRITE setLivingCellsAtBeginningAsPercentage NOTIFY livingCellsAtBeginningAsPercentageChanged)
    Q_PROPERTY(quint32 loopCount READ getLoopCount WRITE setLoopCount NOTIFY loopCountChanged)
    Q_PROPERTY(bool loopIsStopping READ getLoopIsStopping WRITE setLoopIsStopping NOTIFY loopIsStoppingChanged)
    Q_PROPERTY(quint32 stepCount READ getStepCount WRITE setStepCount NOTIFY stepCountChanged)

public:
    explicit GameOfLifeModel(QObject* parent = nullptr);

    enum Roles
    {
        CellRole
    };
    Q_ENUM(Roles)

    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::DisplayRole) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

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
    Q_INVOKABLE void clearPattern();
    Q_INVOKABLE void stopLoop();

Q_SIGNALS:
    void livingCellsAtBeginningAsPercentageChanged();
    void loopCountChanged();
    void stepCountChanged();
    void loopIsStoppingChanged();

private:
    int getLivingCellsAtBeginningAsPercentage() const;
    void setLivingCellsAtBeginningAsPercentage(int newLivingCellsAtBeginningAsPercentage);
    quint32 getLoopCount() const;
    void setLoopCount(quint32 newLoopCount);
    quint32 getStepCount() const;
    void setStepCount(quint32 newStepCount);
    bool getLoopIsStopping() const;
    void setLoopIsStopping(bool newLoopIsStopping);

    int cellNeighboursCount(const QPoint& cellCoordinates) const;
    static bool areCellCoordinatesValid(const QPoint& cellCoordinates);
    static QPoint cellCoordinatesFromIndex(int cellIndex);
    static std::size_t cellIndex(const QPoint& cellCoordinates);

private:
    // @TODO: getting user input for the width and height
    static constexpr int width = 30;
    static constexpr int height = 30;
    static constexpr int size = width * height;
    typedef std::array<bool, size> StateContainer;

    StateContainer m_currentState;
    quint32 m_livingCellsAtBeginningAsPercentage = 50U;
    quint32 m_loopCount = 100U;
    quint32 m_stepCount = 0U;
    bool m_loopIsStopping = false;
};

#endif // GAMEOFLIFEMODEL_H1B7AD7A5506F4C6995FCE4824C51B436
