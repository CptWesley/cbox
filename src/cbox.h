#ifndef CBOX_H
#define CBOX_H

// Controllers
/*
static const int XBOX_CONTROLLER1 = 0;
static const int XBOX_CONTROLLER2 = 1;
static const int XBOX_CONTROLLER3 = 2;
static const int XBOX_CONTROLLER4 = 3;
*/
#define XBOX_CONTROLLER1 0
#define XBOX_CONTROLLER2 1
#define XBOX_CONTROLLER3 2
#define XBOX_CONTROLLER4 3

// Callback types.
/*
static const int XBOX_EVENT_UP = 1;
static const int XBOX_EVENT_DOWN = 2;
static const int XBOX_EVENT_CONNECTED = 3;
static const int XBOX_EVENT_DISCONNECTED = 4;
static const int XBOX_EVENT_TRIGGER = 5;
*/
#define XBOX_EVENT_UP 1
#define XBOX_EVENT_DOWN 2
#define XBOX_EVENT_CONNECTED 3
#define XBOX_EVENT_DISCONNECTED 4
#define XBOX_EVENT_TRIGGER 5

// Buttons.
/*
static const int XBOX_BUTTON_A = 1;
static const int XBOX_BUTTON_B = 2;
static const int XBOX_BUTTON_Y = 3;
static const int XBOX_BUTTON_X = 4;
static const int XBOX_BUTTON_BACK = 5;
static const int XBOX_BUTTON_START = 6;
static const int XBOX_BUTTON_XBOX = 7;
static const int XBOX_BUTTON_LB = 8;
static const int XBOX_BUTTON_RB = 9;
static const int XBOX_BUTTON_LT = 10;
static const int XBOX_BUTTON_RT = 11;
static const int XBOX_BUTTON_DUP = 12;
static const int XBOX_BUTTON_DDOWN = 13;
static const int XBOX_BUTTON_DLEFT = 14;
static const int XBOX_BUTTON_DRIGHT = 15;
static const int XBOX_BUTTON_LSB = 16;
static const int XBOX_BUTTON_RSB = 17;
static const int XBOX_BUTTON_LS = 18;
static const int XBOX_BUTTON_RS = 19;
*/
#define XBOX_BUTTON_A 1
#define XBOX_BUTTON_B 2
#define XBOX_BUTTON_Y 3
#define XBOX_BUTTON_X 4
#define XBOX_BUTTON_BACK 5
#define XBOX_BUTTON_START 6
#define XBOX_BUTTON_XBOX 7
#define XBOX_BUTTON_LB 8
#define XBOX_BUTTON_RB 9
#define XBOX_BUTTON_LT 10
#define XBOX_BUTTON_RT 11
#define XBOX_BUTTON_DUP 12
#define XBOX_BUTTON_DDOWN 13
#define XBOX_BUTTON_DLEFT 14
#define XBOX_BUTTON_DRIGHT 15
#define XBOX_BUTTON_LSB 16
#define XBOX_BUTTON_RSB 17
#define XBOX_BUTTON_LS 18
#define XBOX_BUTTON_RS 19

// Callback types.
typedef void (*xbox_button_callback)(const int, const int);
typedef void (*xbox_trigger_callback)(const int, const int, const int);
typedef void (*xbox_controller_callback)(const int);

// Functions.
int xbox_setup();
void xbox_set_callback(const int, void *);

#endif