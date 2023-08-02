// This file include the logical part of Screen01.ui.qml

import QtQuick 6.2
import QtQuick.Controls 6.2
import YunaPlayer

Screen01 {
    currentPlayingTime: 0
    currentSongIndex: 1
    currentVolume: 50
    playing: false
    songType: "MP3"
    songChannel: "Dual Channel"
    songBitrate: "192kbps"

    onPlayingChanged: {
        songImageAnim.running = true;
        !playing ? songImageAnim.pause() : songImageAnim.resume();
        // if(playing){
        //     if(!songImageAnim.running){
        //         songImageAnim.running = true;
        //         songImageAnim.pause();
        //     }
        // }else songImageAnim.resume
    }

    playControlStop {
        onClicked: {
            playing = !playing;
            player.onPlayButtonClick(playing);
        }
    }
    playingSlider {
        onMoved: {
            currentPlayingTime = playingSlider.value;
            player.onPlaySlideChanged(currentPlayingTime);
        }
    }
    volumeSlider {
        onMoved: {
            currentVolume = volumeSlider.value;
            player.onVolumeChanged(currentVolume);
        }
    }
    SequentialAnimation {
        id: sequentialAnimation

        running: false

        NumberAnimation {
            id: numberAnimationStart

            duration: 5000
            easing.type: Easing.Linear
            target: playingSongName
        }
        NumberAnimation {
            id: numberAnimationEnd

            duration: 0
            easing.type: Easing.Linear
            target: playingSongName
            to: 0
        }
    }
    Timer {
        id: numberAnimationTimer

        interval: 15000
        repeat: true
        running: true
        triggeredOnStart: true

        onTriggered: {
            if (playingSongName.x + playingSongName.width > playingSongName.parent.width) {
                numberAnimationStart.to = 0 - playingSongName.width + playingSongName.parent.width * 3 / 4;
                numberAnimationEnd.to = 0;
                sequentialAnimation.running = true;
            } else {
                sequentialAnimation.running = false;
            }
        }
    }
}
