#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSizePolicy>
#include <QLabel>
#include <QPainter>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QTime>
#include <QLayout>
#include <QEnterEvent>
#include <vector>
#include <QAbstractButton>
#include <QApplication>
#include <QMenu>
#include <QAction>
#include <QMenuBar>
#include <QMessageBox>
#include <QFileDialog>
#include "mainwidget.h"
#include "settingswidget.h"
using namespace std;

enum buttonType{none, close, maximize, minimaze, hide};

class WindowTitleButton: public QAbstractButton{
private:
    QColor background_color;
public:
    buttonType button_type{none};
    WindowTitleButton(QAbstractButton* parent = nullptr );
    void enterEvent(QEnterEvent* event );
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void leaveEvent(QEvent* event );
    void paintEvent(QPaintEvent *event);
    void setButtonType(buttonType type);
};


class WindowTitle: public QWidget{
Q_OBJECT

    QLabel* window_title;
    QGridLayout* main_layout;
    QPoint first_press, first_pos;
    QWidget* main_window;
    bool left_button_pressed;
    QHBoxLayout* buttons_layer;

public:
    WindowTitleButton button_close, button_maximaze, button_hide;

    WindowTitle(QWidget* parent = nullptr );
    void setMainWindow(QWidget* main_window);
    void paintEvent( QPaintEvent* event );
    void enterEvent(QEnterEvent* enterEvent);

    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent *event);

signals:
    void signal_closeClicked();
    void signal_hideClicked();
    void signal_maximazeClicked();


public slots:
    void slot_buttonCloseClicked();
};

class MainWindow: public QWidget{
    Q_OBJECT
private:
    SettingsWidget* settings_widget;
    QVBoxLayout* main_layout;
    WindowTitle* window_title;
    MainWidget* main_widget;
    QMenuBar* menu_bar;
    QMenu* menu_file;
    bool top = false, down = false, left = false, right = false;
public:


    MainWindow(QWidget* parent = nullptr);
    void paintEvent(QPaintEvent *event) override;
    void enterEvent(QEnterEvent* enterEvent);
    void mouseReleaseEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent* event) override;

public slots:
    void slot_maximazeButtonClicked();

};

#endif // MAINWINDOW_H
