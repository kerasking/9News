#ifndef UTILITY_H
#define UTILITY_H

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QPoint>
#include <QSettings>
#include <QNetworkConfigurationManager>
#include <QPointer>
#if(QT_VERSION>=0x050000)
#include <QJSValue>
#include <QJSEngine>
#include <QQmlApplicationEngine>
#else
#include <QScriptValue>
#include <QScriptEngine>
#include <QDeclarativeEngine>
#endif

class QTimer;
class UtilityPrivate : public QObject
{
    Q_OBJECT
    Q_ENUMS(ProxyType)
public:
    enum ProxyType {
        DefaultProxy,
        Socks5Proxy,
        NoProxy,
        HttpProxy,
        HttpCachingProxy,
        FtpCachingProxy
    };
};

class MyHttpRequest;
class DownloadImage;
class Utility : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString appVersion READ appVersion CONSTANT)

public:
    static Utility *createUtilityClass();
    
private:
    explicit Utility(QObject *parent = 0);
    ~Utility();

    static Utility *me;
#if(QT_VERSION>=0x050000)
    QPointer<QQmlApplicationEngine> engine;
#else
    QPointer<QDeclarativeEngine> engine;
#endif
    QPoint old_pos;//记录鼠标上次的位置，判断鼠标位置是否改变
    QTimer *mouse_timer;//检测鼠标位置是否变化的定时器
    QPointer<QSettings> mysettings;
    
    MyHttpRequest *http_request;
    DownloadImage *download_image;

    QNetworkConfigurationManager networkConfigurationManager;
    
    char numToStr(int num);//将数字按一定的规律换算成字母
    QByteArray strZoarium(const QByteArray &str);//按一定的规律加密字符串(只包含数字和字母的字符串)
    QByteArray unStrZoarium(const QByteArray &str);//按一定的规律解密字符串(只包含数字和字母的字符串)
    QByteArray fillContent(const QByteArray &str, int length);//将字符串填充到一定的长度

private Q_SLOTS:
    void emitDesktopPosChanged();
public:
    Q_INVOKABLE void consoleLog(QString str);//输出调试信息
    Q_INVOKABLE QString getCookie( QString cookieName );
#if(QT_VERSION>=0x050000)
    QQmlApplicationEngine *qmlEngine();
#else
    QDeclarativeEngine *qmlEngine();
#endif
    MyHttpRequest *getHttpRequest();
    DownloadImage *getDownloadImage();
    bool networkIsOnline() const;
    QString appVersion() const;


#if(QT_VERSION>=0x050000)
    void initUtility(QSettings *settings=0, QQmlApplicationEngine *qmlEngine=0);
    void setQmlEngine( QQmlApplicationEngine *new_engine );
#else
    void initUtility(QSettings *settings=0, QDeclarativeEngine *qmlEngine=0);
    void setQmlEngine( QDeclarativeEngine *new_engine );
#endif
    QPoint mouseDesktopPos();

    void setQSettings(QSettings *settings);
Q_SIGNALS:
    void mouseDesktopPosChanged(QPoint arg);
    void networkOnlineStateChanged(bool isOnline);
public Q_SLOTS:
    void setValue( const QString & key, const QVariant & value);
    QVariant value(const QString & key, const QVariant & defaultValue = QVariant()) const;
    void removeValue( const QString & key );
    
#if(QT_VERSION>=0x050000)
    void downloadImage( QJSValue callbackFun, QUrl url, QString savePath, QString saveName );
    void httpGet(QJSValue callbackFun, QUrl url, bool highRequest=false );
    void httpPost(QJSValue callbackFun, QUrl url, QByteArray data="", bool highRequest=false );
#else
    void downloadImage( QScriptValue callbackFun, QUrl url, QString savePath, QString saveName );
    void httpGet(QScriptValue callbackFun, QUrl url, bool highRequest=false );
    void httpPost(QScriptValue callbackFun, QUrl url, QByteArray data="", bool highRequest=false );
#endif
    void downloadImage( QObject *caller, QByteArray slotName, QUrl url, QString savePath, QString saveName );
    void httpGet(QObject *caller, QByteArray slotName, QUrl url, bool highRequest=false);
    void httpPost(QObject *caller, QByteArray slotName, QUrl url, QByteArray data, bool highRequest=false);
    void socketAbort();
    void setNetworkProxy( int type, QString location, QString port, QString username, QString password );
    
    QString stringEncrypt(const QString &content, QString key);//加密任意字符串，中文请使用utf-8编码
    QString stringUncrypt(const QString &content_hex, QString key);//解密加密后的字符串
    
    void removePath(QString dirPath ,bool deleteHidden = true, bool deleteSelf = true );
};

#endif // UTILITY_H
