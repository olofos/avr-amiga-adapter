#ifndef CONFIG_H_
#define CONFIG_H_

//// Pins /////////////////////////////////////////////////////////////////////

// PB0 JOY_BR
// PB1 JOY_R
// PB2 JOY_L
// PB3 JOY_D
// PB4 JOY_BL
// PB5 JOY_U / MOSI
// PB6 MISO
// PB7 SCK

// PD0 SW / RX
// PD1 JOY_BM / TX
// PD2 USB_P
// PD3 USB_M
// PD4 USB_PULLUP
// PD5 LED2
// PD6 LED1

// USB

#define PIN_USB_P          PORTD2
#define PIN_USB_P_PORT     PORTD
#define PIN_USB_P_PIN      PIND
#define PIN_USB_P_DDR      DDRD

#define PIN_USB_M          PORTD4
#define PIN_USB_M_PORT     PORTD
#define PIN_USB_M_PIN      PIND
#define PIN_USB_M_DDR      DDRD

#define PIN_USB_PULLUP      PORTD4
#define PIN_USB_PULLUP_PORT PORTD
#define PIN_USB_PULLUP_PIN  PIND
#define PIN_USB_PULLUP_DDR  DDRD


// Joystick / Mouse

// Right Button / Right Button
#define PIN_JOY_BR         PORTB0
#define PIN_JOY_BR_PORT    PORTB
#define PIN_JOY_BR_PIN     PINB
#define PIN_JOY_BR_DDR     DDRB

// Right / HQ-pulse
#define PIN_JOY_R          PORTB1
#define PIN_JOY_R_PORT     PORTB
#define PIN_JOY_R_PIN      PINB
#define PIN_JOY_R_DDR      DDRB

// Left / VQ-pulse
#define PIN_JOY_L          PORTB2
#define PIN_JOY_L_PORT     PORTB
#define PIN_JOY_L_PIN      PINB
#define PIN_JOY_L_DDR      DDRB

// Down / H-pulse
#define PIN_JOY_D          PORTB3
#define PIN_JOY_D_PORT     PORTB
#define PIN_JOY_D_PIN      PINB
#define PIN_JOY_D_DDR      DDRB

// Left Button / Left Butto
#define PIN_JOY_BL         PORTB4
#define PIN_JOY_BL_PORT    PORTB
#define PIN_JOY_BL_PIN     PINB
#define PIN_JOY_BL_DDR     DDRB

// Up / V-pulse
#define PIN_JOY_U          PORTB5
#define PIN_JOY_U_PORT     PORTB
#define PIN_JOY_U_PIN      PINB
#define PIN_JOY_U_DDR      DDRB

// Middle Button / Middle Button
#define PIN_JOY_BM         PORTD1
#define PIN_JOY_BM_PORT    PORTD
#define PIN_JOY_BM_PIN     PIND
#define PIN_JOY_BM_DDR     DDRD

// Mouse Aliases


// Right / HQ-pulse
#define PIN_MOUSE_HQ PIN_JOY_R
#define PIN_MOUSE_HQ_PIN PIN_JOY_R_PIN
// Left / VQ-pulse
#define PIN_MOUSE_VQ PIN_JOY_L
#define PIN_MOUSE_VQ_PIN PIN_JOY_L_PIN
// Down / H-pulse
#define PIN_MOUSE_H  PIN_JOY_D
#define PIN_MOUSE_H_PIN  PIN_JOY_D_PIN
// Up / V-pulse
#define PIN_MOUSE_V  PIN_JOY_U
#define PIN_MOUSE_V_PIN  PIN_JOY_U_PIN

// Left Button / Left Butto
#define PIN_MOUSE_BL PIN_JOY_BL
#define PIN_MOUSE_BL_PIN PIN_JOY_BL_PIN
// Middle Button / Middle Button
#define PIN_MOUSE_BR PIN_JOY_BM
#define PIN_MOUSE_BR_PIN PIN_JOY_BM_PIN
// Right Button / Right Button
#define PIN_MOUSE_BM PIN_JOY_BR
#define PIN_MOUSE_BM_PIN PIN_JOY_BR_PIN


// Switch

#define PIN_SW             PORTD0
#define PIN_SW_PORT        PORTD
#define PIN_SW_PIN         PIND
#define PIN_SW_DDR         DDRD

// LEDs

#define PIN_LED1           PORTD6
#define PIN_LED1_PORT      PORTD
#define PIN_LED1_PIN       PIND
#define PIN_LED1_DDR       DDRD

#define PIN_LED2           PORTD5
#define PIN_LED2_PORT      PORTD
#define PIN_LED2_PIN       PIND
#define PIN_LED2_DDR       DDRD

#endif
