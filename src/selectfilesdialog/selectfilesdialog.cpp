#include "selectfilesdialog.h"
#include <QDebug>
#include <QApplication>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include <QDeclarativeView>
#include <QFile>

SelectFilesDialog::SelectFilesDialog() :
    QObject(0)
{
    lastOpenMode = false;
    m_chooseType = AllType;
    m_chooseMode = MultipleChoice;
    m_inverseTheme = false;
    isShow = false;

    connect(this, SIGNAL(close()), SLOT(onClose()));

    QFile file(":/selectfilesdialog/icon/configure.bat");
    if(file.open(QIODevice::ReadOnly)){
        QString temp_str = file.readLine();
        int temp_pos = temp_str.lastIndexOf(QRegExp("\\S"));
        if(temp_pos>=0)
            temp_str = temp_str.left(temp_pos+1);
        while(temp_str!=""){
            IconInfo info;
            QStringList temp_list = temp_str.split(":");
            info.filePath = temp_list[0];
            info.suffixList = temp_list.at(1).split(" ");
            iconInfoList<<info;

            temp_str = file.readLine();
            temp_pos = temp_str.lastIndexOf(QRegExp("\\S"));
            if(temp_pos>=0)
                temp_str = temp_str.left(temp_pos+1);

            //qDebug()<<temp_str;
        }
    }else{
        qDebug()<<QString::fromUtf8("SelectFilesDialog:configure.bat打开失败")<<file.errorString();
    }
}

int SelectFilesDialog::selectionCount() const
{
    return files.length();
}

SelectFilesDialog::ChooseType SelectFilesDialog::chooseType() const
{
    return m_chooseType;
}

SelectFilesDialog::ChooseMode SelectFilesDialog::chooseMode() const
{
    return m_chooseMode;
}

bool SelectFilesDialog::inverseTheme() const
{
    return m_inverseTheme;
}

void SelectFilesDialog::open(const QUrl initPath, const QString &nameFilters,
                             Filters filters, SortFlags sortflags)
{
    if(isShow){
        qmlView->showFullScreen();
        return;
    }
    isShow = true;
    //qDebug()<<((QDir::Filters)((int)filters))<<((QDir::SortFlags)((int)sortflags));
    files.clear();

    if(!lastOpenMode)
        dir = new QDir(initPath.toLocalFile());
    lastOpenMode = true;

    dir->setFilter((QDir::Filters)((int)filters));
    dir->setSorting((QDir::SortFlags)((int)sortflags));
    if(nameFilters!=""){
        QStringList temp_list = nameFilters.split(";");
        dir->setNameFilters(temp_list);
    }

    qmlView = new QDeclarativeView();
    qmlView->engine()->rootContext()->setContextProperty("fileDialog", this);
#ifdef HARMATTAN_BOOSTER
    qmlView->setSource(QUrl("qrc:/selectfilesdialog/meego.qml"));
#else
    qmlView->setSource(QUrl("qrc:/selectfilesdialog/symbian.qml"));
#endif
    qmlView->showFullScreen();
}

int SelectFilesDialog::exec(const QUrl initPath, const QString &nameFilters,
                            Filters filters, SortFlags sortflags)
{
    if(isShow){
        qmlView->showFullScreen();
        return -1;
    }
    isShow = true;
    //qDebug()<<((QDir::Filters)((int)filters))<<((QDir::SortFlags)((int)sortflags));
    files.clear();

    if(lastOpenMode)
       delete this->dir;
    lastOpenMode = false;

    QDir dir(initPath.toLocalFile());
    this->dir = &dir;

    dir.setFilter((QDir::Filters)((int)filters));
    dir.setSorting((QDir::SortFlags)((int)sortflags));
    if(nameFilters!=""){
        QStringList temp_list = nameFilters.split(";");
        dir.setNameFilters(temp_list);
    }

    QDeclarativeView view;
    qmlView = &view;
    view.engine()->rootContext()->setContextProperty("fileDialog", this);
#ifdef HARMATTAN_BOOSTER
    view.setSource(QUrl("qrc:/selectfilesdialog/meego.qml"));
#else
    view.setSource(QUrl("qrc:/selectfilesdialog/symbian.qml"));
#endif

    QEventLoop loop;
    connect(this, SIGNAL(close()), &loop, SLOT(quit()));
    view.showFullScreen();
    int result = loop.exec(QEventLoop::DialogExec);
    view.close();

    return result;
}

QVariantMap SelectFilesDialog::firstSelection() const
{
    Data data = files.first();
    QVariantMap map;
    map["name"] = data.name;
    map["type"] = data.type;

    return map;
}

QVariantMap SelectFilesDialog::lastSelection() const
{
    Data data = files.last();
    QVariantMap map;
    map["name"] = data.name;
    map["type"] = data.type;

    return map;
}

QVariantMap SelectFilesDialog::at(int index) const
{
    Data data = files[index];
    QVariantMap map;
    map["name"] = data.name;
    map["type"] = data.type;

    return map;
}

QVariantList SelectFilesDialog::allSelection() const
{
    QVariantList result_list;

    foreach(Data data, files){
        QVariantMap map;
        map["name"] = data.name;
        map["type"] = data.type;
        result_list<<map;
    }
    return result_list;
}

QVariantList SelectFilesDialog::getCurrentFilesInfo() const
{
    QVariantList result_list;

    foreach(QFileInfo file_info, dir->entryInfoList()){
        if(file_info.fileName()=="."||file_info.fileName()=="..")
            continue;

        QVariantMap temp_map;
        temp_map["name"] = file_info.fileName();
        temp_map["path"] = file_info.absolutePath();
        temp_map["size"] = ((int)((file_info.size()/1024.0)*100))/100.0;
        temp_map["suffix"] = file_info.suffix();
        temp_map["lastModified"] = file_info.lastModified().toString();

        if(file_info.isFile()){
            temp_map["type"] = FileType;
        }else{
            temp_map["type"] = FolderType;
            temp_map["isEmpty"] = dirIsEmpty(file_info);
        }
        result_list<<temp_map;
    }

    return result_list;
}

bool SelectFilesDialog::cdPath(const QString &newPath)
{
    return dir->cd(newPath);
}

void SelectFilesDialog::addSelection(ChooseType type, const QString &absolutePath)
{
    Data data;
    data.name = absolutePath;
    data.type = type;
    files.append(data);
}

void SelectFilesDialog::removeSelection(const QString &absolutePath)
{
    for(int i=0; i<files.count(); ++i){
        Data data = files[i];
        if(data.name == absolutePath){
            files.removeAt(i);
        }
    }
}

void SelectFilesDialog::clearSelection()
{
    files.clear();
}

void SelectFilesDialog::setChooseType(ChooseType arg)
{
    if (m_chooseType != arg) {
        m_chooseType = arg;
        emit chooseTypeChanged(arg);
    }
}

void SelectFilesDialog::setChooseMode(ChooseMode arg)
{
    if (m_chooseMode != arg) {
        m_chooseMode = arg;
        emit chooseModeChanged(arg);
    }
}

QString SelectFilesDialog::getIconNameBySuffix(const QString &suffix) const
{
    foreach(IconInfo info, iconInfoList){
        foreach(QString str, info.suffixList){
            if(str == suffix){
                return "qrc:/selectfilesdialog/icon/"+info.filePath;
            }
        }
    }

    return "qrc:/selectfilesdialog/icon/unknow";
}

void SelectFilesDialog::setInverseTheme(bool arg)
{
    if (m_inverseTheme != arg) {
        m_inverseTheme = arg;
        emit inverseThemeChanged(arg);
    }
}

bool SelectFilesDialog::dirIsEmpty(const QFileInfo &fileInfo) const
{
    QDir dir = fileInfo.dir();
    dir.cd(fileInfo.fileName());
    return dir.entryList().length()<=2;
}

void SelectFilesDialog::onClose()
{
    if(lastOpenMode){//如果是调用open打开的对话框
        qmlView->deleteLater();
    }
    isShow = false;
}
