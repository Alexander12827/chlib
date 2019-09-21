#include "mem.h"
#include "c_types.h"
#include "user_interface.h"
#include "ets_sys.h"
#include "osapi.h"
#include "ntsc_broadcast.h"
#include "3d.h"

#define MAXBUFFER (18 + 1)
#define MAXQUEUE 11

#define TITLE 0
#define BODY 1
#define FOOTER 3

void ICACHE_FLASH_ATTR configureBroadcast();
void ICACHE_FLASH_ATTR initBroadcast(int);
void ICACHE_FLASH_ATTR freezeBroadcast();
void ICACHE_FLASH_ATTR runBroadcast();

enum Icolor{GR = 0, BW};
enum Colors{Skyblue=5, Purple, Red, RGB, Green, White};

void setCustomInterface(void (*cb)()); //TODO: define function

void ECLInterface(void);
void setECLInterface(enum Icolor);

void submitFooter(enum Colors, char[MAXBUFFER]);
void submitTitle(enum Colors, char[MAXBUFFER]);

void colorBody(enum Colors);

int  pushToBroadcast(char *); //TODO: define queue and function
