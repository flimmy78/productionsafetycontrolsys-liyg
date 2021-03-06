#ifndef WORKTHREADER_H
#define WORKTHREADER_H

#include <QObject>
#include <QDebug>
#include <QThread>
#include <QSqlQuery>
#include <QSqlDatabase>
#include "readdatafrommysql/readdatafrommysql.h"
#include "../core/settings.h"

typedef QList<Import_Area> ImportAreaListType;

class WorkThreader : public QObject
{
    Q_OBJECT
public:
    explicit WorkThreader(QObject *parent = 0);
    QSqlDatabase data_base;
    ImportAreaListType importAreaList;
signals:
    void postImportAreaList(WorkThreader*);
public slots:
    void UpdataTablewidgetImportarea();
};

#endif // WORKTHREADER_H
