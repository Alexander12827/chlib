/******************************************************************************
 MIT License

Copyright (c) 2019 donBuba

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*******************************************************************************/


extern "C" {
#include "user_interface.h"
#include "channel3.h"
}


void setup() {
    // Disable WiFi
    wifi_set_opmode(NULL_MODE);
    initBroadcast(60);
    configureBroadcast();
    setECLInterface(BW);

    submitFooter(White, "HELLO EVERYBODY");
    colorBody(Skyblue);
}
int number = 0;
char str[64];
void loop() {
  runBroadcast();
  ets_sprintf(str, "VIDEO TIME %d:03", number);
  submitTitle(White, str);
  pushToBroadcast("HELLO WORLD");
  delay(1000);
  freezeBroadcast();
  delay(1000);
  if(number >= 12){
    number = 0;
  }else{
    number++;
  }
}
