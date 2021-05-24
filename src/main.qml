import QtQuick 2.11
import QtQuick.Window 2.11

Window {
    id: window
    width: 800
    height: 480
    visible: true
    color: "#52473b" //Previously "#000000"
    title: qsTr("Museum Guide Robot GUI")

    property int menuheight : 280
    property int buttonspacingh : 120 //Horizontal spacing of buttons 120
    property int buttonspacingt : 20 //Spacing of buttons from top 20
    property int buttonspacingb : 40 //Spacing of buttons from bottom 50
    property int buttonrad : 7
    property int buttonwidth : 180
    property int buttonheight : 90
    property double buttonfontsize : 13.5
    property string buttonfontfam : "Arial"
    property string buttoncolourbase : "#006bc3" //"#005398" //Previously "#fbc3c3"
    property string buttoncolourback : "#004780" //"#003865" //Previously "#fb4848"
    property string buttonemstopcolour : "#ff9090"
    property string backgroundcolour : "#f5f5f5" //Previously "#de6e2e"
    property int bannerfontsize : 13
    property int mode : 0 //Tracks which frame is currently visible
    property string message : ""
    property var tournames: [logic_qml.getTourName(0), logic_qml.getTourName(1), logic_qml.getTourName(2)]
    property bool isadmin :false
    property bool logout : false
    property int totalTourCount : logic_qml.getTotalTourCount()

    Rectangle {
        id: logorectangle
        width: 150
        height: 30
        color: "#ffffff"
        radius: 10
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 15
        anchors.topMargin: 15

        Text {
            id: logotext
            x: 248
            y: 219
            anchors.fill: parent
            text: qsTr("Museum Guide Robot")
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: window.bannerfontsize
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors.horizontalCenter: parent.horizontalCenter
        }

        MouseArea {
            id: logomouseArea
            x: 5
            y: 8
            anchors.fill: parent
            onClicked: {
                textdisplaytext.text = qsTr("Hello there, I am your museum guide robot. Please click one of my buttons so I may help you")
                hideall();
                mainbuttonsrectangle.visible = true;
                statusrectangle.intour = false;
                statusrectangle.error = false;
            }
        }
    }

    Rectangle {
        id: adminbadgerectangle
        width: 80
        height: 30
        color: "#f6fe30"
        radius: 10
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.rightMargin: 120
        visible: isadmin ? true : false
        anchors.topMargin: 15
        Text {
            id: adminbadgetext
            x: 248
            y: 219
            anchors.fill:parent
            text: qsTr("Admin")
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: window.bannerfontsize
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors.horizontalCenter: parent.horizontalCenter
        }

        MouseArea {
            id: adminbadgeArea
            x: 5
            y: 8
            anchors.fill: parent
            onClicked:{
                ///Cancel tour edits
                window.message = textdisplaytext.text;
                textdisplaytext.text = qsTr("Welcome admin");
                hideall();
                adminbuttonsrectangle.visible = true;
            }
        }
    }

    Rectangle {
        id: userbadgerectangle
        width: 80
        height: 30
        color: "#ffffff"
        radius: 10
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.rightMargin: 120
        visible: isadmin ? false : true
        anchors.topMargin: 15
        Text {
            id: userbadgetext
            x: 248
            y: 219
            anchors.fill:parent
            text: qsTr("User")
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: window.bannerfontsize
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors.horizontalCenter: parent.horizontalCenter
        }

        MouseArea {
            id: userbadgeArea
            x: 5
            y: 8
            anchors.fill: parent
            onClicked:{
                window.message = textdisplaytext.text;
                textdisplaytext.text = qsTr("Please enter administrator password.");
                hideall();
                adminlogonrectangle.visible = true;
            }
        }
    }

    Rectangle {
        id: statusrectangle
        property bool intour: false
        property bool error: false
        property bool stopped : false
        property int tourno: 0
        width: 125
        height: 30
        color: error || stopped ? "#ff0000" : intour ? "#ffffff" : "#6ce43a" //Old intour colour "#0080e8"
        radius: 7
        anchors.top: parent.top
        anchors.horizontalCenterOffset: 0
        anchors.topMargin: 15
        anchors.horizontalCenter: parent.horizontalCenter
        Text {
            id: statustext
            height: parent.height
            width: parent.width
            text: statusrectangle.error ? qsTr("Fetch Admin") : statusrectangle.stopped ? qsTr("Stopped") : statusrectangle.intour ? qsTr(tournames[statusrectangle.tourno]): qsTr("Available")
            elide: Text.ElideRight
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            font.pixelSize: window.bannerfontsize
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.WordWrap
            maximumLineCount: 1
        }
    }

    Rectangle {
        id: powerbuttonrectangle
        x: 27
        y: 30
        width: 98
        height: 30
        color: "#f61212"
        radius: 7
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.rightMargin: 15
        anchors.topMargin: 15

        Text {
            id: powerbuttontext
            x: 65
            y: 34
            anchors.fill: parent
            text: isadmin ? qsTr("Shutdown") : qsTr("Report error")
            anchors.verticalCenter: parent.verticalCenter
            font.pixelSize: window.bannerfontsize
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            anchors.horizontalCenter: parent.horizontalCenter
        }

        MouseArea {
            id: powerbuttonmouseArea
            anchors.fill: parent
            onClicked:{
                ///Cancel tour edits
                if (isadmin == true){
                    shutdown();
                }else{
                    callhelpdesk();
                }
            }
        }
    }

    Rectangle {
        id: mainrectangle
        x: 0
        y: -201
        width: parent.width
        height: parent.height -60
        opacity: 1
        color: window.backgroundcolour
        border.width: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0

        Rectangle {
            id: textdisplayrectangle
            x: 238
            y: 37
            width: 0.7*parent.width
            height: 95
            color: "#ffffff"
            radius: 3
            anchors.horizontalCenter: parent.horizontalCenter
            visible: true

            Text {
                id: textdisplaytext
                x: 229
                y: 36
                width: parent.width
                height: parent.height
                text: qsTr("Hello, I am your Guide Robot. Please select an option below.")
                elide: Text.ElideMiddle
                anchors.verticalCenter: parent.verticalCenter
                font.pixelSize: 14
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.WordWrap
                font.family: "Arial"
                maximumLineCount: 5
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }

        Rectangle {
            id: mainbuttonsrectangle
            x: 0
            y: 140
            width: parent.width
            height: menuheight
            color: window.backgroundcolour
            border.width: 0
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottomMargin: 0
            visible: true

            Rectangle {
                id: starttourbuttonrectangle
                x: 80
                y: 15
                width: window.buttonwidth
                height: window.buttonheight
                color: window.buttoncolourbase
                radius: window.buttonrad
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.topMargin: window.buttonspacingt
                anchors.leftMargin: window.buttonspacingh
                Text {
                    id: starttourbuttontext
                    x: 65
                    y: 34
                    text: qsTr("Start tours")
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: window.buttonfontsize
                    horizontalAlignment: Text.AlignHCenter
                    font.family: window.buttonfontfam
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                MouseArea {
                    id: starttourmouseArea
                    anchors.fill: parent
                    onClicked:{
                        textdisplaytext.text = qsTr("Select a tour.");
                        if (totalTourCount < 1) {
                            textdisplaytext.text = qsTr("I'm sorry, there are no tours currently available.");
                        }
                        hideall();
                        starttourbuttonsrectangle.visible = true;
                        window.mode = 1;
                    }
                }

            }

            Rectangle {
                id: callhelpbuttonrectanglemain
                x: 80
                y: 144
                width: window.buttonwidth
                height: window.buttonheight
                color: window.buttoncolourbase
                radius: window.buttonrad
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                anchors.bottomMargin: window.buttonspacingb
                anchors.leftMargin: window.buttonspacingh
                Text {
                    id: callhelpbuttontextmain
                    x: 65
                    y: 34
                    text: qsTr("Report error")
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: window.buttonfontsize
                    horizontalAlignment: Text.AlignHCenter
                    font.family: window.buttonfontfam
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                MouseArea {
                    id: callhelpbuttonmouseAreamain
                    anchors.fill: parent
                    onClicked:{
                        callhelpdesk();
                    }
                }
            }

            Rectangle {
                id: giveinfoaboutbuttonrectangle
                x: 77
                width: window.buttonwidth
                height: window.buttonheight
                color: window.buttoncolourbase
                radius: window.buttonrad
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.topMargin: window.buttonspacingt
                anchors.rightMargin: window.buttonspacingh
                Text {
                    id: giveinfoaboutbuttontext
                    x: 65
                    y: 34
                    text: qsTr("More information")
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: window.buttonfontsize
                    horizontalAlignment: Text.AlignHCenter
                    font.family: window.buttonfontfam
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                MouseArea {
                    id: giveinfoaboutmouseArea
                    anchors.fill: parent
                    onClicked:{
                        logic_qml.giveInfoAbout();
                        textdisplaytext.text = logic_qml.speak();
                    }
                }
            }

            Rectangle {
                id: emstopbuttonrectangle
                x: 81
                y: 20
                width: window.buttonwidth
                height: window.buttonheight
                color: window.buttonemstopcolour
                radius: window.buttonrad
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.bottomMargin: window.buttonspacingb
                anchors.rightMargin: window.buttonspacingh
                Text {
                    id: emstopbuttontext
                    x: 65
                    y: 34
                    text: qsTr("Emergency Stop")
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: window.buttonfontsize
                    horizontalAlignment: Text.AlignHCenter
                    font.family: window.buttonfontfam
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                MouseArea {
                    id: emstopmouseArea
                    anchors.fill: parent
                    onClicked:{
                        logic_qml.emergencyStop();
                        statusrectangle.stopped = true;
                        window.message = textdisplaytext.text;
                        textdisplaytext.text = logic_qml.speak();
                        hideall();
                        resumemovingrectangle.visible = true;
                    }
                }
            }
        }

        Rectangle {
            id: starttourbuttonsrectangle
            width: parent.width
            height: menuheight
            color: window.backgroundcolour
            border.width: 0
            anchors.bottom: parent.bottom
            visible: false
            Rectangle {
                id: tour1buttonrectangle
                x: 80
                y: 15
                width: window.buttonwidth
                height: window.buttonheight
                color: window.buttoncolourbase
                radius: window.buttonrad
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.topMargin: window.buttonspacingt
                anchors.leftMargin: window.buttonspacingh
                visible: totalTourCount >= 1 ? true : false
                Text {
                    id: tour1buttontext
                    x: 65
                    y: 34
                    text: qsTr(tournames[0])
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: window.buttonfontsize
                    horizontalAlignment: Text.AlignHCenter
                    font.family: window.buttonfontfam
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                MouseArea {
                    id: tour1mouseArea
                    anchors.fill: parent
                    onClicked:{
                        textdisplaytext.text = qsTr("Starting " + tournames[0]);
                        intourbuttonsrectangle.visible = true;
                        starttourbuttonsrectangle.visible = false;
                        statusrectangle.intour = true;
                        statusrectangle.tourno = 0;
                        window.mode = 2;
                        logic_qml.startTour(statusrectangle.tourno);
                    }
                }
            }

            Rectangle {
                id: tour2buttonrectangle
                x: 395
                y: 16
                width: window.buttonwidth
                height: window.buttonheight
                color: window.buttoncolourbase
                radius: 7
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.topMargin: window.buttonspacingt
                anchors.rightMargin: window.buttonspacingh
                visible: totalTourCount >= 2 ? true : false
                Text {
                    id: tour2buttontext
                    x: 65
                    y: 34
                    text: qsTr(tournames[1])
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: window.buttonfontsize
                    horizontalAlignment: Text.AlignHCenter
                    font.family: window.buttonfontfam
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                MouseArea {
                    id: tour2buttonmouseArea
                    anchors.fill: parent
                    onClicked:{
                        textdisplaytext.text = qsTr("Starting " + tournames[1]);
                        intourbuttonsrectangle.visible = true;
                        starttourbuttonsrectangle.visible = false;
                        statusrectangle.intour = true;
                        statusrectangle.tourno = 1;
                        window.mode = 2;
                        logic_qml.startTour(statusrectangle.tourno);
                    }
                }
            }

            Rectangle {
                id: tour3buttonrectangle
                x: 80
                y: 144
                width: window.buttonwidth
                height: window.buttonheight
                color: window.buttoncolourbase
                radius: window.buttonrad
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                anchors.bottomMargin: window.buttonspacingb
                anchors.leftMargin: window.buttonspacingh
                visible: totalTourCount >= 3 ? true : false
                Text {
                    id: tour3buttontext
                    x: 65
                    y: 34
                    text: qsTr(tournames[2])
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: window.buttonfontsize
                    horizontalAlignment: Text.AlignHCenter
                    font.family: window.buttonfontfam
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                MouseArea {
                    id: tour3buttonmouseArea
                    anchors.fill: parent
                    onClicked:{
                        textdisplaytext.text = qsTr("Starting " + tournames[2]);
                        intourbuttonsrectangle.visible = true;
                        starttourbuttonsrectangle.visible = false;
                        statusrectangle.intour = true;
                        statusrectangle.tourno = 2;
                        window.mode = 2;
                        logic_qml.startTour(statusrectangle.tourno);
                    }
                }
            }

            Rectangle {
                id: toursbackbuttonrectangle
                x: 398
                y: 137
                width: window.buttonwidth
                height: window.buttonheight
                color: window.buttoncolourback
                radius: window.buttonrad
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.bottomMargin: window.buttonspacingb
                anchors.rightMargin: window.buttonspacingh
                Text {
                    id: toursbackbuttontext
                    x: 65
                    y: 34
                    text: qsTr("Back to menu")
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: window.buttonfontsize
                    horizontalAlignment: Text.AlignHCenter
                    font.family: window.buttonfontfam
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                MouseArea {
                    id: toursbackbuttonmouseArea
                    anchors.fill: parent
                    onClicked:{
                        hideall();
                        mainbuttonsrectangle.visible = true;
                        textdisplaytext.text = qsTr("Hello there, I am your museum guide robot. Please click one of my buttons so I may help you.")
                        window.mode = 0;
                    }
                }
            }


            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottomMargin: 0
        }

        Rectangle {
            id: intourbuttonsrectangle
            x: 0
            y: 140
            width: parent.width
            height: menuheight
            visible: false
            color: window.backgroundcolour
            border.width: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.horizontalCenter: parent.horizontalCenter

            Rectangle {
                id: moreinfobuttonrectangle
                property bool givingmoreinfo : false //Status if more information is being given, prevents constant repress of button
                x: 80
                y: 15
                width: window.buttonwidth
                height: window.buttonheight
                color: window.buttoncolourbase
                radius: window.buttonrad
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.topMargin: window.buttonspacingt
                anchors.leftMargin: window.buttonspacingh
                Text {
                    id: moreinfobuttontext
                    x: 65
                    y: 34
                    text: qsTr("More information")
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: window.buttonfontsize
                    horizontalAlignment: Text.AlignHCenter
                    font.family: window.buttonfontfam
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                MouseArea {
                    id: moreinfomouseArea
                    anchors.fill: parent
                    visible: moreinfobuttonrectangle.givingmoreinfo ? false : true
                    onClicked:{
                        moreinfobuttonrectangle.givingmoreinfo = true; //disables button from being pressed again
                        logic_qml.giveInfo();
                        textdisplaytext.text = qsTr(logic_qml.speak());
                        moreinfobuttonrectangle.givingmoreinfo = false; //enables button being pressed again
                    }
                }
            }

            Rectangle {
                id: callhelpbuttonrectangletour
                x: 80
                y: 144
                width: window.buttonwidth
                height: window.buttonheight
                color: window.buttoncolourbase
                radius: 7
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                anchors.leftMargin: window.buttonspacingh
                anchors.bottomMargin: window.buttonspacingb
                Text {
                    id: callhelpbuttontexttour
                    x: 65
                    y: 34
                    text: qsTr("Report error")
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: window.buttonfontsize
                    horizontalAlignment: Text.AlignHCenter
                    font.family: window.buttonfontfam
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                MouseArea {
                    id: callhelpbuttonmouseAreatour
                    anchors.fill: parent
                    onClicked:{
                        callhelpdesk();
                    }
                }
            }

            Rectangle {
                id: nexttourpointbuttonrectangle
                x: 86
                y: 15
                width: window.buttonwidth
                height: window.buttonheight
                color: window.buttoncolourbase
                radius: window.buttonrad
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.rightMargin: window.buttonspacingh
                anchors.topMargin: window.buttonspacingt
                Text {
                    id: nexttourpointbuttontext
                    x: 65
                    y: 34
                    text: qsTr("Next tour point")
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: window.buttonfontsize
                    horizontalAlignment: Text.AlignHCenter
                    font.family: window.buttonfontfam
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                MouseArea {
                    id: nexttourpointmouseArea
                    anchors.fill: parent
                    onClicked: {
                        if (logic_qml.getlocation() < 255){
                            textdisplaytext.text = qsTr("Moving to next tour point.");
                            logic_qml.goNextTourPoint();
                            if (logic_qml.getlocation() >= 255) {///GUI checks tour point before it has been reached, button won't update to finished state
                                nexttourpointbuttontext.text =  qsTr("Finish tour");
                                nexttourpointbuttonrectangle.color = window.buttoncolourback;
                            }
                        } else {
                            textdisplaytext.text = qsTr("Finishing tour, thank you for visiting us today!")
                            nexttourpointbuttontext.text =  qsTr("Next tour point");
                            nexttourpointbuttonrectangle.color = window.buttoncolourbase;
                            stoptour();
                        }

                    }
                }
            }


            Rectangle {
                id: emstoptourbuttonrectangle
                x: 81
                y: 20
                width: window.buttonwidth
                height: window.buttonheight
                color: window.buttonemstopcolour
                radius: window.buttonrad
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.bottomMargin: window.buttonspacingb
                anchors.rightMargin: window.buttonspacingh
                visible: isadmin ? false : true
                Text {
                    id: emstoptourbuttontext
                    x: 65
                    y: 34
                    text: qsTr("Emergency Stop")
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: window.buttonfontsize
                    horizontalAlignment: Text.AlignHCenter
                    font.family: window.buttonfontfam
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                MouseArea {
                    id: emstoptourmouseArea
                    anchors.fill: parent
                    onClicked:{
                        logic_qml.emergencyStop();
                        statusrectangle.stopped = true;
                        window.message = textdisplaytext.text;
                        textdisplaytext.text = logic_qml.speak();
                        hideall();
                        resumemovingrectangle.visible = true;
                    }
                }
            }


            Rectangle {
                id: stoptourbuttonrectangle
                x: 395
                y: 16
                width: window.buttonwidth
                height: window.buttonheight
                color: window.buttoncolourback
                radius: window.buttonrad
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.bottomMargin: window.buttonspacingb
                anchors.rightMargin: window.buttonspacingh
                visible: isadmin ? true : false
                Text {
                    id: stoptourbuttontext
                    x: 65
                    y: 34
                    text: qsTr("Stop Tour")
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: window.buttonfontsize
                    horizontalAlignment: Text.AlignHCenter
                    font.family: window.buttonfontfam
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                MouseArea {
                    id: stoptourbuttonmouseArea
                    anchors.fill: parent
                    onClicked:{
                        textdisplaytext.text = qsTr("Closing tour, come again soon.");
                        stoptour();
                    }
                }
            }

        }

        Rectangle {
            id: resumemovingrectangle
            x: 0
            y: 140
            width: parent.width
            height: menuheight
            visible: false
            color: window.backgroundcolour
            border.width: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.horizontalCenter: parent.horizontalCenter

            Rectangle {
                id: resumemovingarearectangle
                x: 0
                width: 320
                height: 140
                color: "#000000"
                radius: 3
                border.width: 0
                anchors.top: parent.top
                anchors.topMargin: 0
                anchors.horizontalCenter: parent.horizontalCenter

                Rectangle {
                    id: resumemovingbuttonrectangle
                    x: 80
                    y: 72
                    width: 200
                    height: 75
                    color: window.buttoncolourbase
                    radius: 7
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    Text {
                        id: resumemovingbuttontext
                        x: 65
                        y: 34
                        text: qsTr("Resume Moving")
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 14
                        horizontalAlignment: Text.AlignHCenter
                        font.family: window.buttonfontfam
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    MouseArea {
                        id: resumemovingmouseArea
                        anchors.fill: parent
                        onClicked:{
                            logic_qml.resumeMoving();
                            statusrectangle.stopped = false;
                            resumemovingrectangle.visible = false;
                            unhide();
                            textdisplaytext.text = window.message;
                        }
                    }
                }
            }
        }


        Rectangle {
            id: tourdesignrectangle
            x: 0
            y: 140
            width: parent.width
            height: menuheight
            visible: false
            color: window.backgroundcolour
            border.width: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.horizontalCenter: parent.horizontalCenter

            Rectangle {
                id: tourdesignarearectangle
                x: 0
                width: 420
                height: 140
                color: "#000000"
                radius: 3
                border.width: 0
                anchors.top: parent.top
                anchors.topMargin: 0
                anchors.horizontalCenter: parent.horizontalCenter

                Rectangle {
                    id: tourdesigninputrectangle
                    x: 238
                    width: 350
                    height: 30
                    color: "#ffffff"
                    radius: 3
                    anchors.top: parent.top
                    anchors.topMargin: 25
                    anchors.horizontalCenter: parent.horizontalCenter

                    TextInput {
                        id: tourdesigntextInput
                        x: 4
                        y: -2
                        text: qsTr("")
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.fill: parent
                        font.pixelSize: 12
                        verticalAlignment: Text.AlignVCenter
                        font.family: window.buttonfontfam
                        selectByMouse: true
                        overwriteMode: true
                        maximumLength: 32
                        echoMode: TextInput.Password
                        anchors.rightMargin: 5
                        anchors.leftMargin: 5
                        anchors.horizontalCenter: parent.horizontalCenter
                        passwordMaskDelay: 500
                    }
                }

                Rectangle {
                    id: submittourdesignbuttonrectangle
                    x: 80
                    y: 72
                    width: 129
                    height: 44
                    color: window.buttoncolourbase
                    radius: 7
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.rightMargin: 60
                    anchors.bottomMargin: 25
                    Text {
                        id: submittourdesignbuttontext
                        x: 65
                        y: 34
                        text: qsTr("Submit")
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 12
                        horizontalAlignment: Text.AlignHCenter
                        font.family: window.buttonfontfam
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    MouseArea {
                        id: submittourdesignmouseArea
                        anchors.fill: parent
                        onClicked:{
                            ///logic_qml method to submit
                            ///Submit data to method
                            logic_qml.tourConfirmGUI();
                            textdisplaytext.text = logic_qml.speakTour();
                        }
                    }
                }

                Rectangle {
                    id: canceltourdesignbuttonrectangle
                    y: 68
                    width: 129
                    height: 44
                    color: window.buttoncolourback
                    radius: 7
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                    anchors.leftMargin: 60
                    anchors.bottomMargin: 25
                    Text {
                        id: canceltourdesignbuttontext
                        x: 65
                        y: 34
                        text: qsTr("Cancel")
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 12
                        horizontalAlignment: Text.AlignHCenter
                        font.family: window.buttonfontfam
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    MouseArea {
                        id: canceltourdesignmouseArea
                        anchors.fill: parent
                        onClicked:{
                            tourdesigntextInput.text = "";
                            admintourbuttonsrectangle.visible = true;
                            tourdesignrectangle.visible = false;
                            textdisplaytext.text = qsTr("Welcome admin");
                            ///send signal to logic to cancel tour edit
                        }
                    }
                }
            }
        }


        Rectangle {
            id: adminlogonrectangle
            x: 0
            y: 140
            width: parent.width
            height: menuheight
            visible: false
            color: window.backgroundcolour
            border.width: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.horizontalCenter: parent.horizontalCenter

            Rectangle {
                id: logonarearectangle
                x: 0
                width: 420
                height: 140
                color: "#000000"
                radius: 3
                border.width: 0
                anchors.top: parent.top
                anchors.topMargin: 0
                anchors.horizontalCenter: parent.horizontalCenter

                Rectangle {
                    id: adminlogonPasswordrectangle
                    x: 238
                    width: 350
                    height: 30
                    color: "#ffffff"
                    radius: 3
                    anchors.top: parent.top
                    anchors.topMargin: 25
                    anchors.horizontalCenter: parent.horizontalCenter

                    TextInput {
                        id: adminlogonPasswordtextInput
                        x: 4
                        y: -2
                        text: qsTr("")
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.fill: parent
                        font.pixelSize: 12
                        verticalAlignment: Text.AlignVCenter
                        font.family: window.buttonfontfam
                        selectByMouse: true
                        overwriteMode: true
                        maximumLength: 32
                        echoMode: TextInput.Password
                        anchors.rightMargin: 5
                        anchors.leftMargin: 5
                        anchors.horizontalCenter: parent.horizontalCenter
                        passwordMaskDelay: 500
                    }
                }

                Rectangle {
                    id: submitlogonbuttonrectangle
                    x: 80
                    y: 72
                    width: 129
                    height: 44
                    color: window.buttoncolourbase
                    radius: 7
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    anchors.rightMargin: 60
                    anchors.bottomMargin: 25
                    Text {
                        id: submitlogonbuttontext
                        x: 65
                        y: 34
                        text: qsTr("Submit")
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 12
                        horizontalAlignment: Text.AlignHCenter
                        font.family: window.buttonfontfam
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    MouseArea {
                        id: submitlogonmouseArea
                        anchors.fill: parent
                        onClicked:{
                            adminlogonPasswordtextInput.accepted();
                            logon();
                        }
                    }
                }

                Rectangle {
                    id: cancellogonbuttonrectangle
                    y: 68
                    width: 129
                    height: 44
                    color: window.buttoncolourback
                    radius: 7
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                    anchors.leftMargin: 60
                    anchors.bottomMargin: 25
                    Text {
                        id: cancellogonbuttontext
                        x: 65
                        y: 34
                        text: qsTr("Cancel")
                        anchors.verticalCenter: parent.verticalCenter
                        font.pixelSize: 12
                        horizontalAlignment: Text.AlignHCenter
                        font.family: window.buttonfontfam
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                    MouseArea {
                        id: cancellogonmouseArea
                        anchors.fill: parent
                        onClicked:{
                            adminlogonPasswordtextInput.text = "";
                            window.logout = false;
                            statusrectangle.error = false;
                            adminlogonrectangle.visible = false;
                            unhide();
                        }
                    }
                }
            }
        }

        Rectangle {
            id: adminbuttonsrectangle
            x: 0
            y: 140
            width: parent.width
            height: menuheight
            visible: false
            color: window.backgroundcolour
            border.width: 0
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottomMargin: 0

            Rectangle {
                id: tourlistbuttonrectangle
                x: 80
                y: 15
                width: window.buttonwidth
                height: window.buttonheight
                color: window.buttoncolourbase
                radius: window.buttonrad
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.topMargin: window.buttonspacingt
                anchors.leftMargin: window.buttonspacingh
                Text {
                    id: tourlistbuttontext
                    x: 65
                    y: 34
                    text: qsTr("Tours Info")
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: window.buttonfontsize
                    horizontalAlignment: Text.AlignHCenter
                    font.family: window.buttonfontfam
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                MouseArea {
                    id: tourlistmouseArea
                    anchors.fill: parent
                    onClicked: {
                        adminbuttonsrectangle.visible = false;
                        admintourbuttonsrectangle.visible = true;
                    }
                }
            }

            Rectangle {
                id: adminlogoutbuttonrectangle
                x: 77
                width: window.buttonwidth
                height: window.buttonheight
                color: window.buttoncolourbase
                radius: window.buttonrad
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.rightMargin: window.buttonspacingh
                anchors.topMargin: window.buttonspacingt
                Text {
                    id: ccountbuttontext1
                    x: 65
                    y: 34
                    text: qsTr("Logout")
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: window.buttonfontsize
                    horizontalAlignment: Text.AlignHCenter
                    font.family: window.buttonfontfam
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                MouseArea {
                    id: adminlogoutmouseArea
                    anchors.fill: parent
                    onClicked: {
                        isadmin = false;
                        adminbuttonsrectangle.visible = false;
                        unhide();
                    }
                }
            }

            Rectangle {
                id: adminshutdownbuttonrectangle
                x: 81
                y: 20
                width: window.buttonwidth
                height: window.buttonheight
                color: window.buttoncolourbase
                radius: window.buttonrad
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                anchors.leftMargin: window.buttonspacingh
                anchors.bottomMargin: window.buttonspacingb
                Text {
                    id: adminshutdownbuttontext
                    x: 65
                    y: 34
                    text: qsTr("Shutdown")
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: window.buttonfontsize
                    horizontalAlignment: Text.AlignHCenter
                    font.family: window.buttonfontfam
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                MouseArea {
                    id: adminshutdownmouseArea
                    anchors.fill: parent
                    onClicked: {
                        shutdown();
                    }
                }
            }

            Rectangle {
                id: adminbackbuttonrectanglemain
                x: 80
                y: 144
                width: window.buttonwidth
                height: window.buttonheight
                color: window.buttoncolourback
                radius: window.buttonrad
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.rightMargin: window.buttonspacingh
                anchors.bottomMargin: window.buttonspacingb
                Text {
                    id: callhelpbuttontextmain1
                    x: 65
                    y: 34
                    text: qsTr("Go Back")
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: window.buttonfontsize
                    horizontalAlignment: Text.AlignHCenter
                    font.family: window.buttonfontfam
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                MouseArea {
                    id: adminbackbuttonmouseAreamain
                    anchors.fill: parent
                    onClicked: {
                        adminbuttonsrectangle.visible = false;
                        unhide();
                    }
                }
            }
        }

        Rectangle {
            id: admintourbuttonsrectangle
            x: 0
            y: 140
            width: parent.width
            height: menuheight
            visible: false
            color: window.backgroundcolour
            border.width: 0
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottomMargin: 0

            Rectangle {
                id: writetourbuttonrectangle
                x: 80
                y: 15
                width: window.buttonwidth
                height: window.buttonheight
                color: window.buttoncolourbase
                radius: window.buttonrad
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.topMargin: window.buttonspacingt
                anchors.leftMargin: window.buttonspacingh
                Text {
                    id: writetourbuttontext
                    x: 65
                    y: 34
                    text: qsTr("Write tour")
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: window.buttonfontsize
                    horizontalAlignment: Text.AlignHCenter
                    font.family: window.buttonfontfam
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                MouseArea {
                    id: writetourmouseArea
                    anchors.fill: parent
                    onClicked: {
                        logic_qml.tourWrite(); ///Implement code for writing tour
                        admintourbuttonsrectangle.visible = false;
                        tourdesignrectangle.visible = true;
                        textdisplaytext.text = qsTr("Writing tour. Please enter new tour name.");
                    }
                }
            }

            Rectangle {
                id: updatetourbuttonrectangle
                x: 80
                y: 15
                width: window.buttonwidth
                height: window.buttonheight
                color: window.buttoncolourbase
                radius: window.buttonrad
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.topMargin: window.buttonspacingt
                anchors.rightMargin: window.buttonspacingh
                Text {
                    id: updatetourbuttontext
                    x: 65
                    y: 34
                    text: qsTr("Update tour")
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: window.buttonfontsize
                    horizontalAlignment: Text.AlignHCenter
                    font.family: window.buttonfontfam
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                MouseArea {
                    id: updatetourmouseArea
                    anchors.fill: parent
                    onClicked: {
                        //QtConcurrent::run(QThreadPool::globalInstance(), logic_qml.tourUpdate());
                        //QFuture<void> t1 = QtConcurrent::run(logic_qml::tourupdate);
                        logic_qml.tourUpdate(); ///Implement code for tour updating
                        admintourbuttonsrectangle.visible = false;
                        tourdesignrectangle.visible = true;
                        textdisplaytext.text = qsTr("Writing tour. Please enter the name of the tour to be updated.");
                    }
                }
            }

            Rectangle {
                id: admintourlistbuttonrectangle
                x: 81
                y: 20
                width: window.buttonwidth
                height: window.buttonheight
                color: window.buttoncolourbase
                radius: window.buttonrad
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                anchors.leftMargin: window.buttonspacingh
                anchors.bottomMargin: window.buttonspacingb
                Text {
                    id: admintourlistbuttontext
                    x: 65
                    y: 34
                    text: qsTr("Tour list")
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: window.buttonfontsize
                    horizontalAlignment: Text.AlignHCenter
                    font.family: window.buttonfontfam
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                MouseArea {
                    id: admintourlistmouseArea
                    anchors.fill: parent
                    onClicked: {
                        switch (window.totalTourCount){
                        case(0) :
                            textdisplaytext.text = qsTr("There are no stored tours");
                            break;
                        case(1) :
                            textdisplaytext.text = qsTr("There is 1 stored tour: " + tournames[0]);
                            break;
                        case(2):
                            textdisplaytext.text = qsTr("There are 2 stored tours: " + tournames[0] + " and " + tournames[1]);
                            break;
                        case(3):
                            textdisplaytext.text = qsTr("There are 3 stored tours: " + tournames[0] + ", " + tournames[1] + " and " + tournames[2]);
                            break;
                        default:
                            textdisplaytext.text = qsTr("There are "+ window.totalTourCount +" stored tours, of which: " + tournames[0] + ", " + tournames[1] + ", " + tournames[2] + " are available");
                        }


                    }
                }
            }

            Rectangle {
                id: admintourbackbuttonrectanglemain
                x: 80
                y: 144
                width: window.buttonwidth
                height: window.buttonheight
                color: window.buttoncolourback
                radius: window.buttonrad
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.rightMargin: window.buttonspacingh
                anchors.bottomMargin: window.buttonspacingb
                Text {
                    id: admintourbackbuttontextmain
                    x: 65
                    y: 34
                    text: qsTr("Go Back")
                    anchors.verticalCenter: parent.verticalCenter
                    font.pixelSize: window.buttonfontsize
                    horizontalAlignment: Text.AlignHCenter
                    font.family: window.buttonfontfam
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                MouseArea {
                    id: admintourbackbuttonmouseAreamain
                    anchors.fill: parent
                    onClicked: {
                        admintourbuttonsrectangle.visible = false;
                        adminbuttonsrectangle.visible = true;
                    }
                }
            }
        }
    }


    function stoptour(){
        logic_qml.stopTour();
        statusrectangle.intour = false;
        hideall();
        mainbuttonsrectangle.visible = true;
        window.mode = 0;
    }


    function callhelpdesk(){
        logic_qml.callHelp();
        window.message = textdisplaytext.text;
        textdisplaytext.text = qsTr(logic_qml.speak());
        statusrectangle.error = true;
        hideall();
        adminlogonrectangle.visible = true;
    }


    function hideall(){
        mainbuttonsrectangle.visible = false;
        starttourbuttonsrectangle.visible = false;
        intourbuttonsrectangle.visible = false
        adminbuttonsrectangle.visible = false;
        admintourbuttonsrectangle.visible = false;
    }


    function unhide(){
        textdisplaytext.text = window.message;
        switch (window.mode){
        case 0:
            mainbuttonsrectangle.visible = true;
            break;
        case 1:
            starttourbuttonsrectangle.visible = true;
            break;
        case 2:
            intourbuttonsrectangle.visible = true;
            break;
        default:
            textdisplaytext.text = qsTr("Unable to identify current mode");
            mainbuttons.rectangle.visible = true;
            window.mode = 0;
            logic_qml.stopTour();
            break;
        }
    }


    function logon(){
        if (adminlogonPasswordtextInput.text == "hi"){
            if (window.logout){
                Qt.quit();
            }else{
                adminlogonPasswordtextInput.text = "";
                isadmin = true;
                adminlogonrectangle.visible = false;
                unhide();
                statusrectangle.error = false;
            }
        }else{
            adminlogonPasswordtextInput.text = "";
            if (window.logout) {
                textdisplaytext.text = "Confirm administrator password to shutdown. Wrong password entered."
            }else{
                textdisplaytext.text = "Please enter administrator password. Wrong password entered";
            }
        }
    }


    function shutdown(){
        window.message = textdisplaytext.text;
        textdisplaytext.text = "Confirm administrator password to shutdown.";
        window.logout = true;
        hideall();
        adminlogonrectangle.visible = true;
    }

}
