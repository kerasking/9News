// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import com.nokia.meego 1.1
import com.stars.widgets 1.0

PageStackWindow{
    id:main
    showStatusBar:true

    function updateFileList(){
        mymodel.clear()
        var fileList = fileDialog.getCurrentFilesInfo();
        for(var i in fileList){
            mymodel.append({"fileInfo": fileList[i]})
        }
    }

    initialPage: Page{
        tools: ToolBarLayout{
            ToolIcon{
                iconId: "toolbar-up"

                onClicked: {
                    fileDialog.cdPath("..")
                    updateFileList()
                }
            }
            ToolIcon{
                iconId: "toolbar-done"

                enabled: list_file.selectionCount>0
                onClicked: {
                    fileDialog.close()
                }
            }
            ToolIcon{
                iconId: "toolbar-cancle"

                onClicked: {
                    fileDialog.clearSelection()
                    fileDialog.close()
                }
            }
            ToolIcon{
                iconId: "toolbar-refresh"
                onClicked: {
                    updateFileList()
                }
            }
        }

        ListView{
            id: list_file
            property int currentSelectionIndex: -1
            //记录当前被选中的条目的索引，当为单选模式时方便其他条目判断自己选择状态是否已经失去
            property int selectionCount: 0
            anchors.fill: parent
            model: ListModel{id:mymodel}
            delegate: componentListDelegate

            Component.onCompleted: {
                main.updateFileList()
            }

            function addSelection(fileInfo, index){
                if(fileDialog.chooseMode == fileDialog.IndividualChoice){
                    fileDialog.clearSelection()
                    currentSelectionIndex = index
                    selectionCount=0
                }
                var absolutePath = fileInfo.path+"/"+fileInfo.name
                fileDialog.addSelection(fileInfo.type, absolutePath)
                ++selectionCount
            }

            function removeSelection(fileInfo, index){
                if(fileDialog.chooseMode == fileDialog.IndividualChoice){
                    fileDialog.clearSelection()
                    currentSelectionIndex = -1
                    selectionCount = 0
                }else{
                    var absolutePath = fileInfo.path+"/"+fileInfo.name
                    fileDialog.removeSelection(absolutePath)
                    --selectionCount
                }
            }
        }
    }

    Binding {//设置meego系统控件的主题
        target: theme
        property: "inverted"
        value: !fileDialog.inverseTheme
    }

    Component{
        id: componentListDelegate

        Item{
            property bool isSelected: false

            width: parent.width
            height: 80

            function editStatus(){//改变自己的状态，从被选中变为选中，或从选中变为取消选中
                if(rect_background.visible){
                    isSelected = false
                    list_file.removeSelection(fileInfo, index)
                }else{
                    isSelected = true
                    list_file.addSelection(fileInfo, index)
                }
            }

            Rectangle{
                id: rect_background
                anchors.fill: parent
                visible: isSelected&&(fileDialog.chooseMode == FilesDialog.IndividualChoice?
                                          index == list_file.currentSelectionIndex:true)
                gradient: Gradient {
                    GradientStop { position: 0.0; color: "#1080DD" }
                    GradientStop { position: 1.0; color: "#6BB2E7" }
                }
            }

            Image{
                id: fileIcon
                x: 10
                anchors.verticalCenter: parent.verticalCenter
                sourceSize.width: 64
                sourceSize.height: 64

                function getSuffix(){
                    if(fileDialog.inverseTheme){
                        return "-inverse.png"
                    }
                    return ".png"
                }
                function imageSource(){
                    if(fileInfo.type == FilesDialog.FolderType){
                        if(fileInfo.isEmpty){
                            return "qrc:/selectfilesdialog/icon/folder_null"+getSuffix()
                        }else{
                            return "qrc:/selectfilesdialog/icon/folder_files"+getSuffix()
                        }
                    }else if(fileInfo.type == FilesDialog.FileType){
                        return fileDialog.getIconNameBySuffix(fileInfo.suffix)+getSuffix()
                    }else{
                        return "qrc:/selectfilesdialog/icon/unknow"+getSuffix()
                    }
                }

                source: imageSource()

                Connections{
                    target: fileDialog
                    onInverseThemeChanged:{
                        fileIcon.source = imageSource()
                    }
                }
            }

            Text {
                id: fileName
                text: fileInfo.name
                font.bold: true
                anchors.left: fileIcon.right
                font.pixelSize: 24
                anchors.leftMargin: 10
                anchors.top: fileIcon.top
                width: parent.width-fileIcon.width-30
                elide: Text.ElideRight
            }

            Text {
                id: fileOtherInfo
                text: fileInfo.lastModified+" - "+fileInfo.size+" kb"
                anchors.bottom: fileIcon.bottom
                anchors.left: fileName.left
                font.pixelSize: 14
                width: parent.width-fileIcon.width-30
                elide: Text.ElideRight
            }

            Rectangle{
                width: parent.width
                height: 1
                color: "#aaa"
                anchors.bottom: parent.bottom
            }

            MouseArea{
                anchors.fill: parent
                onClicked: {
                    switch(fileDialog.chooseType){
                    case FilesDialog.FolderType:{
                        if(fileInfo.type == FilesDialog.FolderType){
                            editStatus()
                        }else{
                            fileDialog.cdPath(fileInfo.name)
                            updateFileList()
                        }
                        break;
                    }

                    case FilesDialog.FileType:{
                        if(fileInfo.type == FilesDialog.FileType){
                            editStatus()
                        }else{
                            Qt.openUrlExternally(fileInfo.name)
                        }
                        break;
                    }

                    case FilesDialog.AllType:{
                        editStatus()
                        break;
                    }
                    default:break
                    }
                }
                onDoubleClicked: {
                    if(fileInfo.type == FilesDialog.FolderType){
                        fileDialog.cdPath(fileInfo.name)
                        updateFileList()
                    }else{
                        Qt.openUrlExternally(fileInfo.name)
                    }
                }
            }
        }
    }
}
