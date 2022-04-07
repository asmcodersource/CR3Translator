#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QLayout>
#include <QSplitter>
#include "listwidget.h"
#include "settingswidget.h"
#include "filetreewidget.h"

class MainWidget : public QSplitter
{
    Q_OBJECT
public:
    ListWidget* list_widget;
    FileTreeWidget* file_tree_widget;
    SettingsWidget* settings_widget;

public:
    explicit MainWidget(QWidget *parent = nullptr);
    void setSettingsWidget(SettingsWidget* ptr);

public slots:
    void saveFile();
signals:

};

#endif // MAINWIDGET_H
