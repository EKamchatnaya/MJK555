#ifndef DATABASECREATE_H
#define DATABASECREATE_H

#include <QtSql>
class databasecreate
{
public:
    databasecreate();
    QtSqlDatabase CreateDB(char* name);
};

#endif // DATABASECREATE_H
