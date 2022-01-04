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
    maximumWidth: 1280
    maximumHeight: 1024
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
                implicitWidth: 20
                implicitHeight: 20
                color: model.value ? "#098229" : "#e6f0e9"

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
                anchors.left: parent.left
                anchors.right: parent.right
                value: _qmlAdapter.gameOfLifeModel.livingCellsAtBeginningAsPercentage
                stepSize: 1
                from: 1
                to: 100

                onValueChanged: _qmlAdapter.gameOfLifeModel.livingCellsAtBeginningAsPercentage = value
            }

            SettingsButton
            {
                id: generatePatternButton
                text: qsTr("Zufällige Anordnung erstellen")           

                onClicked: _qmlAdapter.gameOfLifeModel.generatePattern()
            }

            SettingsButton
            {
                id: clearPatternButton
                text: qsTr("Anordnung löschen")

                onClicked: _qmlAdapter.gameOfLifeModel.clearPattern()
            }

            SettingsButton
            {
                id: loadFileButton
                text: qsTr("Anordnung laden") 
            }

            SettingsButton
            {
                id: saveFileButton
                text: qsTr("Anordnung speichern")
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
                    width: parent.width / 5
                    height: defaultHeight
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: defaultFontSize
                }

                Rectangle
                {
                    id: loopCountRect
                    width: parent.width / 5
                    height: loopTextEdit.height + 5
                    anchors.verticalCenter: loopCountText.verticalCenter
                    border.color: "black"

                    TextEdit
                    {
                        id: loopTextEdit
                        text: qsTr(_qmlAdapter.gameOfLifeModel.loopCount.toString())
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: defaultFontSize

                        onTextChanged: _qmlAdapter.gameOfLifeModel.loopCount = parseInt(loopTextEdit.text)
                    }
                }
            }

            Row
            {
                id: delayRow
                anchors.left: parent.left
                anchors.right: parent.right
                spacing: 10

                Text
                {
                    id: delayText
                    text: qsTr("Delay:")
                    width: parent.width / 5
                    height: defaultHeight
                    font.pixelSize: defaultFontSize
                }

                Text
                {
                    id: delayTextBesideSlider
                    text: qsTr(_qmlAdapter.gameOfLifeModel.delay.toString() + "ms")
                    width: parent.width / 5
                    height: defaultHeight
                    font.pixelSize: defaultFontSize
                }

                Slider
                {
                    id: delaySlider
                    width: parent.width / 2
                    value: _qmlAdapter.gameOfLifeModel.delay
                    stepSize: 1
                    from: 0
                    to: 1000

                    onValueChanged: _qmlAdapter.gameOfLifeModel.delay = value
                }
            }

            Text
            {
                id: stepCountText
                text: qsTr("Schrittanzahl: " + _qmlAdapter.gameOfLifeModel.stepCount.toString())
                height: defaultHeight
                anchors.left: parent.left
                anchors.right: parent.right
                font.pixelSize: defaultFontSize
            }

            SettingsButton
            {
                id: infiniteLoopButton
                text: qsTr("Starte unendliche Wiederholungen")

                onClicked: _qmlAdapter.gameOfLifeModel.startInfiniteLoop()
            }

            SettingsButton
            {
                id: startLoopButton
                text: qsTr("Starte die Wiederholungen")

                onClicked: _qmlAdapter.gameOfLifeModel.startLoop()
            }

            SettingsButton
            {
                id: nextStepButton
                text: qsTr("Nächster Schritt")

                onClicked: _qmlAdapter.gameOfLifeModel.nextStep()
            }

            SettingsButton
            {
                id: stopLoopButton
                text: qsTr("Loop stoppen")

                onClicked: _qmlAdapter.gameOfLifeModel.stopLoop()
            }
        }
    }
}
