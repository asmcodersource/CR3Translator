#include "mainwidget.h"

MainWidget::MainWidget(QWidget *parent): QSplitter{parent}{
    file_tree_widget = new FileTreeWidget();
    list_widget = new ListWidget();
    setStyleSheet("width: 2px; background-color: rgb(55, 59, 64);");
    addWidget(list_widget);
    addWidget(file_tree_widget);
    setStretchFactor(0, 3);
}

void MainWidget::saveFile(){
    list_widget->saveToFile();
}

void MainWidget::setSettingsWidget(SettingsWidget* ptr){
    settings_widget = ptr;
    list_widget->setSettingsWidget(ptr);
}
