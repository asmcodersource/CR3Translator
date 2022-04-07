#ifndef LISTWIDGET_H
#define LISTWIDGET_H
#include <thread>
#include <QWidget>
#include <QLayout>
#include <QTableView>
#include <QHeaderView>
#include <QMouseEvent>
#include <QStandardItemModel>
#include <QAbstractItemDelegate>
#include <QResizeEvent>
#include "scrollbar.h"
#include <list>
#include <algorithm>
#include <QMessageBox>
#include "advancetranslatepair.h"
#include "settingswidget.h"
#include "tableviewfilters.h"


/*
  Клас, що реалізовує представлення для даних.
*/
class ListWidget : public QWidget
{
    Q_OBJECT
    QStandardItemModel* model;
    QTableView* table_view;
    SettingsWidget* settings_widget;
    QVBoxLayout* layout_main;
    ScrollBar* vertical_scrollbar;
    ListWidgetColumnResizeFilter* column_resize_filter;
    ListWidgetRowResizeFilter* row_resize_filter;
    QString opened_file;
    QString file_data;
    bool edited = false;

public:
    ListWidget(QWidget *parent = nullptr);
    ~ListWidget();

    void translateFile();
    void resizeEvent(QResizeEvent* event);
    void loadFromFile(QString file_path);
    void saveToFile();

    void setSettingsWidget(SettingsWidget* ptr);
};


#endif // LISTWIDGET_H
