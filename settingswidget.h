#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>


struct TranslateSettings{
    QString lang_source, lang_target;
    QString translate_server;
    QString lang_key_source, lang_key_target;
    bool auto_change_lang_key;
    bool auto_change_file_name;
};

/*
 Клас, що реалізовує вікно налаштувань.
*/
class SettingsWidget : public QWidget
{
    Q_OBJECT
    QVBoxLayout* main_layout;
    QStringList langs_strings{"English", "French", "German", "Korean", "Russian", "Simp_Chinese", "Spanish"};
    QStringList langs_codes{"en", "fr", "de", "ko", "ru", "zh", "es"};
    QComboBox* source_lang_box, *result_lang_box;
    QLineEdit* translate_address;
    QLineEdit* lang_source_key, *lang_target_key;
    QCheckBox* auto_change_key, *auto_change_file_name;
    QPushButton* finish_button;
public:
    explicit SettingsWidget(QWidget *parent = nullptr);
    TranslateSettings getSettings();

protected slots:
    void buttonPressed();
    void updateStrings();
signals:

};

#endif // SETTINGSWIDGET_H
