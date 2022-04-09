#include "mainwindow.h"


QString cssStyle =  R"(
font-family: "Comic Sans MS", cursive, sans-serif;
color: rgb(255, 255, 255);
font-weight: 400;
text-decoration: none;
font-style: italic;
font-variant: normal;
text-transform: none;
text-align: center;
)";

QString menu_style = R"(

QMenuBar{
    background-color: rgb(37, 41, 46);
}

QMenuBar::item{
    padding: 5px;
    background-color: rgb(37, 41, 46);
    border-right: 1px solid rgb(55, 55, 55);
    color: white;
}

QMenuBar::item:selected {
    background-color: rgb(47, 51, 56);
}

QMenuBar::item:disabled {
    background-color: rgb(43, 43, 43);
    color: rgb(125, 127, 128);
}

QMenuBar::item:pressed {
    background-color: white;
    color: black;
}


)";

#define wid 5

MainWindow::MainWindow(QWidget* parent): QWidget(parent){
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    main_layout = new QVBoxLayout();
    window_title = new WindowTitle();
    main_widget = new MainWidget();
    settings_widget = new SettingsWidget();
    main_widget->setSettingsWidget(settings_widget);


    menu_bar = new QMenuBar();
    menu_bar->setStyleSheet(menu_style);

    menu_file = new QMenu(tr("Файл"));

    action_open_dir = new QAction("Відкрити папку");
    connect(action_open_dir, &QAction::triggered, this, [this](){
       static QFileDialog* file_dialog = new QFileDialog();
       file_dialog->setFileMode(QFileDialog::Directory);
       file_dialog->setWindowTitle("Оберіть папку, з якою бажаєте працювати");
       file_dialog->setLabelText(QFileDialog::DialogLabel::Accept, "Відкрити");
       file_dialog->setLabelText(QFileDialog::DialogLabel::Reject, "Відміна");
       file_dialog->setOption(QFileDialog::ShowDirsOnly, true);
       file_dialog->show();
       connect(file_dialog, &QFileDialog::fileSelected, this, [this](QString dir_path){
           main_widget->file_tree_widget->setDir(dir_path);
       });
    });
    menu_file->addAction(action_open_dir);

    action_open_file = new QAction("Відкрити файл");
    connect(action_open_file, &QAction::triggered, this, [this](){
       static QFileDialog* file_dialog = new QFileDialog();
       file_dialog->setWindowTitle("Оберіть файл перекладу, з яким бажаєте працювати");
       file_dialog->setFileMode(QFileDialog::ExistingFile);
       file_dialog->setLabelText(QFileDialog::DialogLabel::Accept, "Відкрити");
       file_dialog->setLabelText(QFileDialog::DialogLabel::Reject, "Відміна");
       file_dialog->setLabelText(QFileDialog::DialogLabel::FileType, "Файли формату");
       file_dialog->setLabelText(QFileDialog::DialogLabel::FileName, "Ім'я файлу");
       file_dialog->setNameFilter("Файли формату YAML (*.yml)");
       file_dialog->show();
       connect(file_dialog, &QFileDialog::fileSelected, this, [this](QString file_name){
           main_widget->list_widget->loadFromFile(file_name);
       });
    });
    menu_file->addAction(action_open_file);

    action_save = new QAction("Зберегти файл");
    connect(action_save, &QAction::triggered, this, [this](){
       main_widget->saveFile();
    });
    menu_file->addAction(action_save);

    action_save_as = new QAction("Зберегти файл як...");
    connect(action_save_as, &QAction::triggered, this, [this](){
        static QFileDialog* file_dialog = new QFileDialog();
        file_dialog->setWindowTitle("Зберегти як");
        file_dialog->setLabelText(QFileDialog::DialogLabel::Accept, "Відкрити");
        file_dialog->setLabelText(QFileDialog::DialogLabel::Reject, "Відміна");
        file_dialog->setLabelText(QFileDialog::DialogLabel::FileType, "Файли формату");
        file_dialog->setLabelText(QFileDialog::DialogLabel::FileName, "Ім'я файлу");
        file_dialog->setNameFilter("Файли формату YAML (*.yml)");
        file_dialog->show();
        connect(file_dialog, &QFileDialog::fileSelected, this, [this](QString file_name){
            main_widget->list_widget->saveToFile(file_name);
        });
       main_widget->saveFile();
    });
    menu_file->addAction(action_save_as);

    action_exit = new QAction("Закрити");
    connect(action_exit, &QAction::triggered, this, [this](){
       QApplication::exit();
    });
    menu_file->addAction(action_exit);

    menu_bar->addMenu(menu_file);

    action_settings = new QAction("Налаштування");
    connect( action_settings, &QAction::triggered, settings_widget, &QWidget::show);
    menu_bar->addAction(action_settings);

    action_translate = new QAction("Перекласти поточний файл");
    connect( action_translate, &QAction::triggered, this, [&, this](){
        action_translate->setDisabled(true);
        action_translate->setEnabled(false);
        main_widget->list_widget->translateFile();
    });
    menu_bar->addAction(action_translate);


    action_about = new QAction("Про программу");
    connect(action_about, &QAction::triggered, this, [this](){
        static QMessageBox* dialog = new QMessageBox(QMessageBox::Icon::NoIcon,
                                                     "Про программу CR3 Translator",
                                                     """Це программа для автоматизації перекладу модів ігри Crusader Kings 3."
                                                     "\nВ якості перекладача використовується вільне API сервісу Lingva."
                                                     "\n"
                                                     "\n Курсова робота Новака Віталія Петровича"
                                                     "\n Технічний коледж ТНТУ 2022""");
        dialog->setMinimumSize(300,200);
        dialog->show();
    });
    menu_bar->addAction(action_about);

    connect(main_widget->file_tree_widget, &FileTreeWidget::openFile, main_widget->list_widget, &ListWidget::loadFromFile);
    connect(main_widget->list_widget, &ListWidget::signal_fileTranslated, this, [this](){
        action_translate->setDisabled(false);
        action_translate->setEnabled(true);
    });

    setLayout(main_layout);
    main_layout->setSpacing(0);
    main_layout->addWidget(window_title);
    main_layout->addWidget(menu_bar);
    main_layout->addWidget(main_widget, 9999);
    main_layout->setContentsMargins(5,5,5,5);

    connect(window_title, &WindowTitle::signal_closeClicked, this, &QWidget::close);
    connect(window_title, &WindowTitle::signal_hideClicked, this, [=](){this->showMinimized(); repaint();});
    connect(window_title, &WindowTitle::signal_maximazeClicked, this, &MainWindow::slot_maximazeButtonClicked);

    setMouseTracking(true);
    setAttribute( Qt::WA_TranslucentBackground, true );
    setAttribute(Qt::WA_Hover);
    window_title->setMainWindow(this);
}

void MainWindow::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setBrush(QColor(0, 0, 0, 2));
    painter.setPen(QColor(0, 0, 0, 2));
    painter.drawRect(0, 0 ,width(),height());

    painter.setBrush(QColor(48, 50, 54));
    painter.setPen(QColor(48, 50, 54));
    if( !this->isMaximized() )
        painter.drawRect(4, 4 ,width() - 4 - 5,height() - 4 - 5);
    else
        painter.drawRect(0, 0 ,width(),height());
}

void MainWindow::mouseReleaseEvent(QMouseEvent* event){
    right = false;
    down = false;
}

void MainWindow::mouseMoveEvent(QMouseEvent *event){
    QPoint cursor_pos = this->mapFromGlobal(QCursor().pos());
    setCursor(Qt::CustomCursor);
    if( event->buttons() != Qt::LeftButton ){
        if( cursor_pos.y() >= height() - wid)
            down = true;
        if( cursor_pos.x() >= width() - wid)
            right = true;
        if( right )
            setCursor(Qt::SizeHorCursor);
        if( down )
            setCursor(Qt::SizeVerCursor);
        if( right && down )
            setCursor(Qt::SizeFDiagCursor);
    }
    if ( event->buttons() == Qt::LeftButton  ){
        if( down == true )
            resize(this->size().width(), cursor_pos.y());
        if( right == true )
            resize(cursor_pos.x(), this->size().height());
        if( top == true ){
            if( cursor_pos.y() < 0 ){
                resize(this->size().width(),height() - cursor_pos.y());
                move(this->pos().x(), this->pos().y() + cursor_pos.y());
            }
        }
        if( left == true ){
            if( cursor_pos.x() < 0 ){
                resize(width() - cursor_pos.x(),this->size().height());
                move(this->pos().x() + cursor_pos.x(), this->pos().y());
            }
        }
    }
}

void MainWindow::enterEvent(QEnterEvent* event){
    top = false, down = false, left = false, right = false;
}

void MainWindow::leaveEvent(QEvent* event){
    setCursor(Qt::CustomCursor);

}

void MainWindow::slot_maximazeButtonClicked(){
    if( this->isMaximized() ){
        window_title->button_maximaze.setButtonType(buttonType::maximize);
        main_layout->setContentsMargins(5,5,5,5);
        showNormal();
    } else {
        window_title->button_maximaze.setButtonType(buttonType::minimaze);
        main_layout->setContentsMargins(0,0,0,0);
        showMaximized();
    }
}


//----------------------------------------------------------------------------

WindowTitle::WindowTitle(QWidget* parent ): QWidget(parent){
    setMinimumHeight(23);
    setMaximumHeight(23);
    window_title = new QLabel();
    main_layout = new QGridLayout();

    buttons_layer = new QHBoxLayout();
    buttons_layer->addWidget(&button_hide);
    buttons_layer->addWidget(&button_maximaze);
    buttons_layer->addWidget(&button_close);
    buttons_layer->setContentsMargins(0,0,0,0);
    button_hide.setButtonType(buttonType::hide);
    connect(&button_hide, &QAbstractButton::clicked, this, &WindowTitle::signal_hideClicked);
    button_maximaze.setButtonType(buttonType::maximize);
    connect(&button_maximaze, &QAbstractButton::clicked, this, &WindowTitle::signal_maximazeClicked);
    button_close.setButtonType(buttonType::close);
    connect(&button_close, &QAbstractButton::clicked, this, &WindowTitle::signal_closeClicked);

    window_title->setText("  CR3 Translator");
    window_title->setStyleSheet(cssStyle);
    main_layout->setContentsMargins(1,1,1,1);
    main_layout->setSpacing(0);
    main_layout->addWidget(window_title, 0, 0, Qt::AlignLeft);
    main_layout->addItem(buttons_layer, 0, 2, Qt::AlignRight);
    setLayout(main_layout);
    setMouseTracking(true);
    left_button_pressed = false;
    setAttribute(Qt::WA_Hover);
}

void WindowTitle::paintEvent(QPaintEvent* event){
    QPainter painter(this);
    painter.setBrush(QColor(28, 35, 41));
    painter.setPen(QColor(28, 35, 41));
    painter.drawRect(0,0,width(),height());
}

void WindowTitle::enterEvent(QEnterEvent* event){
    setCursor(Qt::CustomCursor);
}

void WindowTitle::setMainWindow(QWidget *main_window){
    this->main_window = main_window;
}

void WindowTitle::mousePressEvent(QMouseEvent* event){
    left_button_pressed = true;
    first_press = QCursor().pos();

    //qobject_cast<MainWindow*>(main_window)->slot_maximazeButtonClicked();
    main_window->showNormal();
    first_pos = main_window->pos();
}

void WindowTitle::mouseReleaseEvent(QMouseEvent* event){
    left_button_pressed = false;

}

void WindowTitle::mouseMoveEvent(QMouseEvent *event){
    setCursor(Qt::CustomCursor);
    if( left_button_pressed ){
        main_window->move(first_pos - (first_press - QCursor().pos()));

    }
}

void WindowTitle::slot_buttonCloseClicked(){
    //emit signal_closeClicked();
}

//-----------------------------------------------------------------------------


WindowTitleButton::WindowTitleButton(QAbstractButton* parent): QAbstractButton(parent){
    setFixedWidth(20);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    background_color = QColor(28, 35, 41);
    setMouseTracking(true);
    setAttribute(Qt::WA_Hover);
}

void WindowTitleButton::enterEvent(QEnterEvent* event ){
    switch(button_type){
    case buttonType::close:
        background_color = QColor(227, 25, 25);
        break;
    default:
        background_color = QColor(51, 62, 71);
        break;
    }
}

void WindowTitleButton::mousePressEvent(QMouseEvent *event){
   background_color.setRed(background_color.red() + 25);
   background_color.setGreen(background_color.green() + 25);
   background_color.setBlue(background_color.blue() + 25);
   QAbstractButton::mousePressEvent(event);
}

void WindowTitleButton::mouseReleaseEvent(QMouseEvent *event){
   background_color.setRed(background_color.red() - 25);
   background_color.setGreen(background_color.green() - 25);
   background_color.setBlue(background_color.blue() - 25);
   QAbstractButton::mouseReleaseEvent(event);
}

void WindowTitleButton::leaveEvent(QEvent* event ){
   background_color = QColor(28, 35, 41);
}

void WindowTitleButton::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setPen(background_color);
    painter.setBrush(background_color);
    painter.drawRect(0,0, width(), height());
    QPen pen;

    switch(button_type){
    case buttonType::close:
        pen.setColor(QColor(255,255,255));
        pen.setWidthF(1.3);
        painter.setPen(pen);
        painter.drawLine(width()/2.0 - width()/4.0,  height()/2.0 - height()/4.0, width()/2.0 + width()/4.0, height()/2.0 + height()/4.0);
        painter.drawLine(width()/2.0 - width()/4.0,  height()/2.0 + height()/4.0, width()/2.0 + width()/4.0, height()/2.0 - height()/4.0);
        break;
    case buttonType::hide:
        pen.setColor(QColor(255,255,255));
        pen.setWidthF(1.0);
        painter.setPen(pen);
        painter.drawLine(width()/2.0 - width()/4.0,  height()/2.0, width()/2.0 + width()/4.0, height()/2.0);
        break;
    case buttonType::maximize:
        pen.setColor(QColor(255,255,255));
        pen.setWidthF(1.0);
        painter.setPen(pen);
        painter.drawRect(width()/2.0 - width()/4.0,  height()/2.0 - height()/4.0, width()/2.0, height()/2.0);
        break;
    case buttonType::minimaze:
        pen.setColor(QColor(255,255,255));
        pen.setWidthF(1.0);
        painter.setPen(pen);
        painter.drawRect(width()/2.0 - width()/4.0,  height()/2.0 - height()/4.0, width()/2.0, height()/2.0);
        painter.drawRect(width()/2.0 - width()/4.0 + 1,  height()/2.0 - height()/4.0 - 1, width()/2.0, height()/2.0);
        break;
    }

}

void WindowTitleButton::setButtonType(buttonType type){
    button_type = type;
    repaint();
}
