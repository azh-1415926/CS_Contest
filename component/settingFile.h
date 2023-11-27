#pragma once

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

class settingFile : public QObject
{
    Q_OBJECT

    private:
        QJsonObject m_Json;
        QJsonDocument m_Doc;
        int m_LoadFlag;

    public:
        explicit settingFile(QObject* parent = nullptr);
        ~settingFile();
        template<class T>
        void add(const QString& key,T value)
        {
            m_Json.insert(key,value);
        }
        template<class T>
        void add(const QString& key,QList<T> value)
        {
            QJsonArray array;
            for(int i=0;i<value.length();i++)
                array.push_back(value[i]);
            m_Json.insert(key,array);
        }
        template<class T1,class T2=T1>
        void add(const QString& key,QList<QPair<T1,T2>> value)
        {
            QJsonArray array;
            QJsonArray first;
            QJsonArray second;
            for(int i=0;i<value.length();i++)
            {
                first.push_back(value[i].first);
                second.push_back(value[i].second);
            }
            array.push_back(first);
            array.push_back(second);
            m_Json.insert(key,array);
        }
        /* 判定是否已导入 */
        inline bool isLoad() const { return m_LoadFlag==1; }
        /* 获取配置中的的对应属性值 */
        inline QJsonValue value(const QString& key) const { return m_Json.value(key); }

    protected:
        ;

    public slots:
        void load(const QString& path);
        void save(const QString& path);

    signals:
        void loaded();
        void saved();

    private:
        ;
};