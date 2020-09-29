void HandleRoot() {
  String responseHTML = "<html>"\
  "  <head>"\
  "   <style>"\
  "     input { outline: none; }"\
  "     html { font-family: Helvetica;"\
  "       display: inline-block;"\
  "       margin: 0px auto;"\
  "       text-align: center;"\
  "       font-size: 50px;"\
  "       background-color: #202020;"\
  "       color: white; "\
  "     }"\
  "     a.button { "\
  "       background-color: #195B6A; "\
  "       display: block;"\
  "       border: none; "\
  "       color: white; "\
  "       padding: 16px 40px;"\
  "       text-decoration: none; "\
  "       font-size: 75px;"\
  "       margin: 10px; "\
  "       cursor: pointer;"\
  "     }"\
  "     .center {"\
  "       text-align:center;"\
  "       width: 95%;"\
  "       position: absolute;"\
  "       margin:auto;"\
  "     }"\
  "     input[type=range] {"\
  "       -webkit-appearance: none;"\
  "       width: 95%;"\
  "       height: 50px;"\
  "       background-color:#202020"\
  "     }"\
  "     input[type=range]::-webkit-slider-runnable-track {"\
  "       width: 100%;"\
  "       height: 100px;"\
  "       cursor: pointer;"\
  "       box-shadow: 2px 2px 2px rgba(0,0,0,.5);"\
  "       border-radius: 50px;"\
  "       border: 0px solid #000101;"\
  "     }"\
  "     .rainbow::-webkit-slider-runnable-track {"\
  "       background: linear-gradient(to right,"\
  "         hsl(0, 100%, 50%) 0%, "\
  "         hsl(36, 100%, 50%) 10%,"\
  "         hsl(72, 100%, 50%) 20%,"\
  "         hsl(108, 100%, 50%) 30%,"\
  "         hsl(144, 100%, 50%) 40%,"\
  "         hsl(180, 100%, 50%) 50%,"\
  "         hsl(216, 100%, 50%) 60%,"\
  "         hsl(252, 100%, 50%) 70%,"\
  "         hsl(288, 100%, 50%) 80%,"\
  "         hsl(324, 100%, 50%) 90%,"\
  "         hsl(360, 100%, 50%) 100%"\
  "       );"\
  "     }"\
  "     .brightness::-webkit-slider-runnable-track {"\
  "       background: linear-gradient(to right,"\
  "         hsl(0, 0%, 0%) 0%,"\
  "         hsl(0, 100%, 100%) 100%"\
  "       );"\
  "     }"\
  "     input[type=range]::-webkit-slider-thumb {"\
  "       height: 100px;"\
  "       width: 100px;"\
  "       border-radius: 50%;"\
  "       cursor: pointer;"\
  "       -webkit-appearance: none;"\
  "       background: white;"\
  "       background-color: rgba(200, 200, 200, .8);"\
  "       box-shadow: 2px 2px 2px rgba(0,0,0,.5);"\
  "     }"\
  "   </style>"\
  ""\
  "   <script>"\
  "     function Update(id) {"\
  "       value = document.getElementById(id).value;"\
  "       if (!wait) window.location.assign('/color?' + id + '=' + value);"\
  "       else {wait=false;}"\
  "     }"\
  "     let wait = true;"\
  "   </script>"\
  ""\
  " </head>"\
  " <body onload='Update();'>"\
  "   <div class='center' id='center'>"\
  "     <br>"\
  "     <a class='button' href='/color?power=0' style='display:initial; background-color:red'>OFF</a>"\
  "     <a class='button' href='/color?power=1' style='display:initial; background-color:green'>ON</a>"\
  "     <br><br><hr>"\
  "     <h1 style='color:hsl(" + String(rots[0].count) + ", 100%, 50%);'>Hue</h1>"\
  "     <input id='hue' class='rainbow' type='range' min='0' max='360' value='" + String(rots[0].count) + "' oninput='Update(\"hue\");' /><br>"\
  "     <br><hr>"\
  "     <h1 style='color:hsl(0, 0%, " + String(rots[1].count / 2) + "%);'>Brightness</h1>"\
  "     <input id='light' class='brightness' type='range' min='1' max='255' value='" + String(rots[1].count) + "' oninput='Update(\"light\");' /><br>"\
  "     <br><hr>"\
  "     <h1>Presets</h1>"\
  "     <a class='button' href='/color?mode=1'>White</a>"\
  "     <a class='button' href='/color?mode=2'>Blue</a>"\
  "     <a class='button' href='/color?mode=3'>Red</a>"\
  "     <a class='button' href='/color?mode=4'>Rainbow</a>"\
  "     <a class='button' href='/color?mode=5'>Color Fade</a>"\
  "     <a class='button' href='/color?mode=6'>Fruit Loops</a>"\
  "     <a class='button' href='/color?mode=7'>Kit</a>"\
  "     <a class='button' href='/color?mode=8'>Space</a>"\
  "   <div>"\
  " </body>"\
  "</html>";
  
  webServer.send(200, "text/html", responseHTML);
}

void HandleColor() {
  int hue = -1;
  int light = -1;
  int cMode = -1;
  int power = -1;
  
  for (int i = 0; i < webServer.args(); i++) {
    if (webServer.argName(i) == "hue")
      hue = webServer.arg(i).toInt();
    else if (webServer.argName(i) == "light")
      light = webServer.arg(i).toInt();
    else if (webServer.argName(i) == "mode")
      cMode = webServer.arg(i).toInt();
    else if (webServer.argName(i) == "power")
      power = webServer.arg(i).toInt();
  }

  Serial.print("hue: ");
  Serial.println(hue);
  Serial.print("light: ");
  Serial.println(light);
  Serial.print("mode: ");
  Serial.println(cMode);

  if (hue != -1) {
    SetColorMode(COLOR);
    rots[0].count = hue;
    SetColor(hue);
  }
  if (light != -1) {
    rots[1].count = light;
    SetBrightness(light);
  }
  if (cMode != -1)
    SetColorMode(cMode);
  if (power != -1)
    SetPower(power);
  
  webServer.sendHeader("Location", "/");
  webServer.send(303);
}
