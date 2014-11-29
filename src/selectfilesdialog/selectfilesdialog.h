#ifndef SELECTFILESDIALOG_H
#define SELECTFILESDIALOG_H

#include <QObject>
#include <QVariantMap>
#include <QUrl>
#include <QFileInfo>
#include <QDir>

class QDeclarativeView;
class SelectFilesDialog : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int selectionCount READ selectionCount CONSTANT)
    //记录用户选择了多少了文件/文件夹
    Q_PROPERTY(ChooseType chooseType READ chooseType WRITE setChooseType NOTIFY chooseTypeChanged)
    //chooseType为控制文件夹可选还是文件可选，值为FileType时只能选择文件
    //FolderType为文件夹，FileType|FolderType为都可以
    Q_PROPERTY(ChooseMode chooseMode READ chooseMode WRITE setChooseMode NOTIFY chooseModeChanged)
    //chooseMode为控制可选多项还是单项,为true时只能选择单个文件或文件夹
    Q_PROPERTY(bool inverseTheme READ inverseTheme WRITE setInverseTheme NOTIFY inverseThemeChanged)
    //inversej记录界面是否反色显示（正常为亮色，反色为暗色）

    Q_ENUMS(ChooseType)
    Q_ENUMS(ChooseMode)
    Q_FLAGS(Filters)
    Q_FLAGS(SortFlags)

public:
    SelectFilesDialog();

    enum ChooseType{
        FolderType,//选中条目是文件夹
        FileType,//选中条目是文件
        AllType//所有类型
    };

    enum ChooseMode{
        IndividualChoice,//单项选择
        MultipleChoice//多项选择
    };

    enum Filter{
        Dirs        = 0x001,
        Files       = 0x002,
        Drives      = 0x004,
        NoSymLinks  = 0x008,
        AllEntries  = Dirs | Files | Drives,
        TypeMask    = 0x00f,
#ifdef QT3_SUPPORT
        All         = AllEntries,
#endif

        Readable    = 0x010,
        Writable    = 0x020,
        Executable  = 0x040,
        PermissionMask    = 0x070,
#ifdef QT3_SUPPORT
        RWEMask     = 0x070,
#endif

        Modified    = 0x080,
        Hidden      = 0x100,
        System      = 0x200,

        AccessMask  = 0x3F0,

        AllDirs       = 0x400,
        CaseSensitive = 0x800,
        NoDotAndDotDot = 0x1000, // ### Qt5 NoDotAndDotDot = NoDot|NoDotDot
        NoDot         = 0x2000,
        NoDotDot      = 0x4000,

        NoFilter = -1
#ifdef QT3_SUPPORT
        ,DefaultFilter = NoFilter
#endif
    };
    Q_DECLARE_FLAGS(Filters, Filter)

    enum SortFlag {
        Name        = 0x00,
        Time        = 0x01,
        Size        = 0x02,
        Unsorted    = 0x03,
        SortByMask  = 0x03,

        DirsFirst   = 0x04,
        Reversed    = 0x08,
        IgnoreCase  = 0x10,
        DirsLast    = 0x20,
        LocaleAware = 0x40,
        Type        = 0x80,
        NoSort = -1
#ifdef QT3_SUPPORT
        ,DefaultSort = NoSort
#endif
    };
    Q_DECLARE_FLAGS(SortFlags, SortFlag)

    int selectionCount() const;
    ChooseType chooseType() const;
    ChooseMode chooseMode() const;
    bool inverseTheme() const;

public Q_SLOTS:
    void open(const QUrl initPath=QUrl(), const QString& nameFilters="",
              Filters filters=NoFilter, SortFlags sortflags=NoSort);
    int exec(const QUrl initPath=QUrl(), const QString& nameFilters="",
             Filters filters=NoFilter, SortFlags sortflags=NoSort);

    QVariantMap firstSelection() const;
    QVariantMap lastSelection() const;
    QVariantMap at(int index) const;
    QVariantList allSelection() const;

    QVariantList getCurrentFilesInfo() const;
    bool cdPath(const QString& newPath);

    void addSelection(ChooseType type, const QString& absolutePath);
    void removeSelection(const QString& absolutePath);
    void clearSelection();
    void setChooseType(ChooseType arg);
    void setChooseMode(ChooseMode arg);

    QString getIconNameBySuffix(const QString& suffix) const;
    void setInverseTheme(bool arg);

Q_SIGNALS:
    void close();
    void chooseTypeChanged(ChooseType arg);
    void chooseModeChanged(ChooseMode arg);
    void inverseThemeChanged(bool arg);

private Q_SLOTS:
    void onClose();
private:
    struct Data{
        QString name;
        ChooseType type;
    };
    QList<Data> files;
    QDir *dir;
    QDeclarativeView* qmlView;
    bool lastOpenMode;
    //记录上一次打开文件选择对话框使用的是open方法则为true，使用exec方法则为false
    ChooseType m_chooseType;
    ChooseMode m_chooseMode;

    struct IconInfo{
        QString filePath;
        QStringList suffixList;
    };
    QList<IconInfo> iconInfoList;
    bool m_inverseTheme;//记录主题色的模式（亮色或者暗色）
    bool isShow;//记录文件对话框是否正在显示中

    bool dirIsEmpty(const QFileInfo& fileInfo) const;
    //判断一个文件夹是否为空的
};

#endif // SELECTFILESDIALOG_H
