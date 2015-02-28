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

enum nrf24l01p_field_shift{

/* config */
   NRF24L01P_SHIFT_MASK_RX_DR = 6, /* mask_rx_dr */
   NRF24L01P_SHIFT_MASK_TX_DS = 5, /* mask_tx_ds */
   NRF24L01P_SHIFT_MASK_MAX_RT = 4, /* mask_max_rt */
   NRF24L01P_SHIFT_EN_CRC = 3, /* en_crc */
   NRF24L01P_SHIFT_CRCO = 2, /* crco */
   NRF24L01P_SHIFT_PWR_UP = 1, /* pwr_up */
   NRF24L01P_SHIFT_PRIM_RX = 0, /* prim_rx */

/* en_aa */
   NRF24L01P_SHIFT_ENAA_P5 = 5, /* enaa_p5 */
   NRF24L01P_SHIFT_ENAA_P4 = 4, /* enaa_p4 */
   NRF24L01P_SHIFT_ENAA_P3 = 3, /* enaa_p3 */
   NRF24L01P_SHIFT_ENAA_P2 = 2, /* enaa_p2 */
   NRF24L01P_SHIFT_ENAA_P1 = 1, /* enaa_p1 */
   NRF24L01P_SHIFT_ENAA_P0 = 0, /* enaa_p0 */

/* en_rxaddr */
   NRF24L01P_SHIFT_ERX_P5 = 5, /* erx_p5 */
   NRF24L01P_SHIFT_ERX_P4 = 4, /* erx_p4 */
   NRF24L01P_SHIFT_ERX_P3 = 3, /* erx_p3 */
   NRF24L01P_SHIFT_ERX_P2 = 2, /* erx_p2 */
   NRF24L01P_SHIFT_ERX_P1 = 1, /* erx_p1 */
   NRF24L01P_SHIFT_ERX_P0 = 0, /* erx_p0 */

/* setup_aw */
   NRF24L01P_SHIFT_AW = 0, /* aw */

/* setup_retr */
   NRF24L01P_SHIFT_ARD = 4, /* ard */
   NRF24L01P_SHIFT_ARC = 0, /* arc */

/* rf_ch */
   NRF24L01P_SHIFT_RF_CH = 0, /* rf_ch */

/* rf_setup */
   NRF24L01P_SHIFT_CONT_WAVE = 7, /* cont_wave */
   NRF24L01P_SHIFT_RF_DR_LOW = 5, /* rf_dr_low */
   NRF24L01P_SHIFT_PLL_LOCK = 4, /* pll_lock */
   NRF24L01P_SHIFT_RF_DR_HIGH = 3, /* rf_dr_high */
   NRF24L01P_SHIFT_RF_PWR = 1, /* rf_pwr */

/* status */
   NRF24L01P_SHIFT_RX_DR = 6, /* rx_dr */
   NRF24L01P_SHIFT_TX_DS = 5, /* tx_ds */
   NRF24L01P_SHIFT_MAX_RT = 4, /* max_rt */
   NRF24L01P_SHIFT_RX_P_NO = 1, /* rx_p_no */
   NRF24L01P_SHIFT_STATUS_TX_FULL = 0, /* status_tx_full */

/* observe_tx */
   NRF24L01P_SHIFT_PLOS_CNT = 4, /* plos_cnt */
   NRF24L01P_SHIFT_ARC_CNT = 0, /* arc_cnt */

/* rpd */
   NRF24L01P_SHIFT_RPD = 0, /* rpd */

/* rx_pw_p0 */
   NRF24L01P_SHIFT_RX_PW_P0 = 0, /* rx_pw_p0 */

/* rx_pw_p1 */
   NRF24L01P_SHIFT_RX_PW_P1 = 0, /* rx_pw_p1 */

/* rx_pw_p2 */
   NRF24L01P_SHIFT_RX_PW_P2 = 0, /* rx_pw_p2 */

/* rx_pw_p3 */
   NRF24L01P_SHIFT_RX_PW_P3 = 0, /* rx_pw_p3 */

/* rx_pw_p4 */
   NRF24L01P_SHIFT_RX_PW_P4 = 0, /* rx_pw_p4 */

/* rx_pw_p5 */
   NRF24L01P_SHIFT_RX_PW_P5 = 0, /* rx_pw_p5 */

/* fifo_status */
   NRF24L01P_SHIFT_TX_REUSE = 6, /* tx_reuse */
   NRF24L01P_SHIFT_FIFO_TX_FULL = 5, /* fifo_tx_full */
   NRF24L01P_SHIFT_TX_EMPTY = 4, /* tx_empty */
   NRF24L01P_SHIFT_RX_FULL = 1, /* rx_full */
   NRF24L01P_SHIFT_RX_EMPTY = 0, /* rx_empty */

/* dynpd */
   NRF24L01P_SHIFT_DPL_P5 = 5, /* dpl_p5 */
   NRF24L01P_SHIFT_DPL_P4 = 4, /* dpl_p4 */
   NRF24L01P_SHIFT_DPL_P3 = 3, /* dpl_p3 */
   NRF24L01P_SHIFT_DPL_P2 = 2, /* dpl_p2 */
   NRF24L01P_SHIFT_DPL_P1 = 1, /* dpl_p1 */
   NRF24L01P_SHIFT_DPL_P0 = 0, /* dpl_p0 */

/* feature */
   NRF24L01P_SHIFT_EN_DBL = 2, /* en_dbl */
   NRF24L01P_SHIFT_EN_ACK_PAY = 1, /* en_ack_pay */
   NRF24L01P_SHIFT_EN_DYN_ACK = 0, /* en_dyn_ack */
};

enum nrf24l01p_field_mask{

/* config */
   NRF24L01P_MASK_MASK_RX_DR = 0x40, /* mask_rx_dr */
   NRF24L01P_MASK_MASK_TX_DS = 0x20, /* mask_tx_ds */
   NRF24L01P_MASK_MASK_MAX_RT = 0x10, /* mask_max_rt */
   NRF24L01P_MASK_EN_CRC = 0x8, /* en_crc */
   NRF24L01P_MASK_CRCO = 0x4, /* crco */
   NRF24L01P_MASK_PWR_UP = 0x2, /* pwr_up */
   NRF24L01P_MASK_PRIM_RX = 0x1, /* prim_rx */

/* en_aa */
   NRF24L01P_MASK_ENAA_P5 = 0x20, /* enaa_p5 */
   NRF24L01P_MASK_ENAA_P4 = 0x10, /* enaa_p4 */
   NRF24L01P_MASK_ENAA_P3 = 0x8, /* enaa_p3 */
   NRF24L01P_MASK_ENAA_P2 = 0x4, /* enaa_p2 */
   NRF24L01P_MASK_ENAA_P1 = 0x2, /* enaa_p1 */
   NRF24L01P_MASK_ENAA_P0 = 0x1, /* enaa_p0 */

/* en_rxaddr */
   NRF24L01P_MASK_ERX_P5 = 0x20, /* erx_p5 */
   NRF24L01P_MASK_ERX_P4 = 0x10, /* erx_p4 */
   NRF24L01P_MASK_ERX_P3 = 0x8, /* erx_p3 */
   NRF24L01P_MASK_ERX_P2 = 0x4, /* erx_p2 */
   NRF24L01P_MASK_ERX_P1 = 0x2, /* erx_p1 */
   NRF24L01P_MASK_ERX_P0 = 0x1, /* erx_p0 */

/* setup_aw */
   NRF24L01P_MASK_AW = 0x3, /* aw */

/* setup_retr */
   NRF24L01P_MASK_ARD = 0xf0, /* ard */
   NRF24L01P_MASK_ARC = 0xf, /* arc */

/* rf_ch */
   NRF24L01P_MASK_RF_CH = 0x7f, /* rf_ch */

/* rf_setup */
   NRF24L01P_MASK_CONT_WAVE = 0x80, /* cont_wave */
   NRF24L01P_MASK_RF_DR_LOW = 0x20, /* rf_dr_low */
   NRF24L01P_MASK_PLL_LOCK = 0x10, /* pll_lock */
   NRF24L01P_MASK_RF_DR_HIGH = 0x8, /* rf_dr_high */
   NRF24L01P_MASK_RF_PWR = 0x6, /* rf_pwr */

/* status */
   NRF24L01P_MASK_RX_DR = 0x40, /* rx_dr */
   NRF24L01P_MASK_TX_DS = 0x20, /* tx_ds */
   NRF24L01P_MASK_MAX_RT = 0x10, /* max_rt */
   NRF24L01P_MASK_RX_P_NO = 0xe, /* rx_p_no */
   NRF24L01P_MASK_STATUS_TX_FULL = 0x1, /* status_tx_full */

/* observe_tx */
   NRF24L01P_MASK_PLOS_CNT = 0xf0, /* plos_cnt */
   NRF24L01P_MASK_ARC_CNT = 0xf, /* arc_cnt */

/* rpd */
   NRF24L01P_MASK_RPD = 0x1, /* rpd */

/* rx_pw_p0 */
   NRF24L01P_MASK_RX_PW_P0 = 0x3f, /* rx_pw_p0 */

/* rx_pw_p1 */
   NRF24L01P_MASK_RX_PW_P1 = 0x3f, /* rx_pw_p1 */

/* rx_pw_p2 */
   NRF24L01P_MASK_RX_PW_P2 = 0x3f, /* rx_pw_p2 */

/* rx_pw_p3 */
   NRF24L01P_MASK_RX_PW_P3 = 0x3f, /* rx_pw_p3 */

/* rx_pw_p4 */
   NRF24L01P_MASK_RX_PW_P4 = 0x3f, /* rx_pw_p4 */

/* rx_pw_p5 */
   NRF24L01P_MASK_RX_PW_P5 = 0x3f, /* rx_pw_p5 */

/* fifo_status */
   NRF24L01P_MASK_TX_REUSE = 0x40, /* tx_reuse */
   NRF24L01P_MASK_FIFO_TX_FULL = 0x20, /* fifo_tx_full */
   NRF24L01P_MASK_TX_EMPTY = 0x10, /* tx_empty */
   NRF24L01P_MASK_RX_FULL = 0x2, /* rx_full */
   NRF24L01P_MASK_RX_EMPTY = 0x1, /* rx_empty */

/* dynpd */
   NRF24L01P_MASK_DPL_P5 = 0x20, /* dpl_p5 */
   NRF24L01P_MASK_DPL_P4 = 0x10, /* dpl_p4 */
   NRF24L01P_MASK_DPL_P3 = 0x8, /* dpl_p3 */
   NRF24L01P_MASK_DPL_P2 = 0x4, /* dpl_p2 */
   NRF24L01P_MASK_DPL_P1 = 0x2, /* dpl_p1 */
   NRF24L01P_MASK_DPL_P0 = 0x1, /* dpl_p0 */

/* feature */
   NRF24L01P_MASK_EN_DBL = 0x4, /* en_dbl */
   NRF24L01P_MASK_EN_ACK_PAY = 0x2, /* en_ack_pay */
   NRF24L01P_MASK_EN_DYN_ACK = 0x1, /* en_dyn_ack */
};

#endif
