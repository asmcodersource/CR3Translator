#include "settingswidget.h"

SettingsWidget::SettingsWidget(QWidget *parent) : QWidget{parent}{
    setWindowTitle(tr("Налаштування"));

    main_layout = new QVBoxLayout;
    translate_address = new QLineEdit();
    source_lang_box = new QComboBox();
    result_lang_box = new QComboBox();
    finish_button   = new QPushButton(tr("Ок"));
    lang_source_key = new QLineEdit();
    lang_target_key = new QLineEdit();
    auto_change_key = new QCheckBox();
    auto_change_file_name = new QCheckBox();

    source_lang_box->addItems(langs_strings);
    result_lang_box->addItems(langs_strings);
    source_lang_box->setCurrentIndex(0);
    result_lang_box->setCurrentIndex(4);
    lang_target_key->setEnabled(false);
    lang_source_key->setEnabled(false);
    lang_source_key->setText("l_" + langs_strings[source_lang_box->currentIndex()].toLower());
    lang_target_key->setText("l_" + langs_strings[result_lang_box->currentIndex()].toLower());

    translate_address->setPlaceholderText("[ip:port] or [domain:port]");
    translate_address->setText("lingva.pussthecat.org");

    main_layout->addWidget(new QLabel(tr("Мова з якої перекладається")));
    main_layout->addWidget(source_lang_box);
    main_layout->addStretch(1);
    main_layout->addWidget(new QLabel(tr("Мова на яку перекладається")));
    main_layout->addWidget(result_lang_box);
    main_layout->addStretch(50);

    main_layout->addWidget(new QLabel(tr("Код початкової мови")));
    main_layout->addWidget(lang_source_key);
    main_layout->addStretch(1);
    main_layout->addWidget(new QLabel(tr("Код мови перекладу")));
    main_layout->addWidget(lang_target_key);
    main_layout->addStretch(50);

    main_layout->addWidget(new QLabel(tr("Сервер перекладу")));
    main_layout->addWidget(translate_address);
    main_layout->addStretch(200);

    auto_change_key->setText(tr("Автоматично змінити ключ мови"));
    auto_change_key->setChecked(true);
    main_layout->addWidget(auto_change_key);

    auto_change_file_name->setText(tr("Автоматично змінити ключ мови у назві файлу"));
    auto_change_file_name->setChecked(true);
    main_layout->addWidget(auto_change_file_name);

    main_layout->addWidget(finish_button);

    setMinimumSize(300,200);
    setLayout(main_layout);

    connect(finish_button, &QPushButton::pressed, this, &SettingsWidget::buttonPressed);
    connect(source_lang_box, &QComboBox::currentTextChanged, this, &SettingsWidget::updateStrings);
    connect(result_lang_box, &QComboBox::currentTextChanged, this, &SettingsWidget::updateStrings);
}

TranslateSettings SettingsWidget::getSettings(){
    TranslateSettings settings;
    settings.lang_source = langs_codes[source_lang_box->currentIndex()];
    settings.lang_target = langs_codes[result_lang_box->currentIndex()];
    settings.lang_key_source = lang_source_key->text();
    settings.lang_key_target = lang_target_key->text();
    settings.translate_server = translate_address->text();
    settings.auto_change_file_name = auto_change_file_name->isChecked();
    settings.auto_change_lang_key  = auto_change_key->isChecked();
    return settings;
}

void SettingsWidget::updateStrings(){
    lang_source_key->setText("l_" + langs_strings[source_lang_box->currentIndex()].toLower());
    lang_target_key->setText("l_" + langs_strings[result_lang_box->currentIndex()].toLower());
}

void SettingsWidget::buttonPressed(){
    this->hide();
}

