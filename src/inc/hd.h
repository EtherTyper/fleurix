/* */

extern struct devtab hdtab;

/* TODO: add partition support
 * note: the first entry is the whole device
 * */
struct hd_minor {
    uint    m_hd;
    uint    m_start;
    uint    m_limit;
};

extern struct hd_minor hd_minor[16];

#define HD_STAT 0x1F7
#define HD_CMD  0x1F7

/* commands for HD_CMD */
#define HD_CMD_READ    0x20
#define HD_CMD_WRITE   0x30

/* flags for HD_STAT */
#define HD_BSY	  0x80
#define HD_DRDY	  0x40 //Device Ready
#define HD_DF     0x20 //Device Fault
#define HD_ERR	  0x01


int hd_request();
int nulldev();
int nodev();
