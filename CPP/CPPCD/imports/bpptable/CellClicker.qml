import QtQuick 2.0
//import BppTableModel 0.1
import BppTable

MouseArea {
    property CompGrid grid: null
    property bool linkEnabled: false

    signal doCommand();

    cursorShape: linkEnabled ? Qt.PointingHandCursor : Qt.ArrowCursor

    anchors.fill: parent
    onClicked: function(mouse){
        if(grid){
                grid.setSelectedRow(row, mouse.modifiers);
        }
    }

    onDoubleClicked: function(mouse){
        if(linkEnabled) {
                if(grid.selectedRow !== row) grid.setSelectedRow(row, mouse.modifiers);
                doCommand()
            }
    }
}
