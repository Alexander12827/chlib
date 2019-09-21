/******************************************************************************
 MIT License

Copyright (c) 2019 donBuba
 	            2015 Charles Lohr

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


#include "channel3.h"

static uint8_t rflag = 0;
static int period = 0;
static os_timer_t runTimer;
void ICACHE_FLASH_ATTR myTimer();

int seeker = -1;

void (*cb)();
struct broadcastData{
enum Icolor icolor;
uint8_t textColor[3];
char bodyMsg[MAXBUFFER * MAXQUEUE];
char titleMsg[MAXBUFFER];
char footerMsg[MAXBUFFER];
}broadcastCtx;

char *myStrcat (char *dest, const char *src)
{
     ets_strcpy (dest + ets_strlen (dest), src);
     return dest;
}
 
void push(char *str){
char q[MAXBUFFER]; 
if(ets_strlen(str) < MAXBUFFER-1){
ets_strcpy(q, str);
myStrcat(q, "\n");
seeker++;
if(seeker < MAXQUEUE){
    ets_strcpy(broadcastCtx.bodyMsg, myStrcat(broadcastCtx.bodyMsg, q));
}else{
    seeker = 0;
    ets_strcpy(broadcastCtx.bodyMsg, "\n");
    ets_strcpy(broadcastCtx.bodyMsg, myStrcat(broadcastCtx.bodyMsg, q));
}
}
}
  
void ECLInterface(){
    uint8_t cBorder;
    uint8_t cCentral;
    switch((int)broadcastCtx.icolor){
        case GR:
        cBorder = 4;
        cCentral = 7;
        break;
        case BW:
        cBorder = 5;
        cCentral = 10;
        break;
    }
  int * px = &CNFGPenX;
  int * py = &CNFGPenY;
   CNFGColor( cBorder );
   CNFGTackRectangle( 0, 0, (58*4)-1, (55/2)-1);
   CNFGColor( cCentral );
   CNFGTackRectangle( 0, 55/2, (58*4)-1, (55*2)-1);
   CNFGTackRectangle( 0, 55*2, (58*4)-1, (55*3 + 55/4)-1);
   CNFGColor( cBorder );
   CNFGTackRectangle( 0, 55*3 + 55/4, (58*4)-1, (55*4)-1);

   *px = 3;
   *py = 4;
   CNFGColor( broadcastCtx.textColor[TITLE] );
   CNFGDrawText(broadcastCtx.titleMsg, 2 );
    
   *py = 190;
   CNFGColor( broadcastCtx.textColor[FOOTER] );
   CNFGDrawText(broadcastCtx.footerMsg, 2 );

   *px = 10;
   *py = 28;
   CNFGColor( broadcastCtx.textColor[BODY] );
   CNFGDrawText( broadcastCtx.bodyMsg, 2 );
}
void setECLInterface(enum Icolor color){
    broadcastCtx.icolor = color;
    cb = ECLInterface;
}
void submitTitle(enum Colors color, char str[MAXBUFFER]){
    broadcastCtx.textColor[TITLE] = color;
    ets_sprintf(broadcastCtx.titleMsg, " %s", str);
}
void submitFooter(enum Colors color, char str[MAXBUFFER]){
    broadcastCtx.textColor[FOOTER] = color;
    ets_strcpy(broadcastCtx.footerMsg, str);
}
void colorBody(enum Colors color){
	broadcastCtx.textColor[BODY] = color;
}
int  pushToBroadcast(char * str){
  push(str);
}

void ICACHE_FLASH_ATTR loadFrame(uint8_t * ff)
{
   ets_memset( ff, 0, ((232/4)*220) );
   if(cb != NULL){
    cb(); //callback
   }
}

void ICACHE_FLASH_ATTR configureBroadcast(){
  testi2s_init();
  //it is need because a good task functions
  system_update_cpu_freq( SYS_CPU_160MHZ );
}

void ICACHE_FLASH_ATTR freezeBroadcast(){
  if(rflag){
      os_timer_disarm(&runTimer);
      rflag = 0;
  }
}
void ICACHE_FLASH_ATTR runBroadcast(){
  if(!rflag){
    os_timer_setfn(&runTimer, (os_timer_func_t *)myTimer, NULL);
    os_timer_arm(&runTimer, period, 1);
    rflag = 1;
  }
}

void ICACHE_FLASH_ATTR initBroadcast(int freq){
  if(freq > 60 || freq <= 0){
    freq = 60;
  }
  if(!rflag){
  period = 1000 / freq;
  os_timer_setfn(&runTimer, (os_timer_func_t *)myTimer, NULL);
  os_timer_arm(&runTimer, period, 1);
  rflag = 1;
  }
}

void ICACHE_FLASH_ATTR myTimer(){
	static uint8_t lastframe = 0;
	uint8_t tbuffer = !(gframe&1);
	if( lastframe != tbuffer )
	{
		frontframe = (uint8_t*)&framebuffer[((FBW2/4)*FBH)*tbuffer];
		loadFrame(frontframe);
		lastframe = tbuffer;
	}
}
