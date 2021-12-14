import QtQuick 2.6
import QtQuick.Window 2.3
import QtQuick.Controls 2.2

Window {
    id: "mainWindow"
    objectName: "mainWindow"
    visible: true
    width: 1600
    height: 900
    minimumWidth: 800
    minimumHeight: 600
    title: qsTr("Game Of Life")

    property int defaultMargin: 10

    GroupBox
    {
        id: "gameGroupBox"
        objectName: "gameGroupBox"
        title: qsTr("The Game")
        width: parent.width / 1.5
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: defaultMargin
        padding: 0
    }

    GroupBox
    {
        id: "settingsGroupBox"
        objectName: "settingsGroupBox"
        title: qsTr("Settings")
        anchors.left: gameGroupBox.right
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: defaultMargin
    }
}
