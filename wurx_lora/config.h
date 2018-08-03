/*
 * config.h
 *
 *  Created on: 3 mars 2016
 *      Author: faitaoudia
 */

#ifndef CONFIG_H_
#define CONFIG_H_

// Broadcast address
#define BROADCAST_ADDRESS	0x00u

// Assumed maximum payload packet size
#define MAX_PACKET_SIZE	16	// bytes

// Wake-up beacons preamble
#define WUB_PREAMBLE	0xEAu

// Address of the cluster head
#define		NODE_NO	1u

#if NODE_NO == 1u //for sender Author: Xu Ao
#define NODE_ADDRESS	0x55u
#define DEST_ADDRESS	0x56u
#elif NODE_NO == 2u//for receiver Author:Xu Ao
#define NODE_ADDRESS	0x56u
#define DEST_ADDRESS	0x55u
#endif


#endif /* OLD_CONFIG_H_ */
