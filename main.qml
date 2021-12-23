import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

Window {
    id: mainWindow
    objectName: mainWindow
    visible: true
    width: 800
    height: 600
    minimumWidth: 800
    minimumHeight: 600
    title: qsTr("Game Of Life")

    property int defaultMargin: 10

    GroupBox
    {
        id: gameGroupBox
        objectName: gameGroupBox
        title: qsTr("The Game")
        width: parent.width / 1.5
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: defaultMargin
        padding: 0

        TableView
        {
            id: gameTableView
            objectName: gameTableView
            anchors.fill: parent
            contentX: (contentWidth - width) / 2
            contentY: (contentHeight - height) / 2
            rowSpacing: 1
            columnSpacing: 1
            ScrollBar.horizontal: ScrollBar {}
            ScrollBar.vertical: ScrollBar {}

            delegate: Rectangle
            {
                id: cell
                objectName: cell
                implicitWidth: 15
                implicitHeight: 15
                color: model.value ? "#f3f3f4" : "#b5b7bf"

                MouseArea
                {
                    anchors.fill: parent
                    onClicked: model.value = !model.value
                }
            }

            model: GameOfLifeModel
            {
                id: gameOfLifeModel
                objectName: gameOfLifeModel
            }
        }
    }

    GroupBox
    {
        id: settingsGroupBox
        objectName: settingsGroupBox
        title: qsTr("Settings")
        anchors.left: gameGroupBox.right
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: defaultMargin
    }
}
