#ifndef DATABASE_H
#define DATABASE_H

#include <QAbstractItemModel>
#include <QObject>
#include <QSqlDatabase>
#include <QStringList>
#include <QModelIndex>

enum h_type {ROOT, TERM = 1, COURSE, THEME, IMAGE};


struct IData {

    QString path; /// Путь к изображению
    QString comments;
    QStringList tags;
};

struct HData {
    QString name;
    QString comments;
    QStringList tags;
};

struct DataWrapper {
    quint16 id;
    h_type type;
    void *data;
    int number;
    DataWrapper *parent;
    QList<DataWrapper *> children;
    int count;
};

class Database : public QAbstractItemModel
{
    Q_OBJECT
public:
    Database(QString dbname, QObject *parent = nullptr);
    ~Database();

    virtual int rowCount(const QModelIndex &parent) const;
    virtual int columnCount(const QModelIndex &parent) const;
    virtual QModelIndex index(int row, int column, const QModelIndex &parent) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual QModelIndex parent(const QModelIndex &child) const;
    void fetchMore(const QModelIndex &parent);
    bool canFetchMore(const QModelIndex &parent) const;


private:

    DataWrapper d {0, ROOT, nullptr, 0, nullptr, {},-1};
    void fetchAll(const QModelIndex &parent);
    int getChildrenCount (h_type type, int pid) const;
    const DataWrapper *dataForIndex(const QModelIndex &index) const;
    DataWrapper *dataForIndex(const QModelIndex &index);
    QSqlDatabase db;

};
#endif // DATABASE_H

