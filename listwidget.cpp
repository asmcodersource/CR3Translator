#include "listwidget.h"

// стиль для таблиці даних
QString stylesheet = R"(
    QTableView{
        background-color: rgb(30, 30, 30);
        gridline-color: rgb(55, 55, 55);
        color: white;

        font-size: 8pt;
        font-weight: 400;
        border: solid 0px;
    }

    QHeaderView:section{
        background-color: rgb(45, 49, 54);
        border: solid 0px;
        border-right: 1px solid rgb(55, 55, 55);
        border-top: 1px solid rgb(55, 55, 55);
        color: white;
        font-size: 8pt;
        font-weight: 400;
        height: 25px;
    }
)";

// Конструктор, створює потрібні для класу об'єкти, та ініціалізовує їх
ListWidget::ListWidget(QWidget *parent) : QWidget(parent){
    setStyleSheet(stylesheet);
    model = new QStandardItemModel();
    QStringList headers_strings;
    headers_strings << "Ключ";
    headers_strings << "Початкове значення";
    headers_strings << "Нове значення";
    model->setHorizontalHeaderLabels(headers_strings);

    table_view = new QTableView();
    table_view->setModel(model);
    table_view->setFocusPolicy(Qt::NoFocus);
    table_view->setWordWrap(true);
    table_view->setTextElideMode(Qt::ElideMiddle);
    table_view->setContentsMargins(5,5,5,5);
    table_view->setSelectionBehavior(QAbstractItemView::SelectRows);


    QHeaderView* horizontal_header= table_view->horizontalHeader();
    horizontal_header->setSectionResizeMode(QHeaderView::Interactive);
    table_view->horizontalHeader()->setStretchLastSection(true);


    QHeaderView* vertical_header= table_view->verticalHeader();
    vertical_header->hide();
    vertical_header->sectionResizeMode(QHeaderView::Fixed);

    column_resize_filter = new ListWidgetColumnResizeFilter(this);
    column_resize_filter->setTableView(table_view);
    row_resize_filter = new ListWidgetRowResizeFilter(this);
    row_resize_filter->setTableView(table_view);

    table_view->setMouseTracking(true);
    QApplication::instance()->installEventFilter(column_resize_filter);
    QApplication::instance()->installEventFilter(row_resize_filter);

    layout_main = new QVBoxLayout();
    layout_main->addWidget(table_view);
    layout_main->setContentsMargins(0,0,0,0);

    vertical_scrollbar = new ScrollBar();
    table_view->setVerticalScrollBar(vertical_scrollbar);

    setLayout(layout_main);
    setMinimumWidth(480);

    connect(table_view->itemDelegate(), &QAbstractItemDelegate::closeEditor, this, [this](){
        edited = true;
    });
}

// Деструктор класу
ListWidget::~ListWidget(){

}

//  Метод загрузки данних із файлу
void ListWidget::loadFromFile(QString file_path){
    // Визначення формату файлу.
    if( file_path.right(3) != "yml" ){
        // Помилка.
        static QMessageBox* dialog = new QMessageBox(QMessageBox::Icon::Critical,"Помилка", "Файл не відповідає формату YML.");
        dialog->setMinimumSize(300,200);
        dialog->show();
        return;
    }
    // Зчитування файлу
    opened_file = file_path;
    QFile file(file_path, this);
    file.open(QIODevice::ReadOnly);
    file_data = file.readAll();
    file.close();
    // Об'єкти для пошуку пар "ключ:значення"
    QRegularExpression reA("(\\b.*:.*\\b.*)");
    QRegularExpressionMatchIterator i = reA.globalMatch(file_data);
    int row = 0;
    // Початкова ініціалізація моделі
    model->clear();
    QStringList headers_strings;
    headers_strings << "Ключ";
    headers_strings << "Початкове значення";
    headers_strings << "Нове значення";
    model->setHorizontalHeaderLabels(headers_strings);
    while (i.hasNext()) {
        QRegularExpressionMatch match = i.next();
        // Якщо найдено співпадіння
        if (match.hasMatch()) {
            // Отримуювання ключа, та значення
            QString pair = match.captured(0);
            QString key = pair.left(pair.indexOf(':'));
            QString value = pair.right( pair.length() - pair.indexOf(':') - 1);

            // Збереження цих значень у моделі.
            auto item_key = new QStandardItem();
            item_key->setText(key);
            item_key->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
            item_key->setForeground(QColor(222, 150, 18));
            item_key->setEditable(false);
            model->setItem(row, 0, item_key);

            auto item_value = new QStandardItem();
            item_value->setText(value);
            item_value->setTextAlignment(Qt::AlignTop | Qt::AlignLeft);
            item_value->setForeground(QColor(122, 204, 255));
            model->setItem(row, 1, item_value);
            item_value->setEditable(false);

            auto item_value_original = new QStandardItem();
            item_value_original->setText(value);
            item_value_original->setTextAlignment(Qt::AlignTop | Qt::AlignLeft);
            item_value_original->setForeground(QColor(122, 204, 255));
            model->setItem(row, 2, item_value_original);
            row++;

        }
    }
    // Перевизначння розмірів для колонок
    table_view->reset();
    table_view->setColumnWidth(0, width() / 100.0 * 25.0);
    table_view->setColumnWidth(1, width() / 100.0 * 35.0);
    table_view->resizeRowsToContents();
    edited = false;
}

// Метод, що встановлє вказівник на клас, який надає налаштування перекладу
void ListWidget::setSettingsWidget(SettingsWidget* ptr){
    settings_widget = ptr;
}

// Метод збереження даних у файл
void ListWidget::saveToFile(){
    // Відкриття файлу
    QFile file(opened_file, this);
    file.open(QIODevice::WriteOnly | QIODevice::Text);

    // Заміна значеннь оригінальних, на переклажені
    QString result;
    result = file_data;
    for( int32_t i = 0; i < model->rowCount(); i++ ){
        result.replace(model->data(model->index(i, 1)).toString(), model->data(model->index(i, 2)).toString());
    }

    // Перекодування у формат UTF-8, запис у файл
    QTextStream streamFileOut(&file);
    streamFileOut.setCodec("UTF-8");
    streamFileOut.setGenerateByteOrderMark(true);
    streamFileOut << result;
    streamFileOut.flush();

    // Закриття файлу
    file.close();
}

// Перевизначений метод реагування на зміни розміру
void ListWidget::resizeEvent(QResizeEvent* event){
    // Отримування минулого, та нового значення розмірів по ширині
    int32_t old_width = event->oldSize().width();
    int32_t new_width = width();

    // Пропорційна зміна розмірів колонок
    double scale_width = static_cast<double>(new_width) / old_width;
    table_view->setColumnWidth(0, table_view->columnWidth(0) * scale_width);
    table_view->setColumnWidth(1, table_view->columnWidth(1) * scale_width);
}

// Метод що перекладає відкритий файл
void ListWidget::translateFile(){
    // Статичне створення списку вказівників на об'єкти перекладу, та їх очищенання
    static std::list<AdvanceTranslatePair*> pairs;
    pairs.clear();

    static int32_t completed_translate;
    completed_translate = 0;
    for( int32_t i = 0; i < model->rowCount(); i++ ){
        while( completed_translate > i + 10 )
            std::this_thread::yield(); // Якщо переклад наступних десяти не здійснено, перемикаєм потік
        // Створення нового об'єкту перекладу, якому передається наступка пара ключа, та значення
        AdvanceTranslatePair* pair_ptr = new AdvanceTranslatePair(model->data(model->index(i, 0)).toString(), model->data(model->index(i, 1)).toString(),
                                                                  settings_widget->getSourceLang(),
                                                                  settings_widget->getTargetLang());
        // збережнення вказівника на цей об'єкт
        pairs.push_back(pair_ptr);
        // З'єднання сигналу "переклад закінчено", із лямба-функцією, що отримує перекладені значення, та зберігає результати
        connect(pair_ptr, &AdvanceTranslatePair::translateFinished, this, [&, this](TranslatePair* ptr_pair){
             qDebug() << "Pair translated " << completed_translate;
             auto pair_values = qobject_cast<AdvanceTranslatePair*>(ptr_pair)->getTranslatedPair();
             auto iter_this_pair = std::find(pairs.begin(), pairs.end(), qobject_cast<AdvanceTranslatePair*>(ptr_pair));
             int32_t row = std::distance(pairs.begin(), iter_this_pair);
             model->setData(model->index(row, 2), pair_values.value);
             completed_translate++;
        });

        // викликання методу, що провокує початок перекладу пари
        pair_ptr->makeTranslate();
    }
}

