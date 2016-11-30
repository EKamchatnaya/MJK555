#include <QApplication>
#include <QTreeView>
#include "database.h"
#include <QHBoxLayout>
#include <QObject>
#include <QTableView>
#include <QQuickView>
#include <QUrl>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Database model("database");

  // Вывод формы
    QQuickView* s =new QQuickView();
    QUrl source ("qrc:/main.qml");
    s->setSource(source);

    /* QWidget widget;
         QHBoxLayout layout (&widget);
         QTreeView view;
         QTableView lview;
         layout.addWidget(&view);
         //layout.addWidget(&lview);
             view.setModel(&model);
             //lview.setModel(&model);
           //  lview.setRootIndex(ip->index(0,0,ip->index(0,0,QModelIndex())));
        // QObject::connect (&view,SIGNAL(clicked(QModelIndex)),&lview,SLOT(setRootIndex(QModelIndex)));
        // QObject::connect (&view,SIGNAL(entered(QModelIndex)),&lview,SLOT(setRootIndex(QModelIndex)));
        // QObject::connect (&view,SIGNAL(activated(QModelIndex)),&lview,SLOT(setRootIndex(QModelIndex)));
         widget.show();*/

 return a.exec();

}


