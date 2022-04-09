#ifndef AdvanceTranslatePair_H
#define AdvanceTranslatePair_H

#include <QMainWindow>
#include <QRegExp>
#include <QRegularExpressionMatchIterator>
#include <QtNetwork\QNetworkAccessManager>
#include <QtNetwork\QNetworkRequest>
#include <QtNetwork\QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStringList>
#include <QUrlQuery>
#include <QDebug>
#include <QObject>
#include <iostream>
#include <settingswidget.h>
#include "translatepair.h"


/*
 Клас, що реалізоває переклад за допомогою зовнішнього інтерфейсу.
*/
class AdvanceTranslatePair: public TranslatePair{
    Q_OBJECT
    QNetworkAccessManager* network;
    QNetworkRequest* request;
    QStringList capture_list, braced_capture_list, variable_capture_list;
    QString source_lang, target_lang;
    QString translate_server_adrress;
public:
    AdvanceTranslatePair(QString key, QString value, TranslateSettings settings);
    ~AdvanceTranslatePair() = default;
    Pair getTranslatedPair() override;
    Pair getOriginalPair() override;

public slots:
    void makeTranslate() override;
    void getReply(QNetworkReply* replay) override;

signals:
    void translateReplay();
};

#endif //AdvanceTranslatePair_H
