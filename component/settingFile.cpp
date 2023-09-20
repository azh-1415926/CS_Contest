#include "settingFile.h"

#include <QFile>

settingFile::settingFile(QObject *parent)
    : QObject(parent)
    , m_LoadFlag(0)
{
}

settingFile::~settingFile()
{
}

void settingFile::load(const QString &path)
{
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly|QFile::Text))
        return;
    /*
        error 用于接收解析错误
        stream 用于读取文件
        str 存储字符串格式的 json 文件
    */
    QJsonParseError error;
    QTextStream stream(&file);
    QString str=stream.readAll();
    file.close();
    /* 将 json 字符串解析为 QJson 文件对象，并存储错误信息 */
    m_Doc=QJsonDocument::fromJson(str.toUtf8(),&error);
    if(error.error!=QJsonParseError::NoError&&!m_Doc.isNull())
    {
        qDebug()<<"json parse error","json 格式错误!";
        return;
    }
    /* json 接收 QJson 文件对象里的 QJson 根节点 */
    m_Json=m_Doc.object();
    m_LoadFlag=1;
    emit loaded();
}

void settingFile::save(const QString &path)
{
    QFile file(path);
    /* 保存到 settings.json 文件中（只写、截断保存）  */
    if(file.open(QIODevice::WriteOnly|QIODevice::Truncate))
    {
        QTextStream stream(&file);
        #if (QT_VERSION < QT_VERSION_CHECK(6,0,0))
            stream.setCodec("utf-8");
        #elif (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
            stream.setEncoding(QStringConverter::Utf8);
        #endif
        m_Doc.setObject(m_Json);
        stream<<m_Doc.toJson();
        file.close();
        emit saved();
    }
    else
        qDebug()<<"error","save \""+path+"\" is failed!";
}