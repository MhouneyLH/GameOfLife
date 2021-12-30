#ifndef GAMEOFLIFEMODEL_H1B7AD7A5506F4C6995FCE4824C51B436
#define GAMEOFLIFEMODEL_H1B7AD7A5506F4C6995FCE4824C51B436

#include <QAbstractTableModel>
#include <QPoint>

// @GENERAL-TODO: commenting out the usage of the single functions

class GameOfLifeModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(quint32 livingCellsAtBeginningAsPercentage READ getLivingCellsAtBeginningAsPercentage WRITE setLivingCellsAtBeginningAsPercentage NOTIFY livingCellsAtBeginningAsPercentageChanged)
    Q_PROPERTY(quint32 loopCount READ getLoopCount WRITE setLoopCount NOTIFY loopCountChanged)

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

    Q_INVOKABLE void generatePattern();
    Q_INVOKABLE void nextStep();
    Q_INVOKABLE void startLoop();
    Q_INVOKABLE bool loadFile(const QString& fileName);
    Q_INVOKABLE void loadPattern(const QString& plainText);
    Q_INVOKABLE void clear();
    Q_INVOKABLE void clearPattern();

Q_SIGNALS:
    void livingCellsAtBeginningAsPercentageChanged();
    void loopCountChanged();

private:
    int getLivingCellsAtBeginningAsPercentage() const;
    void setLivingCellsAtBeginningAsPercentage(int newLivingCellsAtBeginningAsPercentage);
    quint32 getLoopCount() const;
    void setLoopCount(quint32 newLoopCount);

    int cellNeighboursCount(const QPoint& cellCoordinates) const;
    static bool areCellCoordinatesValid(const QPoint& cellCoordinates);
    static QPoint cellCoordinatesFromIndex(int cellIndex);
    static std::size_t cellIndex(const QPoint& cellCoordinates);

private:
    static constexpr int width = 50;
    static constexpr int height = 50;
    static constexpr int size = width * height;
    typedef std::array<bool, size> StateContainer;

    StateContainer m_currentState;
    quint32 m_livingCellsAtBeginningAsPercentage = 50U;
    quint32 m_loopCount = 100U;
};

#endif // GAMEOFLIFEMODEL_H1B7AD7A5506F4C6995FCE4824C51B436
