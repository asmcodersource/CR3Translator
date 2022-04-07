#ifndef TRANSLATEPAIR_H
#define TRANSLATEPAIR_H

#include <QObject>
#include <QtNetwork\QNetworkAccessManager>
#include <QtNetwork\QNetworkRequest>
#include <QtNetwork\QNetworkReply>

struct Pair{
    QString key;
    QString value;
};

/*
 Абстрактний клас, що надає інтерфейс для класів перекладу
*/
class TranslatePair : public QObject
{
    Q_OBJECT
protected:
    Pair original_pair, translated_pair;

public:
    explicit TranslatePair(QObject *parent = nullptr);
    virtual ~TranslatePair() = default;
    virtual Pair getTranslatedPair() = 0;
    virtual Pair getOriginalPair() = 0;

public slots:
    virtual void makeTranslate() = 0;
    virtual void getReply(QNetworkReply* replay) = 0;
signals:
    void translateFinished(TranslatePair*);
};

#endif // TRANSLATEPAIR_H
