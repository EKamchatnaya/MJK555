#include "database.h"
#include  <stdexcept>
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QPixmap>

Database::Database(QString dbname, QObject *parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbname);
    if (!db.open()){
        throw std::invalid_argument("Невозможно открыть базу данных");
    }
    fetchAll (QModelIndex());
}

Database::~Database()
{
    db.close();
}

int Database::rowCount(const QModelIndex &parent) const
{
    if (!parent.isValid()){
        const DataWrapper *parent_pointer = &d;
        return parent_pointer->count;
    }

    const DataWrapper *parent_pointer = dataForIndex (parent);
    return parent_pointer->count;
}

int Database::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED (parent);
    return 1;
}

QModelIndex Database::index(int row, int column, const QModelIndex &parent) const
{
    if (column != 0) {
        return QModelIndex();
    }
    if (!parent.isValid()) {
        return createIndex (row, column, d.children[row]);
    }
    const DataWrapper *parent_pointer = dataForIndex (parent);
    if (parent_pointer->type == IMAGE) {
        return QModelIndex();
    }
    if (parent_pointer->children.size() <= row)
        return QModelIndex();
    return createIndex (row, column, parent_pointer->children[row]);
}

QVariant Database::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return {};
    if (role == Qt::DisplayRole) {
         if (index.isValid()) {
             if (!index.isValid()) {
                 return &d;
             }
             const DataWrapper *elem = dataForIndex (index);
             switch (elem->type) {
                 case TERM:
                 case COURSE:
                 case THEME:
                     return static_cast<HData*> (elem->data)->name;
                 case IMAGE:
                     return static_cast<IData*> (elem->data)->path;
                 default:
                     break;
             }
         }
     }
    if (role == Qt::DecorationRole||Qt::SizeHintRole){
         if (index.isValid()) {
             const DataWrapper *elem = dataForIndex (index);
                 if (elem->type == IMAGE){
                     QPixmap pix;
                     pix.load(static_cast<IData*> (elem->data)->path);
                     if (role == Qt::DecorationRole){
                         return pix;
                     }
                     else {
                         return pix.size()/2;
                     }
             }
         }
     }
    return QVariant();
}

QModelIndex Database::parent(const QModelIndex &child) const
{
    if (!child.isValid()) return QModelIndex();
    const DataWrapper *child_pointer =
            dataForIndex (child);
    if (!child_pointer->parent) return QModelIndex();
    return createIndex (child_pointer->parent->number, 0, static_cast<void *> (child_pointer->parent));

}


void Database::fetchAll (const QModelIndex &parent)
{
    DataWrapper *data; //= dataForIndex (parent);
    if (!parent.isValid()) {
        data = &d;
    }
    else {
        data = static_cast<DataWrapper *> (parent.internalPointer());
    }
    data->children.clear();
    QSqlQuery query;
    if (data->type != THEME) {
        query.prepare ("SELECT * from hierarchy_tree where pid = :id ORDER BY number");
    } else {
        query.prepare ("SELECT * from images where pid = :id ORDER BY number");
    }
    query.bindValue (":id", data->id);
    query.exec();
    while (query.next()) {
        auto id = query.value ("id").toUInt();
        auto comment = query.value ("comment").toString();
        QStringList tags = query.value ("tags").toStringList();
        auto number = query.value ("number").toInt();
        switch (data->type) {
        case ROOT:
        case TERM:
        case COURSE: {
            auto type = query.value ("type").toInt();
            auto name = query.value ("text").toString();
            data->children.append (
                        new DataWrapper{id, (h_type)type,
                                        new HData{name, comment, tags},
                                        number, data, {}, getChildrenCount (h_type(type),id)});
            break;
        }
        case THEME: {
            auto path = query.value ("path").toString();
            data->children.append (
                        new DataWrapper{id, IMAGE,
                                        new IData{path, comment, tags},
                                        number, data, {}, getChildrenCount (IMAGE,id)});
            break;
        }
        default:
            break;
        }
    }
    data->count = data->children.size();
}

const DataWrapper *Database::dataForIndex (const QModelIndex &index) const
{
    if (!index.isValid()) {
        return &d;
    }
    return static_cast<DataWrapper *> (index.internalPointer());
}

DataWrapper *Database::dataForIndex (const QModelIndex &index)
{
    if (!index.isValid()) {
        return &d;
    }
    return static_cast<DataWrapper *> (index.internalPointer());
}

int Database::getChildrenCount (h_type type, int pid) const
{
    QSqlQuery query;
    switch (type) {
    case ROOT:
    case TERM:
    case COURSE:
        query.prepare ("SELECT COUNT (*) from hierarchy_tree where pid = :id ");
        break;
    case THEME:
        query.prepare ("SELECT COUNT (*) from images where pid = :id ");
        break;
    case IMAGE:
        return 0;
    default:
        break;
    }
    query.bindValue (":id", pid);
    query.exec();
    query.next();
    qDebug() << query.executedQuery();
    qDebug() << query.lastError();
    int count = query.value (0).toInt();
    return count;
}

void Database::fetchMore (const QModelIndex &parent)
{
    fetchAll (parent);
}

bool Database::canFetchMore (const QModelIndex &parent) const
{
    const DataWrapper *data; //= dataForIndex (parent);
    if (!parent.isValid()) {
        data = &d;
    }
    else {
        data = static_cast<DataWrapper *> (parent.internalPointer());
    }
    return data->children.size() < data->count;
}

