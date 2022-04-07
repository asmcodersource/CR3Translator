#ifndef FILETREEWIDGET_H
#define FILETREEWIDGET_H

#include <QDir>
#include <QWidget>
#include <QHeaderView>
#include <QTreeView>
#include <QLayout>
#include <QFileSystemModel>
#include <QAbstractItemView>
#include <QLabel>
#include "scrollbar.h"

class FileTreeWidget : public QWidget
{
    Q_OBJECT
    ScrollBar* vertical_scrollbar;
    QFileSystemModel file_system_model;
    QTreeView* files_view;
    QBoxLayout* layout;
public:
    explicit FileTreeWidget(QWidget *parent = nullptr);
    void setDir(QString dir_path);
signals:
    void openFile(QString file_path);
};

#endif // FILETREEWIDGET_H
