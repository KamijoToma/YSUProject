// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0

import QtQuick 6.2
import QtQuick.Controls 6.2
import QtQuick.Layouts 6.2
import Qt5Compat.GraphicalEffects
import YunaPlayer

ApplicationWindow {
    id: root

    height: Constants.height
    title: "YunaPlayer"
    visible: true
    width: Constants.width

    header: ToolBar {
        id: header

        RowLayout {
            anchors.fill: parent

            ToolButton {
                text: stack.depth > 1 ? qsTr("<") : qsTr("X")

                onClicked: function () {
                    if (stack.depth > 1) {
                        frameTitle.text = "YunaPlayer"
                        stack.pop();
                    }
                }
            }
            Label {
                id: frameTitle

                Layout.fillWidth: true
                elide: Label.ElideRight
                font.pixelSize: Theme.labelTextSize
                horizontalAlignment: Qt.AlignHCenter
                text: "YunaPlayer"
                verticalAlignment: Qt.AlignVCenter
            }
            ToolButton {
                enabled: stack.depth < 2
                text: qsTr(":")

                onClicked: function () {
                    frameTitle.text = "Playlist";
                    stack.push(songList);
                }
            }
        }
    }

    StackView {
        id: stack

        property alias mainScreen: mainScreen

        anchors.fill: parent
        initialItem: mainScreen
        opacity: 0.7

        Screen01 {
            id: mainScreen

            height: root.height - header.height
            objectName: "mainScreen"
            width: root.width
        }
    }
    Component {
        id: songList

        Screen02 {
            height: root.height - root.header.height
            objectName: "songList"
            width: root.width
        }
    }
    Item {
        height: parent.height
        layer.enabled: true
        width: parent.width
        z: -1

        layer.effect: GaussianBlur {
            radius: 30
            source: stack.mainScreen.songImage
        }
    }
}
