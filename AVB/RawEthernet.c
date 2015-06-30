//
//  RawEthernet.c
//  AVB
//
//  Created by Konstantin Klitenik on 6/29/15.
//  Copyright (c) 2015 DK Audio. All rights reserved.
//

#include "RawEthernet.h"

#include <net/bpf.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#define MY_DEST_MAC0	0x00
#define MY_DEST_MAC1	0x11
#define MY_DEST_MAC2	0x22
#define MY_DEST_MAC3	0x33
#define MY_DEST_MAC4	0x44
#define MY_DEST_MAC5	0x22

#define DEFAULT_IF	"en0"
#define BUF_SIZ		1024


void send_raw()
{
    printf("Sending raw packet\n");
    
    char buf[ BUF_SIZ ] = { 0 };
    int bpf = 0;
    
    for( int i = 0; i <= 4; i++ )
    {
        sprintf(buf, "/dev/bpf%i", i);
        printf("Trying to open %s - ", buf);
        bpf = open( buf, O_RDWR );
        
        if( bpf != -1 )
        {
            break;
        }
        
        printf("error: %d (%s)\n", errno, strerror(errno));
    }
    
    if (bpf != -1)
    {
        printf("Opened bpf(%d) on: %s\n", bpf, buf);
    }
    else
    {
        return;
    }
    
    const char* interface = DEFAULT_IF;
    struct ifreq bound_if;
    
    strcpy(bound_if.ifr_name, interface);
    if(ioctl( bpf, BIOCSETIF, &bound_if ) == 0)
    {
        printf("Bound to %s\n", interface);
    }
    else
    {
        printf("Failed to bond: err %d (%s)", errno, strerror(errno));
    }
    
    char sendbuf[BUF_SIZ];
    struct ether_header *eth_header = (struct ether_header *)sendbuf;
    int tx_len = 0;
    eth_header->ether_dhost[0] = 0xe8;
    eth_header->ether_dhost[1] = 0x03;
    eth_header->ether_dhost[2] = 0x9a;
    eth_header->ether_dhost[3] = 0x10;
    eth_header->ether_dhost[4] = 0xf7;
    eth_header->ether_dhost[5] = 0xe3;
    eth_header->ether_shost[0] = 0xff;
    eth_header->ether_shost[1] = 0xff;
    eth_header->ether_shost[2] = 0xff;
    eth_header->ether_shost[3] = 0xff;
    eth_header->ether_shost[4] = 0xff;
    eth_header->ether_shost[5] = 0xff;
    
    // E8-11-32-E5-4B-10
    // e8-03-9a-10-f7-e3
    
    
    tx_len += sizeof(struct ether_header);
    
    // data
    sendbuf[tx_len++] = 0xfa;
    sendbuf[tx_len++] = 0xce;
    sendbuf[tx_len++] = 0xfe;
    sendbuf[tx_len++] = 0xed;
    sendbuf[tx_len++] = 0xbe;
    sendbuf[tx_len++] = 0xef;
    sendbuf[tx_len++] = 0xfa;
    sendbuf[tx_len++] = 0xce;

    
    // eth type
//    tx_len = 64;
    eth_header->ether_type = tx_len;
    
    printf("Writing...");
    
    for (;;)
    {
        int bytes_written = write(bpf, &sendbuf, tx_len);
        printf("Written %d bytes\n", bytes_written);
        usleep(100000);
    }
}