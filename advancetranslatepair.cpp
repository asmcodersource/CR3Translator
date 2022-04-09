#include "advancetranslatepair.h"
using namespace std;

AdvanceTranslatePair::AdvanceTranslatePair(QString key, QString value, TranslateSettings settings){
    source_lang = settings.lang_source;
    target_lang = settings.lang_target;
    translate_server_adrress = settings.translate_server;
    network = new  QNetworkAccessManager();
    request = new QNetworkRequest();
    original_pair.key = key;
    original_pair.value = value;
    translated_pair.key = key;
    translated_pair.value = value;

    // capture tags
    QRegularExpression reA1(R"((((@|#)(([\p{L}\p{N}_])+)|(\#)))|(\"))");
    QRegularExpressionMatchIterator i1 = reA1.globalMatch(original_pair.value);
    while (i1.hasNext()) {
    QRegularExpressionMatch match = i1.next();
        if (match.hasMatch()) {
           QString result = match.captured();
           capture_list.append(result);
        }
    }

    // capture bracet text
    QRegularExpression reA2(R"(\[(.*?)\])");
    QRegularExpressionMatchIterator i2 = reA2.globalMatch(original_pair.value);
    while (i2.hasNext()) {
    QRegularExpressionMatch match = i2.next();
        if (match.hasMatch()) {
           QString result = match.captured();
           braced_capture_list.append(result);
        }
    }

    // capture variable text
    QRegularExpression reA3(R"(\\$(.*?)\$\)");
    QRegularExpressionMatchIterator i3 = reA3.globalMatch(original_pair.value);
    while (i3.hasNext()) {
    QRegularExpressionMatch match = i3.next();
        if (match.hasMatch()) {
           QString result = match.captured();
           variable_capture_list.append(result);
        }
    }

    QObject::connect(network, &QNetworkAccessManager::finished, this, &AdvanceTranslatePair::getReply);
}

void AdvanceTranslatePair::makeTranslate(){
    QString stringForTranslate = translated_pair.value;
    for( auto& element: capture_list ){
        stringForTranslate.replace(element, "()");
    }
    QString ulr_string = "https://" + translate_server_adrress + "/api/v1/" + source_lang + "/" + target_lang;
    QUrlQuery url_query;
    url_query.addQueryItem("", QUrl::toPercentEncoding(stringForTranslate));
    ulr_string = ulr_string + "/" + url_query.toString().right(url_query.toString().length() - 1);
    QUrl url(ulr_string);

    request->setUrl(url);
    network->get(*request);
}


void AdvanceTranslatePair::getReply(QNetworkReply* replay){
    QJsonDocument replay_json = QJsonDocument::fromJson(replay->readAll());
    QJsonObject replay_json_object(replay_json.object());
    if( !replay_json_object.contains("translation")){
        static int i = 0;
        qDebug() << "Translation: " << original_pair.value;
        qDebug() << "Translation error #" << i++ << "!";
        qDebug() << "------------------------------------";
        emit AdvanceTranslatePair::translateFinished(this);
        translated_pair.value = original_pair.value;
        return;
    }
    QString translate_result = replay_json.operator[](QString("translation")).toString();
    for( auto& element: capture_list ){
        uint64_t index = translate_result.indexOf("()");
        translate_result.remove(index, 2);
        translate_result.insert(index, element);
    }

    QRegularExpression reA2(R"(\[(.*?)\])");
    QRegularExpressionMatchIterator i2 = reA2.globalMatch(translate_result);
    while (i2.hasNext()) {
    QRegularExpressionMatch match = i2.next();
        if (match.hasMatch()) {
              if( braced_capture_list.isEmpty() )
                  break;
           QString result = match.captured();
           uint64_t index = translate_result.indexOf(result);
           translate_result.remove(index, result.length());
           translate_result.insert(index, braced_capture_list.front());
           braced_capture_list.pop_front();
        }
    }

    QRegularExpression reA3(R"(\\$(.*?)\$\])");
    QRegularExpressionMatchIterator i3 = reA3.globalMatch(translate_result);
    while (i3.hasNext()) {
    QRegularExpressionMatch match = i3.next();
        if (match.hasMatch()) {
            if( variable_capture_list.isEmpty() )
                break;
           QString result = match.captured();
           uint64_t index = translate_result.indexOf(result);
           translate_result.remove(index, result.length());
           translate_result.insert(index, variable_capture_list.front());
           variable_capture_list.pop_front();
        }
    }

    translated_pair.value = translate_result;
    network->deleteLater();
    replay->deleteLater();
    emit AdvanceTranslatePair::translateFinished(this);
}

Pair AdvanceTranslatePair::getTranslatedPair(){
    return translated_pair;
}

Pair AdvanceTranslatePair::getOriginalPair(){
    return original_pair;
}
