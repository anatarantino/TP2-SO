#include <time.h>
#include <stdint.h>
#include <irqDispatcher.h>
#include <keyboard_driver.h>
#include <prints.h>

void irqDispatcher(uint64_t irq, uint64_t rsp) {
	switch (irq) {
		case 0:
			int_20();
			break;
	 	case 1:
			int_21(rsp);
			break;
	}
	return;
}

void int_20() {
	timer_handler();
}

void int_21(uint64_t rsp) {
	keyboard_handler(rsp);
}


