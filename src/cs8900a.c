// ------------------------------------------------------------------------
/// @file datatypes.h
/// @brief Device driver for the CS8900a chip in 8-bit mode.
///
/// cs8900a ethernet controller driver: The CS8900A is connected in IO-mode
/// (address lines a0,a1,a2 of the dsp are connected to the sa1,sa2,sa3 of the
/// CS8900A, sa8 and sa6 are connected to hi, all other address lines are tied
/// to lo). The base address is set to xintf zone 2 (0x080000), so the chip
/// select signal xzcs2 is used for selecting the CS8900A. For interrupt
/// requests the XINT13 is used, because this interrupt has got lowest priority.
///
/// receiver: The received data is not read in io/mode, it is read by using the
/// packet page pointer and the packet page data. The advantage by using this
/// mode, you need not to read the complete frame before parsing the next one.
///
/// @author Edwin Krasser
/// @author Harald Axmann
/// @date 08.05.2006
// ------------------------------------------------------------------------

#include "cs8900a.h"

#include "datatypes.h"
#include "dsp28.h"
#include "uip_main.h"

/** CS8900A interrupt service routine */
#ifdef __cplusplus
extern "C" interrupt void ethernet_driver_isr(void);
#else
extern interrupt void ethernet_driver_isr(void);
#endif

/* declare all DSP28 registers */
#include "dsp28/include/DSP2833x_Device.h"

/** make a word of two bytes in normal byte order */
#define MAKE_WORD(a, b) ((((uint16)b) << 8) + (uint16)a)

/* address macros */

volatile uint16 *base_address_ = (volatile uint16 *)0x0100000;
/** base address in memory */
#define CS8900A_BASE_ADDRESS (base_address_)
/** receive/transmit data (port 0) (read/write) */
#define CS8900A_RTDATA0 CS8900A_BASE_ADDRESS[0]
/** receive/transmit data (port 1) (read/write) */
#define CS8900A_RTDATA1 CS8900A_BASE_ADDRESS[1]
/** TxCMD (transmit command) (write only) */
#define CS8900A_TXCMD CS8900A_BASE_ADDRESS[2]
/** TxLength (transmit length) (write only) */
#define CS8900A_TXLENGTH CS8900A_BASE_ADDRESS[3]
/** interrupt status queue (read only) */
#define CS8900A_ISQ CS8900A_BASE_ADDRESS[4]
/** packet page pointer (read/write) */
#define CS8900A_PPP CS8900A_BASE_ADDRESS[5]
/** packet page data (port 0) (read/write) */
#define CS8900A_PPD0 CS8900A_BASE_ADDRESS[6]
/** packet page data (port 1) (read/write) */
#define CS8900A_PPD1 CS8900A_BASE_ADDRESS[7]

/** xintf timing register (used zone 2) */
#define XINTFREGS_XTIMING XintfRegs.XTIMING2.all

/* access macros */

/** packet page data */
#define PPD CS8900A_PPD0
/** sets packet page pointer
 * @param ADDR new pointer address */
#define SET_PPP(ADDR)   \
    CS8900A_PPP = ADDR; \
    asm(" rpt #(3-1) || nop") /* wait for correct timing */
/** sets packet page pointer with address increment when reading
 * @param ADDR new pointer address */
#define SET_PPP_INC(ADDR) SET_PPP(0x0100000 + ADDR)

/** reads from cs8900a memory
 * @param ADDR address to read from
 * @param VALUE variable to store value into */
#define READ(ADDR, VALUE) \
    SET_PPP(ADDR);        \
    VALUE = PPD
/** writes to cs8900a memory
 * @param ADDR address to read from
 * @param VALUE value to write */
#define WRITE(ADDR, VALUE) \
    CS8900A_PPP = ADDR;    \
    PPD = VALUE

/** disable cs8900a interrupt */
//#define ETH_INT_DISABLE() IER&=0xEFFF
/** enable cs8900a interrupt */
//#define ETH_INT_ENABLE()  IER|=0x1000

/* register names */

/* bus interface registers */

/** product identification code */
#define PRODUCT_IDENTIFICATION_CODE 0x0000
/** i/o base address */
#define IO_BASE_ADDRESS 0x0020
/** interrupt number */
#define INTERRUPT_NUMBER 0x0022
/** receive frame byte counter */
#define RECEIVE_FRAME_BYTE_COUNTER 0x0050

/* status and control registers */

/** register 3: receiver configuration */
#define REG3_RECEIVER_CONFIG 0x0102
/** register 4: receiver event */
#define REG4_RECEIVER_EVENT 0x0124
/** register 5: receiver control */
#define REG5_RECEIVER_CONTROL 0x0104
/** register 7: transmit configuration */
#define REG7_TRANSMIT_CONFIG 0x0106
/** register b: buffer configuration */
#define REGB_BUFFER_CONFIG 0x010A
/** register 13: line control */
#define REG13_LINE_CONTROL 0x0112
/** register 15: self command */
#define REG15_SELF_COMMAND 0x0114
/** register 16: self status */
#define REG16_SELF_STATUS 0x0136
/** register 17: bus control */
#define REG17_BUS_CONTROL 0x0116
/** register 18: bus status */
#define REG18_BUS_STATUS 0x0138

/* address filter registers */

/** pointer to first element of mac address (6 bytes) */
#define INDIVIDUAL_ADDRESS_0 0x0158

/* data locations */

/* receive status */
#define RECEIVE_STATUS 0x0400
/* receive frame length */
#define RECEIVE_FRAME_LENGTH 0x0402
/* receive frame */
#define RECEIVE_FRAME 0x0404

uint16 PIC;
int ethernet_driver_init(void)
{
    uint16 H;

    /* initialize external memory interface (XTIMCLK=CPUCLOCK/2) */
    EALLOW;

    /// [YCR]

    // Make sure the XINTF clock is enabled
    SysCtrlRegs.PCLKCR3.bit.XINTFENCLK = 1;

    // Configure the GPIO for XINTF with a 16-bit data bus
    // This function is in DSP2833x_Xintf.c
    InitXintf16Gpio();

    EALLOW;
    // All Zones---------------------------------
    // Timing for all zones based on XTIMCLK = SYSCLKOUT
    XintfRegs.XINTCNF2.bit.XTIMCLK = 0;
    // Buffer up to 3 writes
    XintfRegs.XINTCNF2.bit.WRBUFF = 3;
    // XCLKOUT is enabled
    XintfRegs.XINTCNF2.bit.CLKOFF = 0;
    // XCLKOUT = XTIMCLK
    XintfRegs.XINTCNF2.bit.CLKMODE = 0;

    /* initialize bus timing */
    XintfRegs.XTIMING6.bit.XWRTRAIL = 1;   // 4 cycles (26ns)
    XintfRegs.XTIMING6.bit.XWRACTIVE = 5;  // 20 cycles (133ns)
    XintfRegs.XTIMING6.bit.XWRLEAD = 1;    // 4 cycles (26ns)
    XintfRegs.XTIMING6.bit.XRDTRAIL = 1;   // 4 cycles (26ns)
    XintfRegs.XTIMING6.bit.XRDACTIVE = 6;  // 24 cycles (160ns)
    XintfRegs.XTIMING6.bit.XRDLEAD = 1;    // 4 cycles (26ns)
    XintfRegs.XTIMING6.bit.USEREADY = 0;   // Do not use XREADY
    XintfRegs.XTIMING6.bit.READYMODE =
        0;  // Does not matter because XREADY is ignored
    XintfRegs.XTIMING6.bit.XSIZE = 3;  // the only valid combination

    //  XINTFREGS_XTIMING=0x00431CB5;    /* bit 31,30,29,28,27,26,25,24=reserved
    //              * bit 23=reserved
    //                * bit 22=1: scale (double) values
    //                  * bit 21,20,19,18=reserved
    //                    * bit 17,16=11: XSIZE=3, the only valid combination
    //                      * bit 15=0: READYMODE does not matter because XREADY
    //                      is ignored
    //                        * bit 14=0: USERREADY=0 to ignore XREADY
    //                          * bit 13,12=01: XRDLEAD=1 (4 Cycles)
    //                            * bit 11,10,9=110: XRDACTIVE=6 (24
    //                            cycles=160ns)
    //                              * bit 8,7=01: XRDTAIL=1 (4 cycles=26ns)
    //                                * bit 6,5=01: XWRLEAD=1 (4 cycles=26ns)
    //                                  * bit 4,3,2=101: XWRACTIVE=5 (20
    //                                  cycles=133ns)
    //                                    			* bit 1,0=01:
    //                                    XWRTAIL=1 (4 cycles=26ns) */

    /* initialize external interrupt */
    // XIntruptRegs.XNMICR.all=0x0006;  /* bit
    // 15,14,13,12,11,10,9,8,7,6,5,4,3=reserved
    //                           * bit 2=1: generate interrupt on rising edge
    //                         * bit 1=1: XNMI is connected to int13
    //                       * bit 0=0: disable XNMI interrupt */

    /* initialize interrupt */

    EALLOW;
    PieVectTable.XINT3 = &ethernet_driver_isr;

    PieCtrlRegs.PIECTRL.bit.ENPIE = 1;   // Enable the PIE block
    PieCtrlRegs.PIEIER12.bit.INTx1 = 1;  // Enable PIE Gropu 1 INT1
    IER |= M_INT12;                      // Enable CPU int1
    EINT;                                // Enable Global Interrupts

    GpioCtrlRegs.GPBMUX2.bit.GPIO48 = 0;   // GPIO
    GpioCtrlRegs.GPBDIR.bit.GPIO48 = 0;    // Input
    GpioCtrlRegs.GPBQSEL2.bit.GPIO48 = 0;  // Xint is asynchronous

    GpioIntRegs.GPIOXINT1SEL.bit.GPIOSEL = 16;  // Xint1 is GPIO0

    XIntruptRegs.XINT3CR.bit.POLARITY = 1;  // Falling edge interrupt
    XIntruptRegs.XINT3CR.bit.ENABLE = 1;    // Enable Xint1

    /// [/YCR]
    EDIS;

    /* initialize cs8900a */

    /* check cs8900a hardware: read product identification code (must be 0x630E)
     */
    READ(PRODUCT_IDENTIFICATION_CODE, PIC);
    if (PIC != 0x630E) {
        return (1);
    }

    /* reset cs8900a */
    WRITE(REG15_SELF_COMMAND, 0x0040); /* do reset */
    do {
        READ(REG16_SELF_STATUS, H);
    } while ((H & 0x0080) == 0); /* wait for end of reset */

    /* setup cs8900a */
    WRITE(INTERRUPT_NUMBER, 0x0000); /* set interrupt number */
    WRITE(REG3_RECEIVER_CONFIG,
          0x0100); /* interrupt when frame is received without errors */
    WRITE(REG5_RECEIVER_CONTROL,
          0x0D00); /* accept frames without errors and with broadcast or
                      individual address */

    WRITE(REG13_LINE_CONTROL, 0x00C0); /* enable transmitter and receiver */
    WRITE(REG17_BUS_CONTROL, 0x8000);  /* enable interrupt */

    /* set mac address (big endian!) */
    SET_PPP_INC(INDIVIDUAL_ADDRESS_0);

    PPD = MAKE_WORD(uip_ethaddr.addr[0], uip_ethaddr.addr[1]);
    PPD = MAKE_WORD(uip_ethaddr.addr[2], uip_ethaddr.addr[3]);
    PPD = MAKE_WORD(uip_ethaddr.addr[4], uip_ethaddr.addr[5]);

    /* clear interrupt status queue */
    while (CS8900A_ISQ != 0)
        ;

    /* return ok */
    return (0);
}

#ifdef __cplusplus
extern "C" {
#endif

interrupt void ethernet_driver_isr(void)
{
    uint16 ISQ;

    //  ETH_INT_DISABLE();
    while ((ISQ = CS8900A_ISQ) != 0) {
        switch (ISQ & 0x3F) {
            case 0x04:          /* receive event */
                Uip::receive(); /* process ethernet frame */
                break;
            case 0x08: /* transmit event */
                break;
            case 0x0C: /* buffer event */
                break;
            case 0x10: /* receive miss event */
                break;
            case 0x12: /* transmit collision event */
                break;
        }
    }
    //  ETH_INT_ENABLE();
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP12;
}

#ifdef __cplusplus
}
#endif  // extern "C"

uint16 ethernet_driver_read(u8_t *data)
{
    uint16 u, len;
    register uint16 h;

    /* get length */
    SET_PPP(RECEIVE_FRAME_LENGTH);
    len = PPD;

    /* if the frame is too big to handle, throw it away */
    if (len > UIP_BUFSIZE) {
        /* skip the frame */
        u = 0;
        while (u < len) {
            h = CS8900A_RTDATA0;
            u += 2;
        }
        return 0;
    }

    /* read bytes into uip_buf */
    u = 0;
    while (u < len) {
        h = CS8900A_RTDATA0;
        data[u++] = h & 0xFF;
        data[u++] = h >> 8;
    }
    return len;
}

int ethernet_driver_send(const u8_t *header, const u8_t *data, uint16 size)
{
    uint16 u;
    uint16 header_size = 54;
    register uint16 h;

    CS8900A_TXCMD = 0x00C0;  /* start transmission after complete frame is
                                written to cs8900a */
    CS8900A_TXLENGTH = size; /* set length of frame to send (in byte) */

    /* wait for cs8900a */
    SET_PPP(REG18_BUS_STATUS);
    while ((PPD & 0x0100) == 0)
        ;

    // Send 40+14=54 bytes of header
    if (size < header_size) {
        header_size = size;
    }

    for (u = 0; u < 54; u += 2) {
        h = (header[u] & 0x00ff) + (header[u + 1] << 8);
        CS8900A_RTDATA0 = h;
    }

    if (size <= 54) return (0);

    // Send remainder of packet, the application data
    size -= 54;
    for (u = 0; u < size; u += 2) {
        h = (data[u] & 0x00ff) + (data[u + 1] << 8);
        CS8900A_RTDATA0 = h;
    }

    return (0);
}
