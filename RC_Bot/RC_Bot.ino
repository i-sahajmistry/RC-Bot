#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

// Replace with your network credentials
const char* ssid = "WIFI";
const char* password = "123456789";

bool ledState = 0;
const int ledPin = LED_BUILTIN;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
  html {
    font-family: Arial, Helvetica, sans-serif;
    text-align: center;
  }
  h1 {
    font-size: 1.8rem;
    color: white;
  }
  h2{
    font-size: 1.5rem;
    font-weight: bold;
    color: #143642;
  }
  .topnav {
    overflow: hidden;
    background-color: #143642;
  }
  body {
    margin: 0;
  }
  .content {
    padding: 30px;
    max-width: 600px;
    margin: 0 auto;
  }
  .card {
    background-color: #F8F7F9;;
    box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5);
    padding-top:10px;
    padding-bottom:20px;
  }
  .button {
    padding: 15px 50px;
    font-size: 24px;
    text-align: center;
    outline: none;
    color: #fff;
    background-color: #0f8b8d;
    border: none;
    border-radius: 5px;
    -webkit-touch-callout: none;
    -webkit-user-select: none;
    -khtml-user-select: none;
    -moz-user-select: none;
    -ms-user-select: none;
    user-select: none;
    -webkit-tap-highlight-color: rgba(0,0,0,0);
    }
    /*.button:hover {background-color: #0f8b8d}*/
    .button:active {
      background-color: #0f8b8d;
      box-shadow: 2 2px #CDCDCD;
      transform: translateY(2px);
    }
    .state {
      font-size: 1.5rem;
      color:#8c8c8c;
      font-weight: bold;
    }
    #container {
      /* background-color: black; */
      margin-top: 125px;
      display: grid;
      gap: 10px;
      grid-template-columns: repeat(3, 1fr);
    }
    .button {
      border: 2px solid rgb(221, 169, 0);
      border-radius: 40px;
      height: 100px
      background-size: contain;
      background-repeat: no-repeat;
      background-position: 50%, 50%;
    }
    .button:hover {
      border-color: red;
    }
    #left {
      background-image: url(https://drive.google.com/file/d/1hQZtFYp1RszfRIm9QW33MP5mYo9hsOvl/view?usp=sharing);
    }
    #right {
      background-image: url(https://drive.google.com/file/d/1XWsH8zeHpRvcE6TGgdYAJfZBbU43PRgb/view?usp=sharing);
    }
    #front {
      background-image: url(https://drive.google.com/file/d/1iLNZ7GTJMH8loJwh4nHBseNoOg9aYkxh/view?usp=sharing);
    }
    #back {
      background-image: url(https://drive.google.com/file/d/1RCd-2Gldub1kAtlEwBIVmrgWHA6zyR4Q/view?usp=sharing);
    }
    #anticlockwise {
      background-image: url(https://drive.google.com/file/d/1_cO-0U15CCY0hoE2h9IC4Hd51qD0o3aL/view?usp=sharing);
    }
    #clockwise {
      background-image: url(https://drive.google.com/file/d/1of_Kejhil3VUU-YsTAIUAfPs4gkdrnEI/view?usp=sharing);
    }
    #stop {
      background-image: url(https://drive.google.com/file/d/1y7Z9aJO944Ci4d_Lt_nWBIljOPxh9JDb/view?usp=sharing);
    }
    #stop {
      display: flex;
      justify-content: center;
      align-items: center;
      font-size: 30px;
      font-weight: bold;
    }
    #stop:hover {
      color: red;
    }
  </style>
<title>ESP Web Server</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="icon" href="data:,">
</head>
<body>
  <div class="topnav">
  <h1>ESP WebSocket Server</h1>
  </div>
  <div class="content">
    <div class="card">
      <h2>Output - RC Bot</h2>
      <p class="state">state: <span id="state">%STATE%</span></p>
      <p><button id="button" class="button">Toggle</button></p>
      <div id="container">
        <div class="" id="" ></div>
        <div class="button" id="front"     ></div>
        <div class="" id=""></div>
        <div class="button" id="left"  ></div>
        <div class="button" id="back"      ></div>
        <div class="button" id="right" ></div>
        <div class="button" id="anticlockwise" ></div>
        <div class="button" id="stop"      ></div>
        <div class="button" id="clockwise" ></div>
    </div>
    </div>
  </div>
<script>
  var gateway = `ws://${window.location.hostname}/ws`;
  var websocket;
  var d = '1010'
  var l = '000'
  var r = '000'
  window.addEventListener('load', onLoad);
  function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
    websocket.onmessage = onMessage; // <-- add this line
  }
  function onOpen(event) {
    console.log('Connection opened');
  }
  function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
  }
  function onMessage(event) {
    var state;
    if (event.data == "1"){
      state = "ON";
    }
    else{
      state = "OFF";
    }
    document.getElementById('state').innerHTML = state;
  }
  function onLoad(event) {
    initWebSocket();
    initButton();
  }
  function initButton() {
    document.getElementById('button').addEventListener('click', toggle);
  }
  function frontLeft(){
    websocket.send('1010100200');
  }
  function front(){
    websocket.send('1010100100');
  }
  function frontRight(){
    websocket.send('1010200100');
  }
  function backLeft(){
    websocket.send('0101100200');
  }
  function back(){
    websocket.send('0101100100');
  }
  function backRight(){
    websocket.send('0101200100');
  }
  function anticlockwise(){
    websocket.send('0110100100');
  }
  function clockwise(){
    websocket.send('1001100100');
  }
  function stop(){
    websocket.send('1010000000');
  }
  function sendSpeed(){
    websocket.send(`${d}${l}${r}`);
  }
  function toggle(){
    websocket.send(`toggle1`);
  }
</script>
</body>
</html>
)rawliteral";

void notifyClients() {
  ws.textAll(String(ledState));
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    data[len] = 0;
    if (strcmp((char*)data, "toggle1") == 0) {
      Serial.println((char*)data);
      ledState = !ledState;
      notifyClients();
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
    switch (type) {
      case WS_EVT_CONNECT:
        Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
        break;
      case WS_EVT_DISCONNECT:
        Serial.printf("WebSocket client #%u disconnected\n", client->id());
        break;
      case WS_EVT_DATA:
        handleWebSocketMessage(arg, data, len);
        break;
      case WS_EVT_PONG:
      case WS_EVT_ERROR:
        break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

String processor(const String& var){
  Serial.println(var);
  if(var == "STATE"){
    if (ledState){
      return "ON";
    }
    else{
      return "OFF";
    }
  }
  return String();
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  initWebSocket();

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Start server
  server.begin();
}

void loop() {
  ws.cleanupClients();
  digitalWrite(ledPin, ledState);
}
