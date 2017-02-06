
void setNeoColor(int red, int green, int blue){
  pixels.setPixelColor(0,pixels.Color(red, green, blue));
  pixels.show();
  //delay(delayval);
  
  //update the current color
  currentColor[0] = red;
  currentColor[1] = green;
  currentColor[2] = blue;
}
