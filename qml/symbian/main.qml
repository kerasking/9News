// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1
import com.nokia.symbian 1.1
import com.nokia.extras 1.1

PageStackWindow{
    id:main
    showStatusBar:true
    platformSoftwareInputPanelEnabled :true
    platformInverted: true

    initialPage: MainPage{}

    function showBanner(string){
        banner.text = string
        banner.open()
    }

    InfoBanner {
        id: banner
        timeout: 2000
        platformInverted: main.platformInverted
    }
}
