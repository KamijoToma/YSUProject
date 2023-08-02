import QtQuick 6.2
import QtQuick.Controls 6.2
import Qt.labs.qmlmodels 1.0
import YunaPlayer
import BppTable 0.1
import QtQuick.Dialogs

Rectangle {
    property var fruitList: []

    function fillData() {
        var j = [];
        for (var i = 0; i < player.getDataSize(); i++) {
            j.push(player.fillData(i));
            console.log(j);
        }
        fruitList = j;
        console.log(fruitList);
        bGrid.fillFromJson(fruitList);
    }

    //property alias songList: songList

    color: Theme.backgroundColor

    Component.onCompleted: {
        fillData();
    }

    Button {
        id: buttonAdd

        text: "Add"

        anchors {
            left: parent.left
            leftMargin: 10
            top: parent.top
        }

        onClicked: {
            addDialog.open();
        }
    }
    Button {
        id: buttonDelete

        text: "Delete"

        anchors {
            left: buttonAdd.right
            leftMargin: 10
            top: buttonAdd.top
        }

        onClicked: {
            player.deleteFile(bGrid.cellValue(bGrid.selectedRow, 0) - 1)
            player.refreshList();
            fillData();
        }
    }
    Rectangle {
        border.color: "gray"
        border.width: 1
        color: "lightgray"
        height: buttonDelete.height * 0.8
        radius: 5
        width: parent.width / 3

        anchors {
            left: buttonDelete.right
            leftMargin: 20
            right: buttonSearch.left
            rightMargin: 20
            verticalCenter: buttonDelete.verticalCenter
        }
        TextInput {
            id: searchInput

            font.pixelSize: parent.height * 0.8

            anchors {
                fill: parent
                verticalCenter: parent.verticalCenter
            }
        }
    }
    Button {
        id: buttonSearch

        text: "Search"

        onClicked: {
            player.searchFor(searchInput.text);
            console.log("Fill data");
            fillData();
        }

        anchors {
            leftMargin: 10
            right: parent.right
            top: buttonAdd.top
        }
    }
    CompGrid {
        id: bGrid

        cellDelegate: cellItem
        dataHeight: 30
        fromArray: [// {sid: 1, name: "A", artist: "Abc", ci: "Mr.a", qu: "Mr.b", time: "2022-03-01"}
            {
                "role": "sid",
                "title": "ID",
                "dataType": BTColumn.Int
            }, {
                "role": "name",
                "title": "Name",
                "minWidth": 70
            }, {
                "role": "artist",
                "title": "Artist",
                "dataType": BTColumn.Str
            }, {
                "role": "ci",
                "title": "Lyricist",
                "dataType": BTColumn.Str
            }, {
                "role": "qu",
                "title": "Composer",
                "dataType": BTColumn.Str
            }, {
                "role": "time",
                "title": "Release Date",
                "dataType": BTColumn.Date
            }, {
                "role": "duration",
                "title": "Duration",
                "dataType": BTColumn.Int
            }]
        showOptionsButton: true

        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right
            top: buttonAdd.bottom
        }
        Component {
            id: cellItem

            Rectangle {
                color: bGrid.getCellBk(row, highlight)
                implicitHeight: bGrid.dataHeight

                CellText {
                    horizontalAlignment: bGrid.getAlign(dataType)
                    text: bGrid.formatDisplay(display, dataType, 2)
                }
                CellClicker {
                    grid: bGrid
                    linkEnabled: true

                    onDoCommand: {
                        player.playMusic(bGrid.cellValue(bGrid.selectedRow, 0) - 1);
                    }
                }
            }
        }
    }
    FileDialog {
        id: addDialog

        nameFilters: ["Music Files (*.mp3 *.flac *.wav *.aac)"]
        title: qsTr("Please choose music file")

        onAccepted: {
            console.log("Add", selectedFile);
            player.addFile(selectedFile);
            player.refreshList();
            fillData();
        }
    }
}
