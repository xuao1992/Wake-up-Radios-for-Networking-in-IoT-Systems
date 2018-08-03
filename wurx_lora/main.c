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
#define LORA_TX_POWER   10u

#define LORA_BANDWIDTH_VALUE               0
#define LORA_DATARATE_VALUE                1000
#define LORA_CODERATE_VALUE                1
#define LORA_BANDWIDTHAFC_VALUE            83333
#define LORA_PREAMBLELEN_VALUE             0
#define RX_TIMEOUT_VALUE                   21u
#define LORA_FIXLEN_VALUE                  true
#define LORA_PAYLOADLEN_VALUE              1
#define LORA_CRC_ON                        false

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
    printf("radio_timeout_handler!!!\n");
    led2_off();
    // DO NOTHING
}

void radio_crc_error_handler(void)
{
    printf("radio_crc_error_handler!!!\n");
    led2_off();
    // DO NOTHING
}

void rx_packet_handler(void)
{
    printf("rx_packet_handler!!!\n");
    led2_off();
    led1_fast_double_blink();
    // DO NOTHING
}

void local_packet_handler(void)
{
    printf("local_packet_handler!!!\n");
    led2_off();

}

void sw1_handler(void)
{
    sx1276_reset();
    printf("sw1_handler!!!\n");
    //DO NOTHING
}

static void radio_init(void)
{
    // Initializing the SX1276 radio
    init_spi();
    sx1276_init(local_packet_handler, rx_packet_handler, radio_timeout_handler,
                radio_crc_error_handler);

    printf("sx1276 initialled!!\n");
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
    sx1276_set_rx_config(MODEM_LORA, 0, 1000, 0, 83333, 0, 0, true, 1, false);
    sx1276_disable_sync_word();

    printf("ook mode set up!!! %d\n", tx_power);
}

void send_wub(void)
{
    uint8_t wub[] = { 0x6B, 0x55 };
    set_ook_mode(OOK_TX_POWER);
    sx1276_tx_pkt((char*) wub, 2u, 0u);	// Don't care about the third parameter when sending using OOK
    led1_fast_double_blink();
    printf("sent_wub!!\n");

    // FIXME
    sx1276_set_tx_config(MODEM_LORA, OOK_TX_POWER, 0, 0, 1000, 0, 0, true,
    false);
    sx1276_set_rx_config(MODEM_LORA, 0, 1000, 0, 83333, 0, 21u, true, 1, false);

}

void receive_wub(void)
{
    set_ook_mode(OOK_TX_POWER);
    sx1276_rx_single_pkt();
    printf("receive_wub!!\n");
    led2_on();

    // FIXME
    sx1276_set_tx_config(MODEM_LORA, OOK_TX_POWER, 0, 0, 1000, 0, 0, true,
    false);
    sx1276_set_rx_config(MODEM_LORA, 0, 1000, 0, 83333, 0, 0, true, 1, false);
}

static void set_lora_mode(unsigned int tx_power)
{
    sx1276_set_tx_config(MODEM_LORA, tx_power, 0, LORA_BANDWIDTH_VALUE,
                         LORA_DATARATE_VALUE, LORA_CODERATE_VALUE,
                         LORA_PREAMBLELEN_VALUE, LORA_FIXLEN_VALUE,
                         LORA_CRC_ON);


    printf("lora tx config set up!!! %d\n", tx_power);

    sx1276_set_rx_config(MODEM_LORA, LORA_BANDWIDTH_VALUE, LORA_DATARATE_VALUE,
                         LORA_CODERATE_VALUE, LORA_BANDWIDTHAFC_VALUE,
                         LORA_PREAMBLELEN_VALUE, RX_TIMEOUT_VALUE,
                         LORA_FIXLEN_VALUE, LORA_PAYLOADLEN_VALUE, LORA_CRC_ON);

    printf("lora rx config set up!!! \n");
}

void send_lora(void)
{
    uint8_t data[] = { 0x2B, 0x3B };
    set_lora_mode(LORA_TX_POWER);
    sx1276_tx_pkt((char*) data, 2u, DEST_ADDRESS); // Don't care about the third parameter when sending using OOK
    led1_fast_double_blink();
    printf("send %X to %X!!\n", data, DEST_ADDRESS);

}

void receive_lora(void)
{
    set_lora_mode(LORA_TX_POWER);
    sx1276_rx_single_pkt();
    printf("receive_lora!!\n");
    led2_on();

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

    leds_init();
    sw1_init(sw1_handler);

    timer_set_periodic_event(32768u, send_lora);
//    timer_set_periodic_event(32768u, receive_lora);
    printf("event set up!!!\n");

    radio_init();

    _EINT();

    // Starting the event loop
    event_loop();
}
