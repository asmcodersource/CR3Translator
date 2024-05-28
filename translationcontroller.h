#ifndef TRANSLATIONCONTROLLER_H
#define TRANSLATIONCONTROLLER_H

#include <QObject>

class TranslationController : public QObject
{
    Q_OBJECT
    QStringList langs_strings{"English", "French", "German", "Korean", "Russian", "Simp_Chinese", "Spanish"};
    int32_t current_lang;

public:
    explicit TranslationController(QObject *parent = nullptr);

signals:
    void languageChange(TranslationController* translation);
};

#endif // TRANSLATIONCONTROLLER_H
