#include <msp430.h>
#include <stddef.h>
#include <event.h>
#include <misc.h>
#include <intern_temp_sensor.h>
#include <stdlib.h>
#include <sx1276_driver.h>
#include <spi.h>
#include <config.h>
#include <timers.h>
#include <stdio.h>

#define OOK_TX_POWER	10u


#define GPIO_PORT_P3                                                          3
#define GPIO_PIN4                                                      (0x0010)
/*
 * Initialize the clock
 *
 * DCO set at 1 MHz
 *
 * MCLK and SMCLK set at 8 MHz and sources from DCO without div /1, and thus run at 1 MHz
 * ACLK is sources from LFXT (32 kHz) with div / 1
 */
static void init_clock(void)
{
	// Set the DCO at 1 MHz
	CSCTL0 = CSKEY;
	CSCTL1 = DCOFSEL_0;
	CSCTL2 = SELM__DCOCLK | SELS__DCOCLK | SELA__LFXTCLK;
	CSCTL3 = DIVM__1 | DIVS__1 | DIVA__1;
}

// ----------------------------------------------------------------------------------
// For handling the LoRa transceiver
// ----------------------------------------------------------------------------------
void radio_timeout_handler(void)
{
	// DO NOTHING
}

void radio_crc_error_handler(void)
{
	// DO NOTHING
}

void rx_packet_handler(void)
{
	// DO NOTHING
}

void local_packet_handler(void){

}

static void radio_init(void)
{
	// Initializing the SX1276 radio
	init_spi();
	sx1276_init(local_packet_handler, rx_packet_handler, radio_timeout_handler, radio_crc_error_handler);
}

static void set_ook_mode(unsigned int tx_power)
{
	sx1276_sleep();

    // OOK MODE
    // Transmission power = 10 dBm
    // RX Bandwidth = 50 kHz
    // Bitrate = 1 kbps
    // AFC Bandwisth = 83.3 kHz
    // No preamble
    // No sync word
    // Length fixed to 1 byte
    // CRC disabled
    // Timeout: ~5ms
    sx1276_set_tx_config(MODEM_LORA, tx_power, 0, 0, 1000, 0, 0, true, false);
    sx1276_set_rx_config(MODEM_LORA, 0, 1000, 0, 83333, 0, 21u, true, 1, false);
    sx1276_disable_sync_word();
}

void send_wub(void)
{
	uint8_t wub[] = {0x6B, 0x55};
	set_ook_mode(OOK_TX_POWER);
	sx1276_tx_pkt((char*)wub, 2u, 0u);	// Don't care about the third parameter when sending using OOK

	// FIXME
    sx1276_set_tx_config(MODEM_LORA, OOK_TX_POWER, 0, 0, 1000, 0, 0, true, false);
    sx1276_set_rx_config(MODEM_LORA, 0, 1000, 0, 83333, 0, 21u, true, 1, false);
}

void blinking(void)
{
    printf("hello!!!\n");
    fflush(stdout);
//    SHT21ReadTemperature();
//    GPIO_setOutputLowOnPin(GPIO_PORT_P3, GPIO_PIN4);
//    TB0CCTL0 = CCIE;
//    TB0CCR0 = 1000;
//    TB0CTL = TBSSEL__ACLK | MC__UP;
//    __bis_SR_register(LPM3_bits);

//    SHT21ReadHumidity();
//    GPIO_setOutputHighOnPin(GPIO_PORT_P3, GPIO_PIN4);
//    TB0CCTL0 = CCIE;
//    TB0CCR0 = 1000;
//    TB0CTL = TBSSEL__ACLK | MC__UP;
//    __bis_SR_register(LPM3_bits);
}


// -----------------------------------------------------------------------------
// Main
// -----------------------------------------------------------------------------
void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;

    init_clock();
    event_init();
    timers_init();
    srand(NODE_ADDRESS);

    //while(1);

    timer_set_periodic_event(32768u, send_wub);
//    timer_set_periodic_event(8u, send_wub);
    timer_set_periodic_event(22768u, blinking);

    radio_init();

    _EINT();

    // Starting the event loop
    event_loop();
}
