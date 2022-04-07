#include "filetreewidget.h"

QString files_view_stylesheet = R"(
QTreeView{
    background-color: rgb(36, 36, 36);
    gridline-color: rgb(55, 55, 55);
    color: white;

    font-size: 8pt;
    font-weight: 400;
    border: 0px;
    border-left: 1px solid rgba(0,0,0,0);
}



QLabel{
    font-weight:600;
    padding: 5px;
    background-color: rgb(55, 59, 64);
    color: white;
}

)";

FileTreeWidget::FileTreeWidget(QWidget *parent) : QWidget{parent}{
    setMinimumWidth(225);
    layout = new QVBoxLayout();
    layout->setSpacing(0);
    layout->setContentsMargins(0,0,0,0);
    setLayout(layout);
    file_system_model.setFilter(QDir::Files | QDir::AllDirs | QDir::NoDot | QDir::NoDotDot);


    files_view = new QTreeView();
    files_view->setSelectionMode(QAbstractItemView::ExtendedSelection);
    files_view->setModel(&file_system_model);
    files_view->setStyleSheet(files_view_stylesheet);
    for (int i = 1; i < file_system_model.columnCount(); ++i)
        files_view->hideColumn(i);
    QHeaderView* horizontal_header= files_view->header();

    horizontal_header->hide();

    vertical_scrollbar = new ScrollBar();
    files_view->setVerticalScrollBar(vertical_scrollbar);

    auto label = new QLabel("Огляд файлів");
    label->setFixedHeight(25);
    label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    label->setStyleSheet(files_view_stylesheet);
    layout->addWidget(label);
    layout->addWidget(files_view);

    connect(files_view, &QTreeView::doubleClicked, this, [&, this](const QModelIndex& index){
        emit openFile(file_system_model.filePath(index));
    });
}

void FileTreeWidget::setDir(QString dir_path){
    files_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    file_system_model.setRootPath(dir_path);
    files_view->setRootIndex(file_system_model.index(dir_path));
    files_view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
}
