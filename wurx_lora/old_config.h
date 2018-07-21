/*
 * config.h
 *
 *  Created on: 3 mars 2016
 *      Author: faitaoudia
 */

#ifndef OLD_CONFIG_H_
#define OLD_CONFIG_H_

// Broadcast address
#define BROADCAST_ADDRESS	0x00u

// Assumed maximum payload packet size
#define MAX_PACKET_SIZE	16	// bytes

// Wake-up beacons preamble
#define WUB_PREAMBLE	0xEAu

// Sensing period
// Unit: 1/4096 s
#define MAX_SENSING_PERIOD	0xFFFF	// ~15s
#define MIN_SENSING_PERIOD	3*4096u	// 3s

// Address of the cluster head
#define CLUSTER_HEAD_ADDRESS	0x55u

// Address of the sink
#define SINK_ADDRESS			0x54u

// Address of the end-devices sensor nodes
#define SENSOR_NODE_1			0x53u

// My address
#define NODE_ADDRESS	SENSOR_NODE_1


#endif /* OLD_CONFIG_H_ */
