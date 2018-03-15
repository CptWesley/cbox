#include "cbox.h"
#include <linux/input.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

// Controller inputs.
#define XBOX1_INPUT "/dev/input/by-path/platform-20980000.usb-usb-0:1.4:1.0-event-joystick"
#define XBOX2_INPUT "/dev/input/by-path/platform-20980000.usb-usb-0:1.5:1.0-event-joystick"
#define XBOX3_INPUT "/dev/input/by-path/platform-20980000.usb-usb-0:1.6:1.0-event-joystick"
#define XBOX4_INPUT "/dev/input/by-path/platform-20980000.usb-usb-0:1.7:1.0-event-joystick"

// Magic numbers.
#define XBOX_CONTROLLER_COUNT 4
#define ATTEMPT_SLEEP 1000000

static xbox_button_callback down_callback = NULL;
static xbox_button_callback up_callback = NULL;
static xbox_trigger_callback trigger_callback = NULL;
static xbox_controller_callback connected_callback = NULL;
static xbox_controller_callback disconnected_callback = NULL;
static pthread_mutex_t cm1, cm2, cm3, cm4;

static void lock(const int id) {
	switch (id) {
		case XBOX_CONTROLLER1:
			pthread_mutex_lock(&cm1);
			break;
		case XBOX_CONTROLLER2:
			pthread_mutex_lock(&cm2);
			break;
		case XBOX_CONTROLLER3:
			pthread_mutex_lock(&cm3);
			break;
		case XBOX_CONTROLLER4:
			pthread_mutex_lock(&cm4);
			break;
	}
}

static void unlock(const int id) {
	switch (id) {
		case XBOX_CONTROLLER1:
			pthread_mutex_unlock(&cm1);
			break;
		case XBOX_CONTROLLER2:
			pthread_mutex_unlock(&cm2);
			break;
		case XBOX_CONTROLLER3:
			pthread_mutex_unlock(&cm3);
			break;
		case XBOX_CONTROLLER4:
			pthread_mutex_unlock(&cm4);
			break;
	}
}

static char *get_stream(const int id) {
	switch (id) {
		case XBOX_CONTROLLER1:
			return XBOX1_INPUT;
		case XBOX_CONTROLLER2:
			return XBOX2_INPUT;
		case XBOX_CONTROLLER3:
			return XBOX3_INPUT;
		case XBOX_CONTROLLER4:
			return XBOX4_INPUT;
	}
	return NULL;
}

static int get_button(const int controller, const int code, const int value) {
	static int dhor_state[XBOX_CONTROLLER_COUNT]; // Horizontal last known state of the dpad.
	static int dver_state[XBOX_CONTROLLER_COUNT]; // Vertical last known state of the dpad.
	int prev_state;
	switch (code) {
		case 16:
			prev_state = dhor_state[controller];
			dhor_state[controller] = value;
			if (prev_state == 1) {
				return XBOX_BUTTON_DRIGHT;
			} else if (prev_state == -1) {
				return XBOX_BUTTON_DLEFT;
			} else if (value == 1) {
				return XBOX_BUTTON_DRIGHT;
			} else if (value == -1) {
				return XBOX_BUTTON_DLEFT;
			}
		case 17:
			prev_state = dver_state[controller];
			dver_state[controller] = value;
			if (prev_state == 1) {
				return XBOX_BUTTON_DDOWN;
			} else if (prev_state == -1) {
				return XBOX_BUTTON_DUP;
			} else if (value == 1) {
				return XBOX_BUTTON_DDOWN;
			} else if (value == -1) {
				return XBOX_BUTTON_DUP;
			}
		case 304:
			return XBOX_BUTTON_A;
		case 305:
			return XBOX_BUTTON_B;
		case 308:
			return XBOX_BUTTON_Y;
		case 307:
			return XBOX_BUTTON_X;
		case 316:
			return XBOX_BUTTON_XBOX;
		case 315:
			return XBOX_BUTTON_START;
		case 314:
			return XBOX_BUTTON_BACK;
		case 310:
			return XBOX_BUTTON_LB;
		case 311:
			return XBOX_BUTTON_RB;
		case 318:
			return XBOX_BUTTON_RSB;
		case 317:
			return XBOX_BUTTON_LSB;
	}
	return 0;
}

static void send_event(const int controller, const int type, const int code, const int value) {
	int button = get_button(controller, code, value);
	lock(controller);
	if (button) { // Buttons.
		if (value == 0 && down_callback) {
			(*down_callback)(controller, button);
		} else if (up_callback) {
			(*up_callback)(controller, button);
		}
	} else if (code == 2 || code == 5) { // Triggers.
		if (code == 2) {
			button = XBOX_BUTTON_LT;
		} else {
			button = XBOX_BUTTON_RT;
		}
		(*trigger_callback)(controller, button, value);
	}
	unlock(controller);
}

static void *handle_input(void *ptr) {
	int *iptr = (int *) ptr;
	const int id = *iptr;
	free(iptr);
	
	int fd;
	int bread;
	struct input_event event;
	while (1) {
		fd = -1;
		bread = 0;
		while (fd < 0) { fd = open(get_stream(id), O_RDONLY); usleep(ATTEMPT_SLEEP); }
		if (connected_callback) {
			lock(id);
			(*connected_callback)(id);
			unlock(id);
		}
		while (1) {
			bread = read(fd, &event, sizeof(struct input_event));
			if (bread < 0) { break; }
			send_event(id, event.type, event.code, event.value);
		}
		if (disconnected_callback) {
			lock(id);
			(*disconnected_callback)(id);
			unlock(id);
		}
	}
	
	return NULL;
}

int xbox_setup() {
	pthread_mutex_init(&cm1, NULL);
	pthread_mutex_init(&cm2, NULL);
	pthread_mutex_init(&cm3, NULL);
	pthread_mutex_init(&cm4, NULL);
	pthread_t c1_thread, c2_thread, c3_thread, c4_thread;
	
	int *c1 = malloc(sizeof(int *));
	*c1 = XBOX_CONTROLLER1;
	int *c2 = malloc(sizeof(int *));
	*c2 = XBOX_CONTROLLER2;
	int *c3 = malloc(sizeof(int *));
	*c3 = XBOX_CONTROLLER3;
	int *c4 = malloc(sizeof(int *));
	*c4 = XBOX_CONTROLLER4;
	
    if (pthread_create(&c1_thread, NULL, handle_input, c1) ||
        pthread_create(&c2_thread, NULL, handle_input, c2) ||
        pthread_create(&c3_thread, NULL, handle_input, c3) ||
        pthread_create(&c4_thread, NULL, handle_input, c4)) {
        return 0;
    }
	
	return 1;
}

void xbox_set_callback(const int type, void *callback) {
	lock(XBOX_CONTROLLER1);
	lock(XBOX_CONTROLLER2);
	lock(XBOX_CONTROLLER3);
	lock(XBOX_CONTROLLER4);
	switch (type) {
		case XBOX_EVENT_UP:
			up_callback = (xbox_button_callback) callback;
			break;
		case XBOX_EVENT_DOWN:
			down_callback = (xbox_button_callback) callback;
			break;
		case XBOX_EVENT_CONNECTED:
			connected_callback = (xbox_controller_callback) callback;
			break;
		case XBOX_EVENT_DISCONNECTED:
			disconnected_callback = (xbox_controller_callback) callback;
			break;
		case XBOX_EVENT_TRIGGER:
			trigger_callback = (xbox_trigger_callback) callback;
			break;
	}
	unlock(XBOX_CONTROLLER1);
	unlock(XBOX_CONTROLLER2);
	unlock(XBOX_CONTROLLER3);
	unlock(XBOX_CONTROLLER4);
}