#include "settingswidget.h"

SettingsWidget::SettingsWidget(QWidget *parent) : QWidget{parent}{
    main_layout = new QVBoxLayout;
    source_lang_box = new QComboBox();
    result_lang_box = new QComboBox();
    finish_button   = new QPushButton("Ок");
    source_lang_box->addItems(langs_strings);
    result_lang_box->addItems(langs_strings);
    source_lang_box->setCurrentIndex(0);
    result_lang_box->setCurrentIndex(4);

    main_layout->addWidget(new QLabel("Мова з якої перекладається"));
    main_layout->addWidget(source_lang_box);
    main_layout->addStretch(1);
    main_layout->addWidget(new QLabel("Мова на яку перекладається"));
    main_layout->addWidget(result_lang_box);
    main_layout->addStretch(200);
    main_layout->addWidget(finish_button);

    setMinimumSize(200,150);
    setLayout(main_layout);

    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    connect(finish_button, &QPushButton::pressed, this, &SettingsWidget::buttonPressed);
}


void SettingsWidget::buttonPressed(){
    this->hide();
    current_source_lang    = langs_codes[source_lang_box->currentIndex()];
    current_translate_lang = langs_codes[result_lang_box->currentIndex()];
}

QString SettingsWidget::getSourceLang(){
    return current_source_lang;
}

QString SettingsWidget::getTargetLang(){
    return current_translate_lang;
}
