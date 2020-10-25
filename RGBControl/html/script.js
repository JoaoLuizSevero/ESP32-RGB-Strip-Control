var socket;
connectWebSocket();
var activeLed = 0
var lastSize
var ledsArray = []
var actualJson

function selectLed(element) {
    activeLed = Number(document.querySelector("#" + element.id).innerHTML) - 1
    document.getElementById('colorBox').value = ledsArray[activeLed]["color"]
    changeColorBox();
}

// setInterval(function(){
// }, 100)

function changeLeds() {
    let size = document.querySelector("#size").value
    size < 1 ? size = 1 : size = size
    size > 10 ? size = 10 : size = size
    document.querySelector("#size").value = size
    let op = lastSize - size
    lastSize = size
    if (op < 0) {
        for (let i = 0; i < (op * -1); i++) {
            ledsArray.push({ "n": size - 1 + i, "color": "#0000ff" })
        }
    } else {
        for (let i = 0; i < op; i++) {
            ledsArray.pop()
        }
    }
    document.querySelector("#leds").innerHTML = ""
    for (let i = 0; i < size; i++) {
        document.querySelector("#leds").innerHTML += "<button class='colorButton' id='buttonLed" + i + "' name='led' value='" + i + "' style='background-color: " + ledsArray[i]["color"] + ";' onclick='selectLed(this)'>" + (i + 1)
    }
}

function changeColorBox() {
    let color = document.getElementById("colorBox").value
    let r = color.slice(1, -4)
    let g = color.slice(3, -2)
    let b = color.slice(5)
    document.getElementById("r").value = parseInt(r, 16)
    document.getElementById("g").value = parseInt(g, 16)
    document.getElementById("b").value = parseInt(b, 16)
    document.getElementById("buttonLed" + activeLed).style.backgroundColor = color
    ledsArray[activeLed]["color"] = color
}

function rgbChange() {
    let r = document.getElementById("r").value
    let g = document.getElementById("g").value
    let b = document.getElementById("b").value
    r < 0 ? r = 0 : r = r
    g < 0 ? g = 0 : g = g
    b < 0 ? b = 0 : b = b
    r > 255 ? r = 255 : r = r
    g > 255 ? g = 255 : g = g
    b > 255 ? b = 255 : b = b
    document.getElementById("r").value = r
    document.getElementById("g").value = g
    document.getElementById("b").value = b
    let color = "#" + decToHex(r) + decToHex(g) + decToHex(b)
    document.getElementById("colorBox").value = color
    document.getElementById("buttonLed" + activeLed).style.backgroundColor = color
    ledsArray[activeLed]["color"] = color
}

function resetLeds() {
    let size = document.querySelector("#size").value
    for (let i = 0; i < size; i++) {
        ledsArray.pop()
    }
    var jsonData = JSON.parse(actualJson);
    size = jsonData["payload"]["size"]
    lastSize = size
    document.querySelector("#size").value = size
    for (let i = 0; i < size; i++) {
        let r = decToHex(jsonData["payload"]["nodes"]["node_" + i]["r"])
        let g = decToHex(jsonData["payload"]["nodes"]["node_" + i]["g"])
        let b = decToHex(jsonData["payload"]["nodes"]["node_" + i]["b"])
        let color = "#" + r + g + b
        ledsArray.push({ "n": i, "color": color });
    }
    document.querySelector("#leds").innerHTML = ""
    for (let i = 0; i < size; i++) {
        document.querySelector("#leds").innerHTML += "<button class='colorButton' id='buttonLed" + i + "' name='led' value='" + i + "' style='background-color: " + ledsArray[i]["color"] + ";' onclick='selectLed(this)'>" + (i + 1)
    }
    document.getElementById('colorBox').value = ledsArray[0]["color"]
    changeColorBox();
}

function getConfigJson() {
    var JsonData = {
        "getConfig": {
            "message": null
        }
    }
    console.log("SEND MESSAGE: " + JSON.stringify(JsonData));
    socket.send(JSON.stringify(JsonData))
}


function saveConfig() {
    let size = document.querySelector("#size").value
    console.log(size)
    var JsonData = `{"payload":{"size":` + size + `,"nodes":{`
    for (let i = 0; i < size; i++) {
        let color = ledsArray[i]["color"]
        let r = parseInt(color.slice(1, -4), 16)
        let g = parseInt(color.slice(3, -2), 16)
        let b = parseInt(color.slice(5), 16)
        JsonData += `"node_` + i + `":{"0":` + r + `,"1":` + g + `,"2":` + b + `}`
        if (i + 1 < size) {
            JsonData += ","
        }
    }
    JsonData += `}}}`
    socket.send(JsonData)
    console.log(JsonData)
}

function connectWebSocket() {
    socket = new WebSocket('ws://' + location.hostname + ':81/', ['arduino']);
    //socket = new WebSocket('ws://192.168.4.1:81/', ['arduino']);

    socket.binaryType = 'arraybuffer';

    socket.onopen = function () {
        console.log('Connect ' + new Date());
        getConfigJson()
    };

    socket.onclose = function (e) {
        console.log('Socket is closed. Reconnect will be attempted in 1000 miliseconds.', e.reason);
        setTimeout(function () {
            connectWebSocket();
        }, 1000);
    };

    socket.onerror = function (err) {
        console.error('Socket encountered error: ', err.message, 'Closing socket');
        socket.close();
    };

    socket.onmessage = function (e) {
        var jsonData = JSON.parse(e.data);
        switch (Object.keys(jsonData)[0]) {
            case "payload":
                actualJson = JSON.stringify(jsonData)
                console.log("INCOMING MESSAGE: " + actualJson);
                resetLeds()
                break;
        }
    }
}

function decToHex(n) {
    let sn = Number(n).toString(16)
    n < 16 ? sn = "0" + sn : sn = sn
    return sn
}
