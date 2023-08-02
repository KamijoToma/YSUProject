/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick 6.2
import QtQuick.Controls 6.2
import Qt5Compat.GraphicalEffects
import YunaPlayer

Rectangle {
    id: screen01Rect

    property int currentPlayingTime
    property int currentVolume
    property int currentSongIndex
    property int playPolicy // 0: Stop 1: Next 2: Single
    property string songType
    property string songName
    property string songChannel
    property string songBitrate
    property alias playControlStop: playControlStop
    property bool playing
    property alias playingSlider: playingSlider
    property alias playingSongName: playingSongName
    property alias songImage: songImage
    property alias songImageAnim: songImageAnim
    property int totalPlayingTime
    property alias volumeSlider: volumeSlider

    antialiasing: true
    color: Theme.backgroundColor
    height: Constants.height
    width: Constants.width

    Text {
        id: songNumberLabel

        color: Theme.outlineColor
        font.pixelSize: 20
        text: (currentSongIndex+1+'').padStart(3, '0')

        anchors {
            left: parent.left
            leftMargin: 10
            top: parent.top
            topMargin: 10
        }
    }
    Text {
        id: playingSongName

        font.pixelSize: Theme.labelTextSize
        text: songName

        anchors {
            left: songNumberLabel.right
            leftMargin: 10
            top: songNumberLabel.top
        }
        // TODO Make it marquee
    }
    Text {
        id: playingSongInfoFormat

        font.pixelSize: Theme.labelTextSize
        text: songType

        anchors {
            left: songNumberLabel.left
            top: songNumberLabel.bottom
            topMargin: 5
        }
    }
    Text {
        id: playingSongInfoBitrate

        font.pixelSize: Theme.labelTextSize
        text: songBitrate

        anchors {
            left: playingSongInfoFormat.right
            leftMargin: 5
            top: playingSongInfoFormat.top
        }
    }
    Text {
        id: playingSongInfoChannelType

        font.pixelSize: Theme.labelTextSize
        text: songChannel

        anchors {
            left: playingSongInfoBitrate.right
            leftMargin: 5
            top: playingSongInfoFormat.top
        }
    }

    // Bottom
    Button {
        id: playControlStop

        background: null
        flat: true
        text: playing ? "⏸" : "▶"
        width: playControlStop.height

        anchors {
            bottom: parent.bottom
            bottomMargin: 5
            horizontalCenter: parent.horizontalCenter
        }
    }

    Button {
        id: playPolicyControl

        background: null
        flat: true
        text: function () {
            switch (playPolicy) {
                case 0:
                    return 'PAUSE'
                case 1:
                    return 'CONTINUE'
                case 2:
                    return 'LOOP'
            }
            return 'ERR'
        }()

        anchors {
            left: parent.left
            leftMargin: 10
            top: playControlStop.top
        }

        onClicked: {
            playPolicy += 1;
            playPolicy %= 3;
        }
    }
    Button {
        id: playControlPrev

        background: null
        flat: true
        text: "<"
        width: playControlStop.height

        anchors {
            bottom: playControlStop.bottom
            right: playControlStop.left
            rightMargin: 10
        }

        onClicked: {
            player.playMusic(currentSongIndex-1);
        }
    }
    Button {
        id: playControlNext

        background: null
        flat: true
        text: ">"
        width: playControlStop.height

        anchors {
            bottom: playControlStop.bottom
            left: playControlStop.right
            leftMargin: 10
        }

        onClicked: {
            player.playMusic(currentSongIndex+1);
        }
    }
    Text {
        id: playTimeInfo

        font.pixelSize: Theme.labelTextSize
        text: ((Math.floor(currentPlayingTime / 60) + '') + ':' + ('' + currentPlayingTime % 60).padStart(2, '0'))

        anchors {
            bottom: playControlStop.top
            bottomMargin: 5
            left: parent.left
            leftMargin: 5
        }
    }
    Text {
        id: playTotalTimeInfo

        font.pixelSize: Theme.labelTextSize
        text: ((Math.floor(totalPlayingTime / 60) + '') + ':' + ('' + totalPlayingTime % 60).padStart(2, '0'))

        anchors {
            bottom: playControlStop.top
            bottomMargin: 5
            right: parent.right
            rightMargin: 5
        }
    }
    Slider {
        id: playingSlider

        height: playTimeInfo.height
        orientation: Qt.Horizontal
        stepSize: 1
        to: totalPlayingTime
        value: currentPlayingTime

        anchors {
            left: playTimeInfo.right
            leftMargin: 5
            right: playTotalTimeInfo.left
            rightMargin: 5
            top: playTimeInfo.top
        }
    }
    Rectangle {
        id: songClip

        anchors.centerIn: parent
        clip: true
        color: Qt.rgba(255, 255, 255, 0)
        //visible: false
        height: (root.width + root.height) / 6
        width: (root.width + root.height) / 6

        Image {
            id: songImage

            antialiasing: true
            fillMode: Image.PreserveAspectFit
            source: "images/school_logo.jpg"
            transformOrigin: Item.Center
            visible: false

            anchors {
                fill: parent
            }
            RotationAnimation {
                id: songImageAnim

                duration: 8000
                loops: Animation.Infinite
                running: false // See Screen01.qml. There is a listener to pause/resume this animation
                target: songClip
                to: 360
            }
        }
        OpacityMask {
            anchors.fill: songImage
            invert: false
            source: songImage
            opacity: 1

            maskSource: Rectangle {
                color: Qt.rgba(255, 255, 255, 1)
                height: songImage.height
                radius: songImage.width / 2
                width: songImage.width
                opacity: 1
            }
        }
    }
    Slider {
        id: volumeSlider

        height: playTimeInfo.height
        orientation: Qt.Horizontal
        stepSize: 1
        to: 100
        value: currentVolume
        width: root.width / 4

        anchors {
            right: parent.right
            rightMargin: 5
            top: playControlStop.top
        }
    }
    Text {
        id: volumeLabel

        text: currentVolume + " %"

        anchors {
            right: volumeSlider.left
            rightMargin: 10
            top: volumeSlider.top
        }
    }
}
