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
#include <string.h>

#define OOK_TX_POWER	10u
uint8_t LORA_TX_POWER = 2u;

uint32_t LORA_BANDWIDTH_VALUE = 2;   //0-2
uint32_t LORA_DATARATE_VALUE = 7;  //6-12    SpreadingFactor
uint8_t LORA_CODERATE_VALUE = 1;   //1-4  all other values: reserved
#define LORA_BANDWIDTHAFC_VALUE            83333
#define LORA_PREAMBLELEN_VALUE             6u
#define RX_TIMEOUT_VALUE                   1000000u  //21
#define LORA_FIXLEN_VALUE                  false  //implicit header
#define LORA_PAYLOADLEN_VALUE              1
#define LORA_CRC_ON                        true


int WAITING_RESPONSE=0;
const uint8_t REQ = 0xC1;
const uint8_t ACK = 0xC2;
const uint8_t REDUCE_POWER = 0xC3;

uint8_t data[] = { REQ };

int STATUS_INIT = 1;

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

    if(!WAITING_RESPONSE){
        return;
    }

    if (LORA_TX_POWER < 12)
    {
        LORA_TX_POWER++;
    }
    else
    {
        led1_on();
        led2_on();
//        STATUS_INIT = 0;
    }

}

void radio_crc_error_handler(void)
{
    printf("radio_crc_error_handler!!!\n");
    led2_off();
    // DO NOTHING
}

void rx_packet_handler(void)
{
    if (spi_rcv_data(0x1A) - 157 < -50)  //for testing, suppose message do not be received if RSSI < -50
    {
        return;
    }
//    printf("rx_packet_handler!!!\n");
    led2_off();
    printf("receive data: %s \n", sx1276_rx_fifo[sx1276_rx_fifo_first].data);

    switch (sx1276_rx_fifo[sx1276_rx_fifo_first].data[0])
    {
    case REQ:  //for receiver

        printf("receive REQ!!! \n");

        WAITING_RESPONSE = 0;
        if(spi_rcv_data(0x1A) - 157 > -20){
            data[0] = REDUCE_POWER;
            printf("sent REDUCE_POWER!\n");
        } else {
            data[0] = ACK;
            printf("sent ACK!\n");
        }
        __delay_cycles(1000000u);
        sx1276_tx_pkt((char *)data, 1u, DEST_ADDRESS);
        led1_fast_double_blink();
        break;
    case ACK:  //for sender
        printf("receive ACK!!! \n");
//        STATUS_INIT = 0;
        break;
    case REDUCE_POWER:  //for sender
        printf("receive REDUCE_POWER!!! \n");
        if (LORA_TX_POWER > 2)
        {
            LORA_TX_POWER--;
        } else {
//            STATUS_INIT = 0;
        }
        break;
    }

//    printf("SENDER_TXPOWER = %d\n", sx1276_rx_fifo[sx1276_rx_fifo_first].data[0]);
//    printf("SENDER_BANDWIDTH = %d\n", sx1276_rx_fifo[sx1276_rx_fifo_first].data[1]);
//    printf("SENDER_DATARATE = %d\n", sx1276_rx_fifo[sx1276_rx_fifo_first].data[2]);
//    printf("SENDER_CODERATE = %d\n", sx1276_rx_fifo[sx1276_rx_fifo_first].data[3]);
//    printf("=======================================\n");
//    printf("RECEIVER_BANDWIDTH = %d\n", LORA_BANDWIDTH_VALUE);
//    printf("RECEIVER_DATARATE = %d\n", LORA_DATARATE_VALUE);
//    printf("RECEIVER_CODERATE = %d\n", LORA_CODERATE_VALUE);
//    printf("=======================================\n");

//    if (LORA_DATARATE_VALUE < 12)
//    {
//        if (LORA_BANDWIDTH_VALUE < 2)
//        {
//            if (LORA_CODERATE_VALUE < 4)
//            {
//                LORA_CODERATE_VALUE++;
//                return;
//            }
//            else
//            {
//                LORA_CODERATE_VALUE = 1;
//            }
//            LORA_BANDWIDTH_VALUE++;
//            return;
//        }
//        else
//        {
//            LORA_BANDWIDTH_VALUE = 0;
//        }
//        LORA_DATARATE_VALUE++;
//        return;
//    }
//    else
//    {
//        LORA_DATARATE_VALUE = 7;
//    }


        /*
    if(!strcmp(sx1276_rx_fifo[sx1276_rx_fifo_first].data, "ack!")){  //sender
        timer_set_periodic_event(32768u, sx1276_sleep);
        printf("received ack!\n");
        led1_on();
    }
    else if(!strcmp(sx1276_rx_fifo[sx1276_rx_fifo_first].data, "req")) //receiver
    {
        set_lora_mode(LORA_TX_POWER);
        __delay_cycles(1000000u);
        sx1276_tx_pkt("ack!", 4, DEST_ADDRESS);
        led1_fast_double_blink();
        printf("sent ack!\n");
    }
    else if(!strcmp(sx1276_rx_fifo[sx1276_rx_fifo_first].data, "reqdr")) //receiver
    {
        set_lora_mode(LORA_TX_POWER);
        __delay_cycles(1000000u);
        sx1276_tx_pkt("syndr7", 6, DEST_ADDRESS);
        LORA_DATARATE_VALUE = 7;
        led1_fast_double_blink();
        printf("sent syndr7!\n");
    }
    else if (!strncmp(sx1276_rx_fifo[sx1276_rx_fifo_first].data, "syndr", 5)) // sender
    {
        LORA_DATARATE_VALUE = sx1276_rx_fifo[sx1276_rx_fifo_first].data[5] - '0';
        printf("modified LORA_DATARATE_VALUE = %d\n", LORA_DATARATE_VALUE);
        set_lora_mode(LORA_TX_POWER);
        __delay_cycles(1000000u);
        sx1276_tx_pkt("req", 4, DEST_ADDRESS);
        led1_fast_double_blink();
        printf("sent req!\n");
        sx1276_rx_single_pkt();
        led2_on();
    }

    */
}

void local_packet_handler(void)
{
    printf("local_packet_handler!!!\n");
    led2_off();

}

void sw1_handler(void)
{
    sx1276_reset();
    STATUS_INIT = 1;
    printf("sw1_handler!!!\n");
    //DO NOTHING
}

static void radio_init(void)
{
    // Initializing the SX1276 radio
    init_spi();
    sx1276_init(local_packet_handler, rx_packet_handler, radio_timeout_handler,
                radio_crc_error_handler);

//    printf("sx1276 initialled!!\n");
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
    sx1276_sleep(); // sleep is nessary for MODEM changing

    sx1276_set_tx_config(MODEM_LORA, tx_power, 0, LORA_BANDWIDTH_VALUE,
                         LORA_DATARATE_VALUE, LORA_CODERATE_VALUE,
                         LORA_PREAMBLELEN_VALUE, LORA_FIXLEN_VALUE,
                         LORA_CRC_ON);


//    printf("lora tx config set up!!! %d\n", tx_power);

    sx1276_set_rx_config(MODEM_LORA, LORA_BANDWIDTH_VALUE, LORA_DATARATE_VALUE,
                         LORA_CODERATE_VALUE, LORA_BANDWIDTHAFC_VALUE,
                         LORA_PREAMBLELEN_VALUE, RX_TIMEOUT_VALUE,
                         LORA_FIXLEN_VALUE, LORA_PAYLOADLEN_VALUE, LORA_CRC_ON);

//    printf("lora rx config set up!!! \n");

    sx1276_disable_sync_word();
}

void send_lora(void)
{
//    char data[] = {LORA_TX_POWER, LORA_BANDWIDTH_VALUE, LORA_DATARATE_VALUE, LORA_CODERATE_VALUE};
    char data[] = "Hello World!";

    set_lora_mode(LORA_TX_POWER);
//    sx1276_tx_pkt((char*) data, 4u, DEST_ADDRESS);
    sx1276_tx_pkt((char*) data, strlen(data), DEST_ADDRESS);
    led1_fast_double_blink();
    printf("send \"%s\" to %X!!\n", data, DEST_ADDRESS);

//    if (LORA_DATARATE_VALUE < 9)
//    {
//        if (LORA_BANDWIDTH_VALUE < 2)
//        {
//            if (LORA_CODERATE_VALUE < 4)
//            {
//                LORA_CODERATE_VALUE++;
//                return;
//            }
//            else
//            {
//                LORA_CODERATE_VALUE = 1;
//            }
//            LORA_BANDWIDTH_VALUE++;
//            return;
//        }
//        else
//        {
//            LORA_BANDWIDTH_VALUE = 0;
//        }
//        LORA_DATARATE_VALUE++;
//        return;
//    }
//    else
//    {
//        LORA_DATARATE_VALUE = 7;
//    }
}

void receive_lora(void)
{
    set_lora_mode(LORA_TX_POWER);
    sx1276_rx_single_pkt();
    printf("receiving_lora!!\n");
    led2_on();
}

void init_power()
{
    set_lora_mode(LORA_TX_POWER);
    printf("tx_power = %d\n", LORA_TX_POWER);
    led1_fast_double_blink();
    if (STATUS_INIT)
    {
        sx1276_tx_pkt((char*) data, sizeof(data), DEST_ADDRESS);
        WAITING_RESPONSE = 1;
        sx1276_rx_single_pkt();
        led2_on();
    }
    else
    {
        WAITING_RESPONSE = 0;
        sx1276_tx_pkt((char*) "Hello World!!", 13u, DEST_ADDRESS);
    }

}

void init_para(void)
{
    led2_off();
    set_lora_mode(LORA_TX_POWER);
    char Req_data[] = "reqdr";
//    if(LORA_DATARATE_VALUE == 12){
//        strcpy(Req_data, "req");
//    } else {
//        strcpy(Req_data, "irq");
//    }
    printf("sent %s + DR = %d!\n", Req_data, LORA_DATARATE_VALUE);
    sx1276_tx_pkt((char*)Req_data, strlen(Req_data), DEST_ADDRESS);
    led1_fast_double_blink();
    sx1276_rx_single_pkt();
    led2_on();
    if (LORA_DATARATE_VALUE == 7)
    {
        strcpy(Req_data, "req");
    }

    else if (LORA_DATARATE_VALUE < 12)
    {
        LORA_DATARATE_VALUE++;
    }
    else
    {
        LORA_DATARATE_VALUE = 8;
    }
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

//    timer_set_periodic_event(32768u, send_wub);
//    timer_set_periodic_event(32768u, receive_lora);
    timer_set_periodic_event(32768u, init_power);
//
    radio_init();

    _EINT();

    // Starting the event loop
    event_loop();
}
