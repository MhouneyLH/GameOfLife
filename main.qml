import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

Window {
    id: mainWindow
    visible: true
    width: 800
    height: 600
    minimumWidth: 800
    minimumHeight: 600
    title: qsTr("Conway’s Game Of Life")

    property int defaultMargin: 10
    property int defaultHeaderSize: 14
    property int defaultFontSize: 11
    property int defaultHeight: 30

    GroupBox
    {
        id: gameGroupBox
        title: qsTr("Simulation")
        width: parent.width / 1.5
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.margins: defaultMargin
        font.pixelSize: defaultHeaderSize

        TableView
        {
            id: gameTableView          
            anchors.fill: parent
            contentX: (contentWidth - width) / 2
            contentY: (contentHeight - height) / 2
            rowSpacing: 1
            columnSpacing: 1
            ScrollBar.horizontal: ScrollBar {}
            ScrollBar.vertical: ScrollBar {}
            clip: true
            model: _qmlAdapter.gameOfLifeModel

            delegate: Rectangle
            {
                id: cell                
                implicitWidth: 7
                implicitHeight: 7
                color: model.value ? "#ff0026" : "#e1e1e6"

                MouseArea
                {
                    anchors.fill: parent
                    onClicked: model.value = !model.value
                }
            }
        }
    }

    GroupBox
    {
        id: settingsGroupBox     
        title: qsTr("Einstellungen")
        anchors.left: gameGroupBox.right
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.margins: defaultMargin
        font.pixelSize: defaultHeaderSize

        Column
        {
            id: patternColumn
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.right: parent.right
            spacing: 2

            Button
            {
                id: generatePatternButton
                text: qsTr("Zufällige Anordnung erstellen")
                height: defaultHeight
                anchors.left: parent.left
                anchors.right: parent.right
                font.pixelSize: defaultFontSize;

                onClicked: _qmlAdapter.gameOfLifeModel.generatePattern()
            }

            Text
            {
                id: livingCellsText
                text: qsTr(_qmlAdapter.gameOfLifeModel.livingCellsAtBeginningAsPercentage.toString() + "%")
                anchors.horizontalCenter: parent.horizontalCenter
                font.pixelSize: defaultFontSize
            }

            Slider
            {
                id: livingCellsSlider
                anchors.horizontalCenter: parent.horizontalCenter
                value: 50
                stepSize: 1
                from: 1
                to: 100

                onValueChanged: _qmlAdapter.gameOfLifeModel.livingCellsAtBeginningAsPercentage = value
            }

            Button
            {
                id: loadFileButton
                text: qsTr("Anordnung laden")
                height: defaultHeight
                anchors.left: parent.left
                anchors.right: parent.right
                font.pixelSize: defaultFontSize
            }

            Button
            {
                id: saveFileButton
                text: qsTr("Anordnung speichern")
                height: defaultHeight
                anchors.left: parent.left
                anchors.right: parent.right
                font.pixelSize: defaultFontSize
            }            

            Button
            {
                id: startLoopButton
                text: qsTr("Starte die Wiederholungen")
                height: defaultHeight
                anchors.left: parent.left
                anchors.right: parent.right
                font.pixelSize: defaultFontSize

                onClicked: _qmlAdapter.gameOfLifeModel.startLoop()
            }

            Button
            {
                id: nextStepButton
                text: qsTr("Nächster Schritt")
                height: defaultHeight
                anchors.left: parent.left
                anchors.right: parent.right
                font.pixelSize: defaultFontSize

                onClicked: _qmlAdapter.gameOfLifeModel.nextStep()
            }

            Button
            {
                id: clearPatternButton
                text: qsTr("Anordnung löschen")
                height: defaultHeight
                anchors.left: parent.left
                anchors.right: parent.right
                font.pixelSize: defaultFontSize

                onClicked: _qmlAdapter.gameOfLifeModel.clearPattern()
            }
        }

        Column
        {
            id: loopColumn
            anchors.left: parent.left
            anchors.top: patternColumn.bottom
            anchors.topMargin: defaultMargin
            anchors.right: parent.right
            anchors.bottom: parent.bottom

            Text
            {
                id: loopHeaderText
                text: qsTr("Wiederholungen")
                height: defaultHeight
                font.pixelSize: defaultHeaderSize
            }

            Row
            {
                id: loopDetailsRow
                anchors.left: parent.left
                anchors.right: parent.right
                spacing: 10

                Text
                {
                    id: loopCountText
                    text: qsTr("Anzahl:")
                    height: defaultHeight
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: defaultFontSize
                }

                Rectangle
                {
                    id: loopCountRect
                    width: loopTextEdit.width + 8
                    height: loopTextEdit.height + 5
                    anchors.verticalCenter: loopCountText.verticalCenter
                    border.color: "black"

                    TextEdit
                    {
                        id: loopTextEdit
                        text: qsTr("100")
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: defaultFontSize

                        onTextChanged: _qmlAdapter.gameOfLifeModel.loopCount = parseInt(loopTextEdit.text)
                    }
                }
            }                       
        }
    }
}
