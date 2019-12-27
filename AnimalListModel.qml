import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

ColumnLayout {

    Frame {
        Layout.fillWidth: true

        ListView {
            id: myview
            implicitHeight: 400
            implicitWidth: 300
            clip: true
            anchors.fill: parent
            highlight: Rectangle {
                color: "lightsteelblue"
                radius: 5
            }
            focus: true

            //highlightRangeMode: ListView.StrictlyEnforceRange
            model: myModel
            delegate: RowLayout {
                id: displayRow
                width: parent.width

                Text {
                    id: idname
                    text: model.animalid
                }
                CheckBox {
                    id: checkdone
                    checked: model.done
                    onClicked: myview.currentIndex = index
                    onCheckedChanged: model.done = checked
                }
                TextField {
                    Layout.fillWidth: true
                    text: model.type
                    font.strikeout: checkdone.checked
                    onPressed: myview.currentIndex = index
                    onEditingFinished: model.type = text
                }
                TextField {
                    Layout.fillWidth: true
                    text: model.size
                    font.strikeout: checkdone.checked
                    onPressed: myview.currentIndex = index
                    onEditingFinished: model.size = text
                }
                Button {
                    text: qsTr("删除")
                    Layout.fillWidth: true
                    onClicked: myModel.m_removeOne(index)
                }
            }
        }
    }
    RowLayout {
        Button {
            text: qsTr("新增")
            Layout.fillWidth: true
            onClicked: myModel.m_addAnimal()
        }
        Button {
            text: qsTr("删除已完成")
            Layout.fillWidth: true
            onClicked: myModel.m_removeCompleted()
        }
    }
}
