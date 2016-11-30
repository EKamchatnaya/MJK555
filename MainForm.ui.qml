import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2


    Rectangle
    {
       // Идентификатор, по нему будет происходить
        // обращение к свойствам этого элемента
        id: canvas;
        color: "#3e8ef4"
        radius: 2
        property alias button_add: button_add
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0
        anchors.fill: parent
        border.width: 7
        border.color: "#0f6cb9"

        // Цвет фона, черный

        // Изменять размер под размеры
        // родительского элемента

        // Будет получать фокус ввода
        focus: true
        TreeView {
            id: treeView
            x: 7
            y: 57
            width: 243
            height: 365
            backgroundVisible: true
            frameVisible: true
        }

        Button {
            id: button_add
            x: 75
            y: 24
            text: qsTr("Добавление")


        }

        Button {
            id: button_left
            x: 288
            y: 435
            width: 28
            height: 27
        }

        Button {
            id: button_right
            x: 567
            y: 435
            width: 28
            height: 27
        }

        Button {
            id: button_turnright
            x: 513
            y: 435
            width: 28
            height: 27
        }

        Button {
            id: button_turnleft
            x: 345
            y: 435
            width: 28
            height: 27
        }

        Button {
            id: button_print
            x: 89
            y: 435
            text: qsTr("Печать")
            enabled: false
        }

        Button {
            id: button_close
            x: 599
            y: 11
            width: 28
            height: 27
        }

        Button {
            id: button_cut
            x: 401
            y: 435
            text: qsTr("Обрезать")
            tooltip: "Выделите изображение"
            enabled: false
        }

        Image {
            id: image
            x: 256
            y: 57
            width: 376
            height: 365

            MouseArea {
                id: mouseArea1
                x: 0
                y: 0
                width: 376
                height: 365
            }
            // source: "qrc:/qtquickplugin/images/template_image.png"
        }


    }







