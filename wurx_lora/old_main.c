#include <msp430.h> 
#include <timers.h>
#include <stddef.h>
#include <string.h>
#include <uart.h>
#include <event.h>
#include <misc.h>
#include <mac_layer.h>
#include <intern_temp_sensor.h>
#include <stdlib.h>
#include <timers.h>

/*
 * What are we
 */
//#define MAIN_SINK
//#define CLUSTER_HEAD
#define SENSOR_NODE

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

/*
 * Packet type
 */
enum Packet_type_t
{
	SENSING_DATA,	// Sensing data
	COMMAND,			// Command from the sink
	COMMAND_ACK		// Command ACK
};

struct Packet_t
{
	enum Packet_type_t type;	// Type of the packet
	int data;				// Information: specific to each type
};

#define MAX_CMD_ACK_BACKOFF 4095	// 1s - MAX random backoff for cmd ack

/*
 * Events handlers specific to each class of node
 */
#ifdef MAIN_SINK
/*
 * Command ID
 */
static uint16_t _next_command_id = 0u;

/*
 * Packet receive handler
 */
void main_sink_rx_packet(void)
{
	struct Packet_t *packet;

	while (!FIFO_EMPTY(mac_rx_fifo_first, mac_rx_fifo_last))
	{
		packet = (struct Packet_t*)mac_rx_fifo[mac_rx_fifo_first].data;

		if (packet->type == SENSING_DATA)
		{
			// Sending the data through UART
			uart_send_str("NEW SENSING PACKET FROM : ");
			uart_send_int(mac_rx_fifo[mac_rx_fifo_first].address);
			uart_send_str(" Temperature : ");
			uart_send_int(packet->data);
			uart_send_str(" Celcius");
			uart_new_line();
		}
		else if (packet->type == COMMAND_ACK)
		{
			// Sending the data through UART
			uart_send_str("COMMAND ACK FROM : ");
			uart_send_int(mac_rx_fifo[mac_rx_fifo_first].address);
			uart_send_str(" FOR COMMAND : ");
			uart_send_int(packet->data);
			uart_new_line();
		}
		// Free this spot
		FIFO_INCR(mac_rx_fifo_first, MAC_RX_FIFO_SIZE);
	}
}

/*
 * SW1 button pushed handler
 */
void main_sink_sw1(void)
{
	// Creating and broadcasting a foo command to the cluster heads
	// Sending the data through UART
	uart_send_str("BROADCASTING COMMAND ID : ");
	uart_send_int(_next_command_id);
	uart_new_line();

	struct Packet_t packet;
	packet.type = COMMAND;
	packet.data = _next_command_id++;

	mac_lora_tx(BROADCAST_ADDRESS, (char*)(&packet), sizeof(struct Packet_t));
}

#elif defined(CLUSTER_HEAD)
/*
 * Packet received handler
 */
void cluster_head_rx_packet(void)
{
	struct Packet_t *packet;

	while (!FIFO_EMPTY(mac_rx_fifo_first, mac_rx_fifo_last))
	{
		packet = (struct Packet_t*)mac_rx_fifo[mac_rx_fifo_first].data;

		if (packet->type == COMMAND)
		{
			// If we received a command, we send the command ACK only if the source of the command is the SINK
			if (mac_rx_fifo[mac_rx_fifo_first].address == SINK_ADDRESS)
			{
				// Sending ACK to the sink
				struct Packet_t cmd_ack;
				cmd_ack.type = COMMAND_ACK;
				cmd_ack.data = packet->data;
				mac_lora_tx(SINK_ADDRESS, (char*)(&cmd_ack), sizeof(struct Packet_t));

				// Forwarding the command to the end-devices sensor nodes
				mac_local_tx(BROADCAST_ADDRESS, (char*)packet, sizeof(struct Packet_t));
			}
		}
		// Free this spot
		FIFO_INCR(mac_rx_fifo_first, MAC_RX_FIFO_SIZE);
	}
}

/*
 * Periodic timer
 */
void cluster_head_periodic_timer(void)
{
	// Creating and sending a sensing value (temperature) to the sink
	struct Packet_t packet;
	packet.type = SENSING_DATA;
	packet.data = temp_sensor_get_reading();
	mac_lora_tx(SINK_ADDRESS, (char*)(&packet), sizeof(struct Packet_t));

	// Setting a new period
	uint16_t new_period = rand() & MAX_SENSING_PERIOD;
	if (new_period < MIN_SENSING_PERIOD)
	{
		new_period = MIN_SENSING_PERIOD;
	}
	timer_periodic_set_period(new_period);
}

#elif defined(SENSOR_NODE)

void sensor_node_periodic_time(void)
{
	// Creating and sending a sensing value (temperature) to the sink
	struct Packet_t packet;
	packet.type = SENSING_DATA;
	packet.data = temp_sensor_get_reading();
	mac_lora_tx(SINK_ADDRESS, (char*)(&packet), sizeof(struct Packet_t));

	// Setting a new period
	uint16_t new_period = rand() & MAX_SENSING_PERIOD;
	if (new_period < MIN_SENSING_PERIOD)
	{
		new_period = MIN_SENSING_PERIOD;
	}
	timer_periodic_set_period(new_period);
}

// To remember the next cmd id to ack
static int id_cmd_ack;

void _sensor_node_send_cmd_ack(void)
{
	// Sending CMD ACK to the sink
	struct Packet_t cmd_ack;
	cmd_ack.type = COMMAND_ACK;
	cmd_ack.data = id_cmd_ack;
	mac_lora_tx(SINK_ADDRESS, (char*)(&cmd_ack), sizeof(struct Packet_t));
}

void sensor_node_rx_packet(void)
{
	struct Packet_t *packet;

	while (!FIFO_EMPTY(mac_rx_fifo_first, mac_rx_fifo_last))
	{
		packet = (struct Packet_t*)mac_rx_fifo[mac_rx_fifo_first].data;

		if (packet->type == COMMAND)
		{
			// If we received a command, we send the command ACK only if the source of the command is the SINK
			if (mac_rx_fifo[mac_rx_fifo_first].address == CLUSTER_HEAD_ADDRESS)
			{
				id_cmd_ack = packet->data;
				// Before sending ACK, a random backoff
				uint16_t backoff = rand() & MAX_CMD_ACK_BACKOFF;
				timer_set_one_shot_event(backoff, _sensor_node_send_cmd_ack);
			}
		}
		// Free this spot
		FIFO_INCR(mac_rx_fifo_first, MAC_RX_FIFO_SIZE);
	}
}

#endif


// -----------------------------------------------------------------------------
// Main
// -----------------------------------------------------------------------------
void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;

    init_clock();
    event_init();
    led_init();
    timers_init();
    srand(NODE_ADDRESS);

#ifdef MAIN_SINK
    uart_init();
    sw1_init(main_sink_sw1);

    mac_init(main_sink_rx_packet, false, true);
#elif defined(CLUSTER_HEAD)
    temp_sensor_init();

    mac_init(cluster_head_rx_packet, false, true);

    // Setting the periodic timer for periodic sensing
    timer_set_periodic_event(MIN_SENSING_PERIOD, cluster_head_periodic_timer);
#elif defined(SENSOR_NODE)
    temp_sensor_init();

    mac_init(sensor_node_rx_packet, true, false);

    // Setting the periodic timer for periodic sensing
    timer_set_periodic_event(MIN_SENSING_PERIOD, sensor_node_periodic_time);
#endif

    // Enabling global interrupts
    _EINT();

    // Starting the event loop
    event_loop();
}
