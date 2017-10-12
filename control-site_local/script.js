$( document ).ready(function() {

    // Device
    var address = "192.168.88.100"; // Αυτό αλλάζετε!
    var device = new Device(address);

    // Buttons
    $('#forward').mousedown(function() {
      device.callFunction("forward");
    });
    $('#forward').mouseup(function() {
      device.callFunction("stop");
    });

    $('#right').mousedown(function() {
      device.callFunction("right");
    });
    $('#right').mouseup(function() {
      device.callFunction("stop");
    });

    $('#left').mousedown(function() {
      device.callFunction("left");
    });
    $('#left').mouseup(function() {
      device.callFunction("stop");
    });

    $('#backward').mousedown(function() {
      device.callFunction("backward");
    });
    $('#backward').mouseup(function() {
      device.callFunction("stop");
    });


    $('#leftSpin').mousedown(function() {
      device.callFunction("leftSpin");
    });
    $('#leftSpin').mouseup(function() {
      device.callFunction("stop");
    });


    $('#rightSpin').mousedown(function() {
      device.callFunction("rightSpin");
    });
    $('#rightSpin').mouseup(function() {
      device.callFunction("stop");
    });


});
