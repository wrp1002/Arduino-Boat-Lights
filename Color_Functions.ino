void UpdateColors() {
  switch(stripData.colorMode) {
    case RAINBOW:
    {
      if (millis() % 1 == 0) {
        stripData.targetColor = (stripData.targetColor + 150) % 65536L;
        
        for(int i=0; i<strip.numPixels(); i++) {
          int pixelHue = stripData.targetColor + ((strip.numPixels() - i) * 65536L / strip.numPixels());
          strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
        }
        strip.show();
      }
      
      break;
    }

    case FADE:
    {
      if (rots[0].count == stripData.targetColor)
        stripData.fadeTimer--;
        
      if (stripData.fadeTimer <= 0) {
        Serial.println("CHANING");
        //while (stripData.targetColor == rots[0].count)
          stripData.targetColor = rand() % 360;
        stripData.fadeTimer = stripData.fadeTimerStart;
        Serial.println("CHANDED");
        Serial.println(stripData.targetColor);
      }
      
      if (millis() % 10 == 0 && stripData.targetColor != rots[0].count) {
        if (rots[0].count > stripData.targetColor)
          rots[0].count = (rots[0].count - 1) % 360;
        else
          rots[0].count = (rots[0].count + 1) % 360;

        Serial.print(rots[0].count);
        Serial.print(" target:");
        Serial.println(stripData.targetColor);

        SetColor(rots[0].count);
      }
      break;
    }

    case LUCKY_CHARMS:
    {
      if (millis() % 100 == 0) {
        for (int i = 0; i < strip.numPixels(); i++)
          strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(rand() % 65536)));
        strip.show();
      }
      break;
    }

    case KIT:
    {
      if (millis() % 25 == 0) {
        SetColorRGB(0, 0, 0);
  
        stripData.targetColor += stripData.dir;
  
        if (stripData.targetColor <= 0 || stripData.targetColor >= strip.numPixels())
          stripData.dir *= -1;
        int place = stripData.targetColor;
        
        strip.setPixelColor(place, strip.Color(255, 0, 0));
        for (int i = 0; i < KIT_WIDTH; i++) {
          if (place + i <= strip.numPixels())
            strip.setPixelColor(place + i, strip.Color(255 - (i + 1) * 70, 0, 0));
          if (place - i >= 0)
            strip.setPixelColor(place - i, strip.Color(255 - (i + 1) * 70, 0, 0));
        }
        
        strip.show();
      }
  
      break;
    }

    case SPACE:
    {
      stripData.fadeTimer--;
      
      if (stripData.fadeTimer <= 0) {
        stripData.fadeTimer = 125;
        SetColorRGB(0, 0, 0);
  
        stripData.targetColor = (stripData.targetColor + 1) % 5;
        
        for (int i = stripData.targetColor; i < strip.numPixels(); i += 5)
          strip.setPixelColor(i, strip.Color(0, 0, 255));
        
        strip.show();

        Serial.println(stripData.targetColor);
      }
  
      break;
    }
    
  }
}

void SetColorMode(int newColorMode) {
  stripData.colorMode = newColorMode;
  
  if (stripData.colorMode == COLOR)
    SetColor(rots[0].count);
  if (stripData.colorMode == COLOR_WHITE)
    SetColorRGB(255, 255, 255);
  else if (stripData.colorMode == COLOR_BLUE) {
    rots[0].count = 240;
    SetColor(rots[0].count);
  }
  else if (stripData.colorMode == COLOR_RED) {
    rots[0].count = 0;
    SetColor(rots[0].count);
  }
  else if (stripData.colorMode == RAINBOW) {
    
  }
  else if (stripData.colorMode == FADE) {
    stripData.fadeTimer = 0;
  }
  else if (stripData.colorMode == KIT) {
    stripData.dir = 1;
    stripData.targetColor = 0;
  }
  else if (stripData.targetColor == SPACE) {
    stripData.targetColor = 0;
    stripData.fadeTimer = 125;
  }
}

void RotateColorMode() {
  SetColorMode((stripData.colorMode + (stripData.colorMode + 1 == MAX_COLOR_MODE ? 2 : 1)) % MAX_COLOR_MODE);
  Serial.print("Color mode:");
  Serial.println(stripData.colorMode);
}

void UpdatePre1() {
  UpdateDials(0);
}
void UpdatePre2() {
  UpdateDials(1);
}


void SetColor(int color) {
  long tmp = (float(color) / 360.0) * 65536.0;
  //Serial.println(color);
  //Serial.println(tmp);

  for (int i = 0; i < strip.numPixels(); i++)
    strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(tmp)));

  strip.show();
}

void SetColorRGB(int r, int g, int b) {
  for (int i = 0; i < strip.numPixels(); i++)
      strip.setPixelColor(i, strip.Color(r, g, b));
  strip.show();
}

void SetBrightness(int brightness) {
  strip.setBrightness(brightness);
  //SetColor(rots[0].count);
  strip.show();
}

void SetPower(bool power) {
  if (power) {
    SetBrightness(rots[1].count);
    SetColorMode(stripData.colorMode);
  }
  else {
    SetBrightness(0);
  }
}

void UpdateDials(int i) {
  if (!digitalRead(rots[i].buttonPin) && stripData.sleepTimer == 0) {
    if (i == 0)
      RotateColorMode();
    else {
      stripData.lightsOn = !stripData.lightsOn;
      SetPower(stripData.lightsOn);
    }
      
    stripData.sleepTimer = stripData.sleepTimerStart;
    Serial.print("Dial ");
    Serial.print(i);
    Serial.println(" button pushed");
  }
  else {
    int check = digitalRead(rots[i].pin1);
    if (check != rots[i].prev && stripData.sleepTimer == 0) {
      if (digitalRead(rots[i].pin2) == check)
        rots[i].count += stripData.stepSize;
      else
        rots[i].count -= stripData.stepSize;

      if (i == 0) {
        if (rots[i].count > 360)
          rots[i].count = 0;
        else if (rots[i].count < 0)
          rots[i].count = 360;

        SetColor(rots[0].count);
        SetColorMode(COLOR);
      }
      else {
        if (rots[i].count > 255)
          rots[i].count = 255;
        else if (rots[i].count < 1)
          rots[i].count = 1;


        SetBrightness(rots[i].count);
      }


      Serial.print("Dial ");
      Serial.print(i);
      Serial.print("Position: ");
      Serial.println(rots[i].count);

      //SetColor(counter);
      rots[i].prev = rots[i].count;
    }
  }
}
