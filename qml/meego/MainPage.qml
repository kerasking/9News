// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import com.nokia.meego 1.1
import com.stars.widgets 1.0

MyPage{
    tools: ToolBarLayout{
        ToolIcon{
            iconId: "toolbar-volume"

            onClicked: {
                fileDialog.inverseTheme = true
                fileDialog.exec("./", "", FilesDialog.Dirs, FilesDialog.Name)
                console.debug("selected files:"+fileDialog.allSelection())
            }
        }
    }
}
