radio.onReceivedString(function (receivedString) {
    // Forward the signal over USB serial to the Raspberry Pi
    serial.writeLine(receivedString)
    // Visual feedback on the receiver
    if (receivedString == "hot") {
        basic.showIcon(IconNames.Heart)
    } else if (receivedString == "cold") {
        basic.showIcon(IconNames.Diamond)
    }
    basic.pause(200)
    basic.clearScreen()
})
// MakeCode JavaScript — flash to the RECEIVER micro:bit (connected to Raspberry Pi via USB)
radio.setGroup(42)
