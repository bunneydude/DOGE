#ifndef NRF24L01P_CONST
#define NRF24L01P_CONST

#include <stdint.h>

enum nrf24l01p_commands{
   NRF24L01P_CMD_R_REGISTER = 0x0,
   NRF24L01P_CMD_W_REGISTER = 0x20,
   NRF24L01P_CMD_REGISTER_MASK = 0x1f,
   NRF24L01P_CMD_R_RX_PAYLOAD = 0x61,
   NRF24L01P_CMD_W_TX_PAYLOAD = 0xa0,
   NRF24L01P_CMD_FLUSH_TX = 0xe1,
   NRF24L01P_CMD_FLUSH_RX = 0xe2,
   NRF24L01P_CMD_REUSE_TX_PL = 0xe3,
   NRF24L01P_CMD_ACTIVATE = 0x50,
   NRF24L01P_CMD_R_RX_PL_WID = 0x60,
   NRF24L01P_CMD_NOP = 0xff,
};

enum nrf24l01p_registers{
   NRF24L01P_REG_CONFIG = 0x0,
   NRF24L01P_REG_EN_AA = 0x1,
   NRF24L01P_REG_EN_RXADDR = 0x2,
   NRF24L01P_REG_SETUP_AW = 0x3,
   NRF24L01P_REG_SETUP_RETR = 0x4,
   NRF24L01P_REG_RF_CH = 0x5,
   NRF24L01P_REG_RF_SETUP = 0x6,
   NRF24L01P_REG_STATUS = 0x7,
   NRF24L01P_REG_OBSERVE_TX = 0x8,
   NRF24L01P_REG_RPD = 0x9,
   NRF24L01P_REG_RX_ADDR_P0 = 0xa,
   NRF24L01P_REG_RX_ADDR_P1 = 0xb,
   NRF24L01P_REG_RX_ADDR_P2 = 0xc,
   NRF24L01P_REG_RX_ADDR_P3 = 0xd,
   NRF24L01P_REG_RX_ADDR_P4 = 0xe,
   NRF24L01P_REG_RX_ADDR_P5 = 0xf,
   NRF24L01P_REG_TX_ADDR = 0x10,
   NRF24L01P_REG_RX_PW_P0 = 0x11,
   NRF24L01P_REG_RX_PW_P1 = 0x12,
   NRF24L01P_REG_RX_PW_P2 = 0x13,
   NRF24L01P_REG_RX_PW_P3 = 0x14,
   NRF24L01P_REG_RX_PW_P4 = 0x15,
   NRF24L01P_REG_RX_PW_P5 = 0x16,
   NRF24L01P_REG_FIFO_STATUS = 0x17,
   NRF24L01P_REG_DYNPD = 0x1c,
   NRF24L01P_REG_FEATURE = 0x1d,
};

enum nrf24l01p_fields{
   NRF24L01P_FIELD_MASK_RX_DR = 0,
   NRF24L01P_FIELD_MASK_TX_DS = 1,
   NRF24L01P_FIELD_MASK_MAX_RT = 2,
   NRF24L01P_FIELD_EN_CRC = 3,
   NRF24L01P_FIELD_CRCO = 4,
   NRF24L01P_FIELD_PWR_UP = 5,
   NRF24L01P_FIELD_PRIM_RX = 6,
   NRF24L01P_FIELD_ENAA_P5 = 7,
   NRF24L01P_FIELD_ENAA_P4 = 8,
   NRF24L01P_FIELD_ENAA_P3 = 9,
   NRF24L01P_FIELD_ENAA_P2 = 10,
   NRF24L01P_FIELD_ENAA_P1 = 11,
   NRF24L01P_FIELD_ENAA_P0 = 12,
   NRF24L01P_FIELD_ERX_P5 = 13,
   NRF24L01P_FIELD_ERX_P4 = 14,
   NRF24L01P_FIELD_ERX_P3 = 15,
   NRF24L01P_FIELD_ERX_P2 = 16,
   NRF24L01P_FIELD_ERX_P1 = 17,
   NRF24L01P_FIELD_ERX_P0 = 18,
   NRF24L01P_FIELD_AW = 19,
   NRF24L01P_FIELD_ARD = 20,
   NRF24L01P_FIELD_ARC = 21,
   NRF24L01P_FIELD_RF_CH = 22,
   NRF24L01P_FIELD_CONT_WAVE = 23,
   NRF24L01P_FIELD_RF_DR_LOW = 24,
   NRF24L01P_FIELD_PLL_LOCK = 25,
   NRF24L01P_FIELD_RF_DR_HIGH = 26,
   NRF24L01P_FIELD_RF_PWR = 27,
   NRF24L01P_FIELD_RX_DR = 28,
   NRF24L01P_FIELD_TX_DS = 29,
   NRF24L01P_FIELD_MAX_RT = 30,
   NRF24L01P_FIELD_RX_P_NO = 31,
   NRF24L01P_FIELD_STATUS_TX_FULL = 32,
   NRF24L01P_FIELD_PLOS_CNT = 33,
   NRF24L01P_FIELD_ARC_CNT = 34,
   NRF24L01P_FIELD_RPD = 35,
   NRF24L01P_FIELD_RX_PW_P0 = 36,
   NRF24L01P_FIELD_RX_PW_P1 = 37,
   NRF24L01P_FIELD_RX_PW_P2 = 38,
   NRF24L01P_FIELD_RX_PW_P3 = 39,
   NRF24L01P_FIELD_RX_PW_P4 = 40,
   NRF24L01P_FIELD_RX_PW_P5 = 41,
   NRF24L01P_FIELD_TX_REUSE = 42,
   NRF24L01P_FIELD_FIFO_TX_FULL = 43,
   NRF24L01P_FIELD_TX_EMPTY = 44,
   NRF24L01P_FIELD_RX_FULL = 45,
   NRF24L01P_FIELD_RX_EMPTY = 46,
   NRF24L01P_FIELD_DPL_P5 = 47,
   NRF24L01P_FIELD_DPL_P4 = 48,
   NRF24L01P_FIELD_DPL_P3 = 49,
   NRF24L01P_FIELD_DPL_P2 = 50,
   NRF24L01P_FIELD_DPL_P1 = 51,
   NRF24L01P_FIELD_DPL_P0 = 52,
   NRF24L01P_FIELD_EN_DBL = 53,
   NRF24L01P_FIELD_EN_ACK_PAY = 54,
   NRF24L01P_FIELD_EN_DYN_ACK = 55,
};

static uint8_t nrf24l01p_shiftList[] = {

/* config */
   6, /* mask_rx_dr */
   5, /* mask_tx_ds */
   4, /* mask_max_rt */
   3, /* en_crc */
   2, /* crco */
   1, /* pwr_up */
   0, /* prim_rx */

/* en_aa */
   5, /* enaa_p5 */
   4, /* enaa_p4 */
   3, /* enaa_p3 */
   2, /* enaa_p2 */
   1, /* enaa_p1 */
   0, /* enaa_p0 */

/* en_rxaddr */
   5, /* erx_p5 */
   4, /* erx_p4 */
   3, /* erx_p3 */
   2, /* erx_p2 */
   1, /* erx_p1 */
   0, /* erx_p0 */

/* setup_aw */
   0, /* aw */

/* setup_retr */
   4, /* ard */
   0, /* arc */

/* rf_ch */
   0, /* rf_ch */

/* rf_setup */
   7, /* cont_wave */
   5, /* rf_dr_low */
   4, /* pll_lock */
   3, /* rf_dr_high */
   1, /* rf_pwr */

/* status */
   6, /* rx_dr */
   5, /* tx_ds */
   4, /* max_rt */
   1, /* rx_p_no */
   0, /* status_tx_full */

/* observe_tx */
   4, /* plos_cnt */
   0, /* arc_cnt */

/* rpd */
   0, /* rpd */

/* rx_pw_p0 */
   0, /* rx_pw_p0 */

/* rx_pw_p1 */
   0, /* rx_pw_p1 */

/* rx_pw_p2 */
   0, /* rx_pw_p2 */

/* rx_pw_p3 */
   0, /* rx_pw_p3 */

/* rx_pw_p4 */
   0, /* rx_pw_p4 */

/* rx_pw_p5 */
   0, /* rx_pw_p5 */

/* fifo_status */
   6, /* tx_reuse */
   5, /* fifo_tx_full */
   4, /* tx_empty */
   1, /* rx_full */
   0, /* rx_empty */

/* dynpd */
   5, /* dpl_p5 */
   4, /* dpl_p4 */
   3, /* dpl_p3 */
   2, /* dpl_p2 */
   1, /* dpl_p1 */
   0, /* dpl_p0 */

/* feature */
   2, /* en_dbl */
   1, /* en_ack_pay */
   0, /* en_dyn_ack */
};

static uint8_t nrf24l01p_maskList[] = {

/* config */
   0x40, /* mask_rx_dr */
   0x20, /* mask_tx_ds */
   0x10, /* mask_max_rt */
   0x8, /* en_crc */
   0x4, /* crco */
   0x2, /* pwr_up */
   0x1, /* prim_rx */

/* en_aa */
   0x20, /* enaa_p5 */
   0x10, /* enaa_p4 */
   0x8, /* enaa_p3 */
   0x4, /* enaa_p2 */
   0x2, /* enaa_p1 */
   0x1, /* enaa_p0 */

/* en_rxaddr */
   0x20, /* erx_p5 */
   0x10, /* erx_p4 */
   0x8, /* erx_p3 */
   0x4, /* erx_p2 */
   0x2, /* erx_p1 */
   0x1, /* erx_p0 */

/* setup_aw */
   0x3, /* aw */

/* setup_retr */
   0xf0, /* ard */
   0xf, /* arc */

/* rf_ch */
   0x7f, /* rf_ch */

/* rf_setup */
   0x80, /* cont_wave */
   0x20, /* rf_dr_low */
   0x10, /* pll_lock */
   0x8, /* rf_dr_high */
   0x6, /* rf_pwr */

/* status */
   0x40, /* rx_dr */
   0x20, /* tx_ds */
   0x10, /* max_rt */
   0xe, /* rx_p_no */
   0x1, /* status_tx_full */

/* observe_tx */
   0xf0, /* plos_cnt */
   0xf, /* arc_cnt */

/* rpd */
   0x1, /* rpd */

/* rx_pw_p0 */
   0x3f, /* rx_pw_p0 */

/* rx_pw_p1 */
   0x3f, /* rx_pw_p1 */

/* rx_pw_p2 */
   0x3f, /* rx_pw_p2 */

/* rx_pw_p3 */
   0x3f, /* rx_pw_p3 */

/* rx_pw_p4 */
   0x3f, /* rx_pw_p4 */

/* rx_pw_p5 */
   0x3f, /* rx_pw_p5 */

/* fifo_status */
   0x40, /* tx_reuse */
   0x20, /* fifo_tx_full */
   0x10, /* tx_empty */
   0x2, /* rx_full */
   0x1, /* rx_empty */

/* dynpd */
   0x20, /* dpl_p5 */
   0x10, /* dpl_p4 */
   0x8, /* dpl_p3 */
   0x4, /* dpl_p2 */
   0x2, /* dpl_p1 */
   0x1, /* dpl_p0 */

/* feature */
   0x4, /* en_dbl */
   0x2, /* en_ack_pay */
   0x1, /* en_dyn_ack */
};

#endif
