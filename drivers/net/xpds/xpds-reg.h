/*
 * Copyright 1998, 1999, 2000 Xpeed, Inc.
 * xpds-reg.h, $Revision: 1.2 $
 * License to copy and distribute is GNU General Public License, version 2.
 */
#ifndef XPDS_REG_H
#define XPDS_REG_H 1

/*
 * Main control registers
 */
#define XPDS_MCR_CONFIG		0x00
#define XPDS_MCR_TEST		0x04
#define XPDS_MCR_MASK_SET	0x08
#define XPDS_MCR_MASK_CLR	0x0c
#define XPDS_MCR_INT_SET	0x10
#define XPDS_MCR_INT_CLR	0x14
#define XPDS_MCR_GPIO_SET	0x18
#define XPDS_MCR_GPIO_CLR	0x1c
#define XPDS_MCR_TXCFG		0x20
#define XPDS_MCR_RXCFG		0x24
#define XPDS_MCR_PACKH		0x28
#define XPDS_MCR_PACKL		0x2c
#define XPDS_MCR_TXCI		0x30
#define XPDS_MCR_RXCI		0x34
#define XPDS_MCR_DBG_ADDR	0x38
#define XPDS_MCR_DBG		0x3c

#define XPDS_MCR_INT_MASK_SET	XPDS_MCR_MASK_SET
#define XPDS_MCR_INT_MASK_CLR	XPDS_MCR_MASK_CLR
#define XPDS_MCR_INT_STATUS_SET	XPDS_MCR_INT_SET
#define XPDS_MCR_INT_STATUS_CLR	XPDS_MCR_INT_CLR
#define XPDS_MCR_TX_CONFIG	XPDS_MCR_TXCFG
#define XPDS_MCR_RX_CONFIG	XPDS_MCR_RXCFG

#define XPDS_MCR_CONFIG__REVISION_MASK	0xe0
#define XPDS_MCR_CONFIG__MODE_NORMAL	0x00
#define XPDS_MCR_CONFIG__MODE_LOOPBACK	0x18
#define XPDS_MCR_CONFIG__CONFIG_MASK	0x07

#define XPDS_MCR_TEST__BIT_CLK_EN	0x10
#define XPDS_MCR_TEST__CLK_DIV_4	0x00
#define XPDS_MCR_TEST__CLK_DIV_8	0x02
#define XPDS_MCR_TEST__CLK_DIV_16	0x04
#define XPDS_MCR_TEST__CLK_DIV_32	0x06
#define XPDS_MCR_TEST__CLK_DIV_64	0x08
#define XPDS_MCR_TEST__SEL_PMD_CLK	0x01

#define XPDS_MCR_MASK__RX_FIFO		0x01
#define XPDS_MCR_MASK__TX_FIFO		0x02
#define XPDS_MCR_MASK__RX_DMA		0x04
#define XPDS_MCR_MASK__TX_DMA		0x08
#define XPDS_MCR_MASK__PCI_ERROR	0x10
#define XPDS_MCR_MASK__EXT		0x20
#define XPDS_MCR_MASK__RXCI		0x40

#define XPDS_MCR_INT__RX_FIFO		0x01
#define XPDS_MCR_INT__TX_FIFO		0x02
#define XPDS_MCR_INT__RX_DMA		0x04
#define XPDS_MCR_INT__TX_DMA		0x08
#define XPDS_MCR_INT__PCI_ERROR		0x10
#define XPDS_MCR_INT__EXT		0x20
#define XPDS_MCR_INT__RXCI		0x40
#define XPDS_MCR_INT__INT		0x80

#define XPDS_MCR_GPIO__GP_MASK		0x0f
#define XPDS_MCR_GPIO__GP_SEPROM_CS	0x01
#define XPDS_MCR_GPIO__GP_SEPROM_SK	0x02
#define XPDS_MCR_GPIO__GP_SEPROM_DO	0x04
#define XPDS_MCR_GPIO__GP_SEPROM_DI	0x08
#define XPDS_MCR_GPIO__GP_FPGA_NT	0x00
#define XPDS_MCR_GPIO__GP_FPGA_LT	0x01
#define XPDS_MCR_GPIO__GP_ASIC_ACT_LED	0x02
#define XPDS_MCR_GPIO__GP_ASIC_LINK_LED	0x08
#define XPDS_MCR_GPIO__GP_SDSL_ACT_LED	0x08
#define XPDS_MCR_GPIO__GP_FPGA_LT_NT	0x01
#define XPDS_MCR_GPIO__GP_FPGA_TX_LED	0x02
#define XPDS_MCR_GPIO__GP_FPGA_RX_LED	0x04
#define XPDS_MCR_GPIO__OE_MASK		0xf0
#define XPDS_MCR_GPIO__OE_SEPROM_CS	0x10
#define XPDS_MCR_GPIO__OE_SEPROM_SK	0x20
#define XPDS_MCR_GPIO__OE_SEPROM_DO	0x40
#define XPDS_MCR_GPIO__OE_SEPROM_DI	0x80
#define XPDS_MCR_GPIO__OE_ASIC_ACT_LED	0x20
#define XPDS_MCR_GPIO__OE_ASIC_LINK_LED	0x80
#define XPDS_MCR_GPIO__OE_SDSL_ACT_LED	0x80
#define XPDS_MCR_GPIO__OE_FPGA_LT_NT	0x10
#define XPDS_MCR_GPIO__OE_FPGA_TX_LED	0x20
#define XPDS_MCR_GPIO__OE_FPGA_RX_LED	0x40

#define XPDS_MCR_TXCFG__TRANS		0x01
#define XPDS_MCR_TXCFG__ENABLE		0x02
#define XPDS_MCR_TXCFG__FILL		0x04
#define XPDS_MCR_TXCFG__SKIP_CRC	0x08
#define XPDS_MCR_TXCFG__CRC		0x10
#define XPDS_MCR_TXCFG__BIT_REV_CRC	0x20
#define XPDS_MCR_TXCFG__BIT_REV_DATA	0x40
#define XPDS_MCR_TXCFG__SWAP_CRC	0x80

#define XPDS_MCR_RXCFG__TRANS		0x01
#define XPDS_MCR_RXCFG__ENABLE		0x02
#define XPDS_MCR_RXCFG__IGNORE_CRC	0x04
#define XPDS_MCR_RXCFG__NO_CRC		0x08
#define XPDS_MCR_RXCFG__CRC		0x10
#define XPDS_MCR_RXCFG__BIT_REV_CRC	0x20
#define XPDS_MCR_RXCFG__BIT_REV_DATA	0x40
#define XPDS_MCR_RXCFG__SWAP_CRC	0x80

#define XPDS_MCR_TXCI__PMD_ENABLE	0x80
#define XPDS_MCR_TXCI__PMD_CONFIG_MASK	0x60
#define XPDS_MCR_TXCI__PMD_CONFIG_B1	0x00
#define XPDS_MCR_TXCI__PMD_CONFIG_B2	0x20
#define XPDS_MCR_TXCI__PMD_CONFIG_B1B2	0x40
#define XPDS_MCR_TXCI__PMD_CONFIG_B1B2D	0x60
#define XPDS_MCR_TXCI__PMD_RESQ		0x10
#define XPDS_MCR_TXCI__MASK		0x0f

#define XPDS_MCR_RXCI__PMD_ACTIVE	0x40
#define XPDS_MCR_RXCI__MASK		0x0f

/*
 * FIFO control registers
 */
#define XPDS_FCR_CONFIG		0x00
#define XPDS_FCR_INC		0x04
#define XPDS_FCR_STAT		0x08
#define XPDS_FCR_TAG		0x0c

#define XPDS_FCR_STATUS		XPDS_FCR_STAT

#define XPDS_FCR_CONFIG__RESET		0x01
#define XPDS_FCR_CONFIG__PTR_MASK	0x06

#define XPDS_FCR_INC__NEXT	0x01

#define XPDS_FCR_STAT__LAST		0x80
#define XPDS_FCR_STAT__SIZE_MASK	0x1f

#define XPDS_FCR_TAG__MASK	0x0f

/*
 * DMA
 */
#define XPDS_DMA_CONFIG		0x00
#define XPDS_DMA_GO		0x04
#define XPDS_DMA_DESC		0x08
#define XPDS_DMA_SET_MASK	0x0c
#define XPDS_DMA_CLR_MASK	0x10
#define XPDS_DMA_SET_STAT	0x14
#define XPDS_DMA_CLR_STAT	0x18
#define XPDS_DMA_STAT_ADDR	0x1c
#define XPDS_DMA_BUF_ADDR	0x20
#define XPDS_DMA_DBG_ADDR	0x24
#define XPDS_DMA_DBG		0x28

#define XPDS_DMA_MASK_SET	XPDS_DMA_SET_MASK
#define XPDS_DMA_MASK_CLR	XPDS_DMA_CLR_MASK
#define XPDS_DMA_STAT_SET	XPDS_DMA_SET_STAT
#define XPDS_DMA_STAT_CLR	XPDS_DMA_CLR_STAT

#define XPDS_DMA_CONFIG__TEST_ENABLE	0x08
#define XPDS_DMA_CONFIG__BURST_ENABLE	0x04
#define XPDS_DMA_CONFIG__RESET		0x02
#define XPDS_DMA_CONFIG__ENABLE		0x01

#define XPDS_DMA_GO__HUNT		0x01

#define XPDS_DMA_DESC__SW_GO			0x80000000
#define XPDS_DMA_DESC__HW_GO			0x40000000
#define XPDS_DMA_DESC__NEXT_VALID		0x20000000
#define XPDS_DMA_DESC__PACKET_TAG_MASK		0x000f0000
#define XPDS_DMA_DESC__PACKET_TAG_OK		0x00000000
#define XPDS_DMA_DESC__PACKET_TAG_CRC		0x00010000
#define XPDS_DMA_DESC__PACKET_TAG_BOUNDARY_VIOLATION	0x00020000
#define XPDS_DMA_DESC__PACKET_TAG_LONG		0x00040000
#define XPDS_DMA_DESC__PACKET_TAG_ABORT		0x00050000
#define XPDS_DMA_DESC__PACKET_TAG_OVERFLOW	0x00080000
#define XPDS_DMA_DESC__PACKET_LENGTH_MASK	0x0000ffff

#define XPDS_DMA_MASK__FLOW		0x20
#define XPDS_DMA_MASK__CRC		0x10
#define XPDS_DMA_MASK__LONG		0x08
#define XPDS_DMA_MASK__ABORT		0x04
#define XPDS_DMA_MASK__DONE		0x01

#define XPDS_DMA_STAT__FLOW		0x20
#define XPDS_DMA_STAT__CRC		0x10
#define XPDS_DMA_STAT__LONG		0x08
#define XPDS_DMA_STAT__ABORT		0x04
#define XPDS_DMA_STAT__DONE		0x01

/*
 * Auxiliary registers (8032 for SDSL)
 */
#define XPDS_AUX_CONTROL		0x00
#define XPDS_AUX_WRITE_ADDR		0x04
#define XPDS_AUX_WRITE_DATA		0x08
#define XPDS_AUX_READ_DATA		0x04
#define XPDS_AUX_READ_ADDR		0x08
#define XPDS_AUX_MAILBOX_DATA		0x0c

#define XPDS_AUX_MAILBOX__HANDSHAKE			0x80
#define XPDS_AUX_MAILBOX__WRITE_BOOTDATA		0x00
#define XPDS_AUX_MAILBOX__WRITE_MAC_ADDR		0x01
#define XPDS_AUX_MAILBOX__WRITE_HARDWARE_VERSION	0x02
#define XPDS_AUX_MAILBOX__WRITE_FIRMWARE_VERSION	0x03
#define XPDS_AUX_MAILBOX__WRITE_PHYSICAL_MODE		0x04
#define XPDS_AUX_MAILBOX__READ_MAC_ADDR			0x05
#define XPDS_AUX_MAILBOX__READ_HARDWARE_VERSION		0x06
#define XPDS_AUX_MAILBOX__READ_FIRMWARE_VERSION		0x07
#define XPDS_AUX_MAILBOX__READ_PHYSICAL_MODE		0x08
#define XPDS_AUX_MAILBOX__WRITE_TEST_MODE		0x09
#define XPDS_AUX_MAILBOX__CLEAR_IRQ			0x0a
#define XPDS_AUX_MAILBOX__READ_PHYSICAL_STATE		0x0b

#define XPDS_AUX_MAILBOX__NIC_WRITE_DEBUG_PORT		0x0d
#define XPDS_AUX_MAILBOX__NIC_WRITE_IRQ_TYPE		0x0e
#define XPDS_AUX_MAILBOX__NIC_WRITE_EXIT_CODE		0x0f

#endif
