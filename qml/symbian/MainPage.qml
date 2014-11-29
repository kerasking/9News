// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import com.nokia.symbian 1.1
import com.stars.widgets 1.0

MyPage{
    property bool isQuit: false
    //判断此次点击后退键是否应该退出

    tools: ToolBarLayout{
        ToolButton{
            iconSource: "toolbar-back"
            platformInverted: main.platformInverted
            onClicked: {
                if(isQuit){
                    Qt.quit()
                }else{
                    isQuit = true
                    main.showBanner(qsTr("Press again to exit"))
                }
            }
        }
        ToolButton{
            iconSource: "toolbar-menu"
            platformInverted: main.platformInverted
            onClicked: {
                fileDialog.inverseTheme = true
                fileDialog.exec("./", "", FilesDialog.AllEntries, FilesDialog.Name)
                console.debug("selected files:"+fileDialog.allSelection())
            }
        }
    }

    Timer{
        //当第一次点击后退键时启动定时器，如果在定时器被触发时用户还未按下第二次后退键则将isQuit置为false
        id: timer_quit
        interval: 2000
        onTriggered: {
            isQuit = false
        }
    }
}
