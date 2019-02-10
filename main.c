#define ADAPTER_MOUSE 1

#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/wdt.h>
#include <string.h>

#include "io.h"
#include "config.h"

#include "usbdrv/usbdrv.h"
#include "hid_def.h"

static void init_pins()
{
    set_output(PIN_LED1);
    set_output(PIN_LED2);

    set_input(PIN_JOY_L);
    set_input(PIN_JOY_R);
    set_input(PIN_JOY_U);
    set_input(PIN_JOY_D);

    set_input(PIN_JOY_BL);
    set_input(PIN_JOY_BM);
    set_input(PIN_JOY_BR);

    set_input(PIN_SW);

    enable_pullup(PIN_JOY_L);
    enable_pullup(PIN_JOY_R);
    enable_pullup(PIN_JOY_U);
    enable_pullup(PIN_JOY_D);

    enable_pullup(PIN_JOY_BL);
    enable_pullup(PIN_JOY_BM);
    enable_pullup(PIN_JOY_BR);

    enable_pullup(PIN_SW);    

    set_low(PIN_LED1);
    set_low(PIN_LED2);
}


#ifdef ADAPTER_MOUSE

PROGMEM const char usbHidReportDescriptor[USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH] = {
    /**/HID_USAGE_PAGE (GENERIC_DESKTOP),          // 2
    /**/HID_USAGE (MOUSE),                         // 2
    /**/HID_COLLECTION (APPLICATION),              // 2
    /**/  HID_USAGE (POINTER),                     // 2
    /**/  HID_COLLECTION (PHYSICAL),               // 2
    /**/    HID_USAGE (X),                         // 2
    /**/    HID_USAGE (Y),                         // 2
    /**/    HID_LOGICAL_MINIMUM (1,-127),          // 2
    /**/    HID_LOGICAL_MAXIMUM (1,+127),          // 2
    /**/    HID_REPORT_SIZE (8),                   // 2
    /**/    HID_REPORT_COUNT (2),                  // 2
    /**/    HID_INPUT (DATA, VARIABLE, RELATIVE),  // 2
    /**/    HID_USAGE_PAGE(BUTTONS),               // 2
    /**/    HID_USAGE_MINIMUM (1,1),               // 2
    /**/    HID_USAGE_MAXIMUM (1,3),               // 2
    /**/    HID_LOGICAL_MINIMUM (1,0),             // 2
    /**/    HID_LOGICAL_MAXIMUM (1,1),             // 2
    /**/    HID_REPORT_SIZE (1),                   // 2
    /**/    HID_REPORT_COUNT (3),                  // 2
    /**/    HID_INPUT (DATA, VARIABLE, ABSOLUTE),  // 2
    /**/    HID_REPORT_COUNT (1),                  // 2
    /**/    HID_REPORT_SIZE (5),                   // 2
    /**/    HID_INPUT (CONSTANT),                  // 2
    /**/  HID_END_COLLECTION (PHYSICAL),           // 1
    /**/HID_END_COLLECTION (APPLICATION),          // 1

    /**/                                           // 48
};

#else

PROGMEM const char usbHidReportDescriptor[USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH] = {
    /**/HID_USAGE_PAGE (GENERIC_DESKTOP),          // 2
    /**/HID_USAGE (GAME_PAD),                      // 2
    /**/HID_COLLECTION (APPLICATION),              // 2
    /**/  HID_USAGE (POINTER),                     // 2
    /**/  HID_COLLECTION (PHYSICAL),               // 2
    /**/    HID_USAGE (X),                         // 2
    /**/    HID_USAGE (Y),                         // 2
    /**/    HID_LOGICAL_MINIMUM (1,-127),          // 2
    /**/    HID_LOGICAL_MAXIMUM (1,+127),          // 2
    /**/    HID_REPORT_SIZE (8),                   // 2
    /**/    HID_REPORT_COUNT (2),                  // 2
    /**/    HID_INPUT (DATA, VARIABLE, ABSOLUTE),  // 2
    /**/    HID_USAGE_PAGE(BUTTONS),               // 2
    /**/    HID_USAGE_MINIMUM (1,1),               // 2
    /**/    HID_USAGE_MAXIMUM (1,3),               // 2
    /**/    HID_LOGICAL_MINIMUM (1,0),             // 2
    /**/    HID_LOGICAL_MAXIMUM (1,1),             // 2
    /**/    HID_REPORT_SIZE (1),                   // 2
    /**/    HID_REPORT_COUNT (3),                  // 2
    /**/    HID_INPUT (DATA, VARIABLE, ABSOLUTE),  // 2
    /**/    HID_REPORT_COUNT (1),                  // 2
    /**/    HID_REPORT_SIZE (5),                   // 2
    /**/    HID_INPUT (CONSTANT),                  // 2
    /**/  HID_END_COLLECTION (PHYSICAL),           // 1
    /**/HID_END_COLLECTION (APPLICATION),          // 1

    /**/                                           // 48
};

#endif






uint8_t report[3];
uint8_t report_out[3];


uint8_t usbFunctionSetup( uint8_t data [8] )
{
	usbRequest_t const* rq = (usbRequest_t const*) data;

	if ( (rq->bmRequestType & USBRQ_TYPE_MASK) != USBRQ_TYPE_CLASS )
		return 0;
	
	switch ( rq->bRequest )
	{
	case USBRQ_HID_GET_REPORT: // HID joystick only has to handle this
		usbMsgPtr = (usbMsgPtr_t) report_out;
		return sizeof(report_out);
	
	//case USBRQ_HID_SET_REPORT: // LEDs on joystick?
	
	default:
		return 0;
	}
}

#ifdef ADAPTER_MOUSE

int8_t dx, dy;

static void read_mouse()
{
    static int8_t enc_states[] = {0,-1,1,0,1,0,0,-1,-1,0,0,1,0,1,-1,0};
    static uint8_t old_AB_H, old_AB_V;

    uint8_t AB_H = (is_high(PIN_MOUSE_HQ) >> PIN_MOUSE_HQ) | (is_high(PIN_MOUSE_H) >> (PIN_MOUSE_H - 1));

    old_AB_H <<= 2;
    old_AB_H |= AB_H;

    dx += 2*enc_states[old_AB_H & 0x0f];

    uint8_t AB_V = (is_high(PIN_MOUSE_VQ) >> PIN_MOUSE_VQ) | (is_high(PIN_MOUSE_V) >> (PIN_MOUSE_V - 1));

    old_AB_V <<= 2;
    old_AB_V |= AB_V;

    dy += 2*enc_states[old_AB_V & 0x0f];
}

static void fill_report_mouse()
{
    report[0] = dx;
    report[1] = dy;
    
    dx = 0;
    dy = 0;

    report[2] = 0;
                       
    if(is_low(PIN_MOUSE_BL)) report[2] |= 0x01;
    if(is_low(PIN_MOUSE_BM)) report[2] |= 0x02;
    if(is_low(PIN_MOUSE_BR)) report[2] |= 0x04;

}

#else

static void fill_report_joystick()
{
    if(is_low(PIN_JOY_L))
    {
        report[0] = -127;
    } else if(is_low(PIN_JOY_R)) {
        report[0] = +127;
    } else {
        report[0] = 0;
    }
            
    if(is_low(PIN_JOY_U))
    {
        report[1] = -127;
    } else if(is_low(PIN_JOY_D)) {
        report[1] = +127;
    } else {
        report[1] = 0;
    }

    report[2] = 0;
                       
    if(is_low(PIN_JOY_BL)) report[2] |= 0x01;
    if(is_low(PIN_JOY_BM)) report[2] |= 0x02;
    if(is_low(PIN_JOY_BR)) report[2] |= 0x04;
}

#endif

int main (void)
{
    wdt_disable();

    init_pins();

    wdt_enable(WDTO_1S);

    usbInit();

    usbDeviceDisconnect(); // enforce re-enumeration
    for(uint8_t i = 0; i < 250; i++) { // wait 500 ms
        wdt_reset(); // keep the watchdog happy
        _delay_ms(2);
    }
    usbDeviceConnect();
        
    sei(); // Enable interrupts after re-enumeration
        
    for(;;) {
        wdt_reset(); // keep the watchdog happy

#ifdef ADAPTER_MOUSE

        read_mouse();
        
        usbPoll();

        if ( usbInterruptIsReady() )
        {
            fill_report_mouse();
            usbSetInterrupt(report, sizeof(report));
        }

#else
        
        
        if ( usbInterruptIsReady() )
        {
            fill_report_joystick();
            
            if(memcmp(report, report_out, sizeof(report)))
            {
                memcpy(report_out, report, sizeof(report));
                usbSetInterrupt(report_out, sizeof(report_out));

                toggle(PIN_LED1);
            }
        }
#endif

//        set_pin(PIN_LED1, is_low(PIN_JOY_U));
//        set_pin(PIN_LED2, is_low(PIN_JOY_D));

    }
}
