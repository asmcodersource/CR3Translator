#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QComboBox>
#include <QPushButton>


/*
 Клас, що реалізовує вікно налаштувань.
*/
class SettingsWidget : public QWidget
{
    Q_OBJECT
    QVBoxLayout* main_layout;
    QStringList langs_strings{"Англійська", "Російська", "Іспанська", "Німецька", "Українська"};
    QStringList langs_codes{"en", "ru", "es", "de", "uk"};
    QComboBox* source_lang_box, *result_lang_box;
    QPushButton* finish_button;
    QString current_source_lang = "en";
    QString current_translate_lang = "uk";
public:
    explicit SettingsWidget(QWidget *parent = nullptr);
    QString getSourceLang();
    QString getTargetLang();

protected slots:
    void buttonPressed();
signals:

};

#endif // SETTINGSWIDGET_H
