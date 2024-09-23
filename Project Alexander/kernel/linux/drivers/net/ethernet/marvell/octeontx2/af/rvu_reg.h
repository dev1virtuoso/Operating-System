/* SPDX-License-Identifier: GPL-2.0 */
/* Marvell RVU Admin Function driver
 *
 * Copyright (C) 2018 Marvell.
 *
 */

#ifndef RVU_REG_H
#define RVU_REG_H

/* Admin function registers */
#define RVU_AF_MSIXTR_BASE                  (0x10)
#define RVU_AF_ECO                          (0x20)
#define RVU_AF_BLK_RST                      (0x30)
#define RVU_AF_PF_BAR4_ADDR                 (0x40)
#define RVU_AF_RAS                          (0x100)
#define RVU_AF_RAS_W1S                      (0x108)
#define RVU_AF_RAS_ENA_W1S                  (0x110)
#define RVU_AF_RAS_ENA_W1C                  (0x118)
#define RVU_AF_GEN_INT                      (0x120)
#define RVU_AF_GEN_INT_W1S                  (0x128)
#define RVU_AF_GEN_INT_ENA_W1S              (0x130)
#define RVU_AF_GEN_INT_ENA_W1C              (0x138)
#define	RVU_AF_AFPF_MBOX0		    (0x02000)
#define	RVU_AF_AFPF_MBOX1		    (0x02008)
#define RVU_AF_AFPFX_MBOXX(a, b)            (0x2000 | (a) << 4 | (b) << 3)
#define RVU_AF_PFME_STATUS                  (0x2800)
#define RVU_AF_PFTRPEND                     (0x2810)
#define RVU_AF_PFTRPEND_W1S                 (0x2820)
#define RVU_AF_PF_RST                       (0x2840)
#define RVU_AF_HWVF_RST                     (0x2850)
#define RVU_AF_PFAF_MBOX_INT                (0x2880)
#define RVU_AF_PFAF_MBOX_INT_W1S            (0x2888)
#define RVU_AF_PFAF_MBOX_INT_ENA_W1S        (0x2890)
#define RVU_AF_PFAF_MBOX_INT_ENA_W1C        (0x2898)
#define RVU_AF_PFFLR_INT                    (0x28a0)
#define RVU_AF_PFFLR_INT_W1S                (0x28a8)
#define RVU_AF_PFFLR_INT_ENA_W1S            (0x28b0)
#define RVU_AF_PFFLR_INT_ENA_W1C            (0x28b8)
#define RVU_AF_PFME_INT                     (0x28c0)
#define RVU_AF_PFME_INT_W1S                 (0x28c8)
#define RVU_AF_PFME_INT_ENA_W1S             (0x28d0)
#define RVU_AF_PFME_INT_ENA_W1C             (0x28d8)
#define RVU_AF_PFX_BAR4_ADDR(a)             (0x5000 | (a) << 4)
#define RVU_AF_PFX_BAR4_CFG                 (0x5200 | (a) << 4)
#define RVU_AF_PFX_VF_BAR4_ADDR             (0x5400 | (a) << 4)
#define RVU_AF_PFX_VF_BAR4_CFG              (0x5600 | (a) << 4)
#define RVU_AF_PFX_LMTLINE_ADDR             (0x5800 | (a) << 4)
#define RVU_AF_SMMU_ADDR_REQ		    (0x6000)
#define RVU_AF_SMMU_TXN_REQ		    (0x6008)
#define RVU_AF_SMMU_ADDR_RSP_STS	    (0x6010)
#define RVU_AF_SMMU_ADDR_TLN		    (0x6018)
#define RVU_AF_SMMU_TLN_FLIT0		    (0x6020)

/* Admin function's privileged PF/VF registers */
#define RVU_PRIV_CONST                      (0x8000000)
#define RVU_PRIV_GEN_CFG                    (0x8000010)
#define RVU_PRIV_CLK_CFG                    (0x8000020)
#define RVU_PRIV_ACTIVE_PC                  (0x8000030)
#define RVU_PRIV_PFX_CFG(a)                 (0x8000100 | (a) << 16)
#define RVU_PRIV_PFX_MSIX_CFG(a)            (0x8000110 | (a) << 16)
#define RVU_PRIV_PFX_ID_CFG(a)              (0x8000120 | (a) << 16)
#define RVU_PRIV_PFX_INT_CFG(a)             (0x8000200 | (a) << 16)
#define RVU_PRIV_PFX_NIXX_CFG(a)            (0x8000300 | (a) << 3)
#define RVU_PRIV_PFX_NPA_CFG		    (0x8000310)
#define RVU_PRIV_PFX_SSO_CFG                (0x8000320)
#define RVU_PRIV_PFX_SSOW_CFG               (0x8000330)
#define RVU_PRIV_PFX_TIM_CFG                (0x8000340)
#define RVU_PRIV_PFX_CPTX_CFG(a)            (0x8000350 | (a) << 3)
#define RVU_PRIV_BLOCK_TYPEX_REV(a)         (0x8000400 | (a) << 3)
#define RVU_PRIV_HWVFX_INT_CFG(a)           (0x8001280 | (a) << 16)
#define RVU_PRIV_HWVFX_NIXX_CFG(a)          (0x8001300 | (a) << 3)
#define RVU_PRIV_HWVFX_NPA_CFG              (0x8001310)
#define RVU_PRIV_HWVFX_SSO_CFG              (0x8001320)
#define RVU_PRIV_HWVFX_SSOW_CFG             (0x8001330)
#define RVU_PRIV_HWVFX_TIM_CFG              (0x8001340)
#define RVU_PRIV_HWVFX_CPTX_CFG(a)          (0x8001350 | (a) << 3)

/* RVU PF registers */
#define	RVU_PF_VFX_PFVF_MBOX0		    (0x00000)
#define	RVU_PF_VFX_PFVF_MBOX1		    (0x00008)
#define RVU_PF_VFX_PFVF_MBOXX(a, b)         (0x0 | (a) << 12 | (b) << 3)
#define RVU_PF_VF_BAR4_ADDR                 (0x10)
#define RVU_PF_BLOCK_ADDRX_DISC(a)          (0x200 | (a) << 3)
#define RVU_PF_VFME_STATUSX(a)              (0x800 | (a) << 3)
#define RVU_PF_VFTRPENDX(a)                 (0x820 | (a) << 3)
#define RVU_PF_VFTRPEND_W1SX(a)             (0x840 | (a) << 3)
#define RVU_PF_VFPF_MBOX_INTX(a)            (0x880 | (a) << 3)
#define RVU_PF_VFPF_MBOX_INT_W1SX(a)        (0x8A0 | (a) << 3)
#define RVU_PF_VFPF_MBOX_INT_ENA_W1SX(a)    (0x8C0 | (a) << 3)
#define RVU_PF_VFPF_MBOX_INT_ENA_W1CX(a)    (0x8E0 | (a) << 3)
#define RVU_PF_VFFLR_INTX(a)                (0x900 | (a) << 3)
#define RVU_PF_VFFLR_INT_W1SX(a)            (0x920 | (a) << 3)
#define RVU_PF_VFFLR_INT_ENA_W1SX(a)        (0x940 | (a) << 3)
#define RVU_PF_VFFLR_INT_ENA_W1CX(a)        (0x960 | (a) << 3)
#define RVU_PF_VFME_INTX(a)                 (0x980 | (a) << 3)
#define RVU_PF_VFME_INT_W1SX(a)             (0x9A0 | (a) << 3)
#define RVU_PF_VFME_INT_ENA_W1SX(a)         (0x9C0 | (a) << 3)
#define RVU_PF_VFME_INT_ENA_W1CX(a)         (0x9E0 | (a) << 3)
#define RVU_PF_PFAF_MBOX0                   (0xC00)
#define RVU_PF_PFAF_MBOX1                   (0xC08)
#define RVU_PF_PFAF_MBOXX(a)                (0xC00 | (a) << 3)
#define RVU_PF_INT                          (0xc20)
#define RVU_PF_INT_W1S                      (0xc28)
#define RVU_PF_INT_ENA_W1S                  (0xc30)
#define RVU_PF_INT_ENA_W1C                  (0xc38)
#define RVU_PF_MSIX_VECX_ADDR(a)            (0x000 | (a) << 4)
#define RVU_PF_MSIX_VECX_CTL(a)             (0x008 | (a) << 4)
#define RVU_PF_MSIX_PBAX(a)                 (0xF0000 | (a) << 3)
#define RVU_PF_VF_MBOX_ADDR                 (0xC40)
#define RVU_PF_LMTLINE_ADDR                 (0xC48)

/* RVU VF registers */
#define	RVU_VF_VFPF_MBOX0		    (0x00000)
#define	RVU_VF_VFPF_MBOX1		    (0x00008)

/* NPA block's admin function registers */
#define NPA_AF_BLK_RST                  (0x0000)
#define NPA_AF_CONST                    (0x0010)
#define NPA_AF_CONST1                   (0x0018)
#define NPA_AF_LF_RST                   (0x0020)
#define NPA_AF_GEN_CFG                  (0x0030)
#define NPA_AF_NDC_CFG                  (0x0040)
#define NPA_AF_NDC_SYNC                 (0x0050)
#define NPA_AF_INP_CTL                  (0x00D0)
#define NPA_AF_ACTIVE_CYCLES_PC         (0x00F0)
#define NPA_AF_AVG_DELAY                (0x0100)
#define NPA_AF_GEN_INT                  (0x0140)
#define NPA_AF_GEN_INT_W1S              (0x0148)
#define NPA_AF_GEN_INT_ENA_W1S          (0x0150)
#define NPA_AF_GEN_INT_ENA_W1C          (0x0158)
#define NPA_AF_RVU_INT                  (0x0160)
#define NPA_AF_RVU_INT_W1S              (0x0168)
#define NPA_AF_RVU_INT_ENA_W1S          (0x0170)
#define NPA_AF_RVU_INT_ENA_W1C          (0x0178)
#define NPA_AF_ERR_INT			(0x0180)
#define NPA_AF_ERR_INT_W1S		(0x0188)
#define NPA_AF_ERR_INT_ENA_W1S		(0x0190)
#define NPA_AF_ERR_INT_ENA_W1C		(0x0198)
#define NPA_AF_RAS			(0x01A0)
#define NPA_AF_RAS_W1S			(0x01A8)
#define NPA_AF_RAS_ENA_W1S		(0x01B0)
#define NPA_AF_RAS_ENA_W1C		(0x01B8)
#define NPA_AF_BP_TEST                  (0x0200)
#define NPA_AF_ECO                      (0x0300)
#define NPA_AF_AQ_CFG                   (0x0600)
#define NPA_AF_AQ_BASE                  (0x0610)
#define NPA_AF_AQ_STATUS		(0x0620)
#define NPA_AF_AQ_DOOR                  (0x0630)
#define NPA_AF_AQ_DONE_WAIT             (0x0640)
#define NPA_AF_AQ_DONE                  (0x0650)
#define NPA_AF_AQ_DONE_ACK              (0x0660)
#define NPA_AF_AQ_DONE_INT              (0x0680)
#define NPA_AF_AQ_DONE_INT_W1S          (0x0688)
#define NPA_AF_AQ_DONE_ENA_W1S          (0x0690)
#define NPA_AF_AQ_DONE_ENA_W1C          (0x0698)
#define NPA_AF_BATCH_CTL		(0x06a0)
#define NPA_AF_LFX_AURAS_CFG(a)         (0x4000 | (a) << 18)
#define NPA_AF_LFX_LOC_AURAS_BASE(a)    (0x4010 | (a) << 18)
#define NPA_AF_LFX_QINTS_CFG(a)         (0x4100 | (a) << 18)
#define NPA_AF_LFX_QINTS_BASE(a)        (0x4110 | (a) << 18)
#define NPA_PRIV_AF_INT_CFG             (0x10000)
#define NPA_PRIV_LFX_CFG		(0x10010)
#define NPA_PRIV_LFX_INT_CFG		(0x10020)
#define NPA_AF_RVU_LF_CFG_DEBUG         (0x10030)

/* NIX block's admin function registers */
#define NIX_AF_CFG			(0x0000)
#define NIX_AF_STATUS			(0x0010)
#define NIX_AF_NDC_CFG			(0x0018)
#define NIX_AF_CONST			(0x0020)
#define NIX_AF_CONST1			(0x0028)
#define NIX_AF_CONST2			(0x0030)
#define NIX_AF_CONST3			(0x0038)
#define NIX_AF_SQ_CONST			(0x0040)
#define NIX_AF_CQ_CONST			(0x0048)
#define NIX_AF_RQ_CONST			(0x0050)
#define NIX_AF_PL_CONST			(0x0058)
#define NIX_AF_PSE_CONST		(0x0060)
#define NIX_AF_TL1_CONST		(0x0070)
#define NIX_AF_TL2_CONST		(0x0078)
#define NIX_AF_TL3_CONST		(0x0080)
#define NIX_AF_TL4_CONST		(0x0088)
#define NIX_AF_MDQ_CONST		(0x0090)
#define NIX_AF_MC_MIRROR_CONST		(0x0098)
#define NIX_AF_LSO_CFG			(0x00A8)
#define NIX_AF_BLK_RST			(0x00B0)
#define NIX_AF_TX_TSTMP_CFG		(0x00C0)
#define NIX_AF_PL_TS			(0x00C8)
#define NIX_AF_RX_CFG			(0x00D0)
#define NIX_AF_AVG_DELAY		(0x00E0)
#define NIX_AF_CINT_DELAY		(0x00F0)
#define NIX_AF_VWQE_TIMER		(0x00F8)
#define NIX_AF_RX_MCAST_BASE		(0x0100)
#define NIX_AF_RX_MCAST_CFG		(0x0110)
#define NIX_AF_RX_MCAST_BUF_BASE	(0x0120)
#define NIX_AF_RX_MCAST_BUF_CFG		(0x0130)
#define NIX_AF_RX_MIRROR_BUF_BASE	(0x0140)
#define NIX_AF_RX_MIRROR_BUF_CFG	(0x0148)
#define NIX_AF_LF_RST			(0x0150)
#define NIX_AF_GEN_INT			(0x0160)
#define NIX_AF_GEN_INT_W1S		(0x0168)
#define NIX_AF_GEN_INT_ENA_W1S		(0x0170)
#define NIX_AF_GEN_INT_ENA_W1C		(0x0178)
#define NIX_AF_ERR_INT			(0x0180)
#define NIX_AF_ERR_INT_W1S		(0x0188)
#define NIX_AF_ERR_INT_ENA_W1S		(0x0190)
#define NIX_AF_ERR_INT_ENA_W1C		(0x0198)
#define NIX_AF_RAS			(0x01A0)
#define NIX_AF_RAS_W1S			(0x01A8)
#define NIX_AF_RAS_ENA_W1S		(0x01B0)
#define NIX_AF_RAS_ENA_W1C		(0x01B8)
#define NIX_AF_RVU_INT			(0x01C0)
#define NIX_AF_RVU_INT_W1S		(0x01C8)
#define NIX_AF_RVU_INT_ENA_W1S		(0x01D0)
#define NIX_AF_RVU_INT_ENA_W1C		(0x01D8)
#define NIX_AF_TCP_TIMER		(0x01E0)
#define NIX_AF_RX_DEF_ET(a)		(0x01F0ull | (uint64_t)(a) << 3)
#define NIX_AF_RX_DEF_OL2		(0x0200)
#define NIX_AF_RX_DEF_OIP4		(0x0210)
#define NIX_AF_RX_DEF_IIP4		(0x0220)
#define NIX_AF_RX_DEF_VLAN0_PCP_DEI	(0x0228)
#define NIX_AF_RX_DEF_OIP6		(0x0230)
#define NIX_AF_RX_DEF_VLAN1_PCP_DEI	(0x0238)
#define NIX_AF_RX_DEF_IIP6		(0x0240)
#define NIX_AF_RX_DEF_OTCP		(0x0250)
#define NIX_AF_RX_DEF_ITCP		(0x0260)
#define NIX_AF_RX_DEF_OUDP		(0x0270)
#define NIX_AF_RX_DEF_IUDP		(0x0280)
#define NIX_AF_RX_DEF_OSCTP		(0x0290)
#define NIX_AF_RX_DEF_CST_APAD0		(0x0298)
#define NIX_AF_RX_DEF_ISCTP		(0x02A0)
#define NIX_AF_RX_DEF_IPSECX		(0x02B0)
#define NIX_AF_RX_DEF_CST_APAD1		(0x02A8)
#define NIX_AF_RX_DEF_IIP4_DSCP		(0x02E0)
#define NIX_AF_RX_DEF_OIP4_DSCP		(0x02E8)
#define NIX_AF_RX_DEF_IIP6_DSCP		(0x02F0)
#define NIX_AF_RX_DEF_OIP6_DSCP		(0x02F8)
#define NIX_AF_RX_IPSEC_GEN_CFG		(0x0300)
#define NIX_AF_RX_CPTX_INST_ADDR	(0x0310)
#define NIX_AF_RX_CPTX_INST_QSEL(a)	(0x0320ull | (uint64_t)(a) << 3)
#define NIX_AF_RX_CPTX_CREDIT(a)	(0x0360ull | (uint64_t)(a) << 3)
#define NIX_AF_NDC_RX_SYNC		(0x03E0)
#define NIX_AF_NDC_TX_SYNC		(0x03F0)
#define NIX_AF_AQ_CFG			(0x0400)
#define NIX_AF_AQ_BASE			(0x0410)
#define NIX_AF_AQ_STATUS		(0x0420)
#define NIX_AF_AQ_DOOR			(0x0430)
#define NIX_AF_AQ_DONE_WAIT		(0x0440)
#define NIX_AF_AQ_DONE			(0x0450)
#define NIX_AF_AQ_DONE_ACK		(0x0460)
#define NIX_AF_AQ_DONE_TIMER		(0x0470)
#define NIX_AF_AQ_DONE_INT		(0x0480)
#define NIX_AF_AQ_DONE_INT_W1S		(0x0488)
#define NIX_AF_AQ_DONE_ENA_W1S		(0x0490)
#define NIX_AF_AQ_DONE_ENA_W1C		(0x0498)
#define NIX_AF_RX_LINKX_SLX_SPKT_CNT	(0x0500)
#define NIX_AF_RX_LINKX_SLX_SXQE_CNT	(0x0510)
#define NIX_AF_RX_MCAST_JOBSX_SW_CNT	(0x0520)
#define NIX_AF_RX_MIRROR_JOBSX_SW_CNT	(0x0530)
#define NIX_AF_RX_LINKX_CFG(a)		(0x0540 | (a) << 16)
#define NIX_AF_RX_SW_SYNC		(0x0550)
#define NIX_AF_RX_SW_SYNC_DONE		(0x0560)
#define NIX_AF_SEB_ECO			(0x0600)
#define NIX_AF_SEB_TEST_BP		(0x0610)
#define NIX_AF_NORM_TX_FIFO_STATUS	(0x0620)
#define NIX_AF_EXPR_TX_FIFO_STATUS	(0x0630)
#define NIX_AF_SDP_TX_FIFO_STATUS	(0x0640)
#define NIX_AF_TX_NPC_CAPTURE_CONFIG	(0x0660)
#define NIX_AF_TX_NPC_CAPTURE_INFO	(0x0670)
#define NIX_AF_SEB_CFG			(0x05F0)
#define NIX_PTP_1STEP_EN		BIT_ULL(2)

#define NIX_AF_DEBUG_NPC_RESP_DATAX(a)          (0x680 | (a) << 3)
#define NIX_AF_SMQX_CFG(a)                      (0x700 | (a) << 16)
#define NIX_AF_SQM_DBG_CTL_STATUS               (0x750)
#define NIX_AF_DWRR_SDP_MTU                     (0x790) /* All CN10K except CN10KB */
#define NIX_AF_DWRR_MTUX(a)			(0x790 | (a) << 16) /* Only for CN10KB */
#define NIX_AF_DWRR_RPM_MTU                     (0x7A0)
#define NIX_AF_PSE_CHANNEL_LEVEL                (0x800)
#define NIX_AF_PSE_SHAPER_CFG                   (0x810)
#define NIX_AF_TX_EXPR_CREDIT			(0x830)
#define NIX_AF_MARK_FORMATX_CTL(a)              (0x900 | (a) << 18)
#define NIX_AF_TX_LINKX_NORM_CREDIT(a)		(0xA00 | (a) << 16)
#define NIX_AF_TX_LINKX_EXPR_CREDIT(a)		(0xA10 | (a) << 16)
#define NIX_AF_TX_LINKX_SW_XOFF(a)              (0xA20 | (a) << 16)
#define NIX_AF_TX_LINKX_HW_XOFF(a)              (0xA30 | (a) << 16)
#define NIX_AF_SDP_LINK_CREDIT                  (0xa40)
#define NIX_AF_SDP_SW_XOFFX(a)                  (0xA60 | (a) << 3)
#define NIX_AF_SDP_HW_XOFFX(a)                  (0xAC0 | (a) << 3)
#define NIX_AF_TL4X_BP_STATUS(a)                (0xB00 | (a) << 16)
#define NIX_AF_TL4X_SDP_LINK_CFG(a)             (0xB10 | (a) << 16)
#define NIX_AF_TL1X_SCHEDULE(a)                 (0xC00 | (a) << 16)
#define NIX_AF_TL1X_SHAPE(a)                    (0xC10 | (a) << 16)
#define NIX_AF_TL1X_CIR(a)                      (0xC20 | (a) << 16)
#define NIX_AF_TL1X_SHAPE_STATE(a)              (0xC50 | (a) << 16)
#define NIX_AF_TL1X_SW_XOFF(a)                  (0xC70 | (a) << 16)
#define NIX_AF_TL1X_TOPOLOGY(a)                 (0xC80 | (a) << 16)
#define NIX_AF_TL1X_GREEN(a)                    (0xC90 | (a) << 16)
#define NIX_AF_TL1X_YELLOW(a)                   (0xCA0 | (a) << 16)
#define NIX_AF_TL1X_RED(a)                      (0xCB0 | (a) << 16)
#define NIX_AF_TL1X_MD_DEBUG0(a)                (0xCC0 | (a) << 16)
#define NIX_AF_TL1X_MD_DEBUG1(a)                (0xCC8 | (a) << 16)
#define NIX_AF_TL1X_MD_DEBUG2(a)                (0xCD0 | (a) << 16)
#define NIX_AF_TL1X_MD_DEBUG3(a)                (0xCD8 | (a) << 16)
#define NIX_AF_TL1A_DEBUG                       (0xce0)
#define NIX_AF_TL1B_DEBUG                       (0xcf0)
#define NIX_AF_TL1_DEBUG_GREEN                  (0xd00)
#define NIX_AF_TL1_DEBUG_NODE                   (0xd10)
#define NIX_AF_TL1X_DROPPED_PACKETS(a)          (0xD20 | (a) << 16)
#define NIX_AF_TL1X_DROPPED_BYTES(a)            (0xD30 | (a) << 16)
#define NIX_AF_TL1X_RED_PACKETS(a)              (0xD40 | (a) << 16)
#define NIX_AF_TL1X_RED_BYTES(a)                (0xD50 | (a) << 16)
#define NIX_AF_TL1X_YELLOW_PACKETS(a)           (0xD60 | (a) << 16)
#define NIX_AF_TL1X_YELLOW_BYTES(a)             (0xD70 | (a) << 16)
#define NIX_AF_TL1X_GREEN_PACKETS(a)            (0xD80 | (a) << 16)
#define NIX_AF_TL1X_GREEN_BYTES(a)              (0xD90 | (a) << 16)
#define NIX_AF_TL2X_SCHEDULE(a)                 (0xE00 | (a) << 16)
#define NIX_AF_TL2X_SHAPE(a)                    (0xE10 | (a) << 16)
#define NIX_AF_TL2X_CIR(a)                      (0xE20 | (a) << 16)
#define NIX_AF_TL2X_PIR(a)                      (0xE30 | (a) << 16)
#define NIX_AF_TL2X_SCHED_STATE(a)              (0xE40 | (a) << 16)
#define NIX_AF_TL2X_SHAPE_STATE(a)              (0xE50 | (a) << 16)
#define NIX_AF_TL2X_POINTERS(a)                 (0xE60 | (a) << 16)
#define NIX_AF_TL2X_SW_XOFF(a)                  (0xE70 | (a) << 16)
#define NIX_AF_TL2X_TOPOLOGY(a)                 (0xE80 | (a) << 16)
#define NIX_AF_TL2X_PARENT(a)                   (0xE88 | (a) << 16)
#define NIX_AF_TL2X_GREEN(a)                    (0xE90 | (a) << 16)
#define NIX_AF_TL2X_YELLOW(a)                   (0xEA0 | (a) << 16)
#define NIX_AF_TL2X_RED(a)                      (0xEB0 | (a) << 16)
#define NIX_AF_TL2X_MD_DEBUG0(a)                (0xEC0 | (a) << 16)
#define NIX_AF_TL2X_MD_DEBUG1(a)                (0xEC8 | (a) << 16)
#define NIX_AF_TL2X_MD_DEBUG2(a)                (0xED0 | (a) << 16)
#define NIX_AF_TL2X_MD_DEBUG3(a)                (0xED8 | (a) << 16)
#define NIX_AF_TL2A_DEBUG                       (0xee0)
#define NIX_AF_TL2B_DEBUG                       (0xef0)
#define NIX_AF_TL3X_SCHEDULE(a)                 (0x1000 | (a) << 16)
#define NIX_AF_TL3X_SHAPE(a)                    (0x1010 | (a) << 16)
#define NIX_AF_TL3X_CIR(a)                      (0x1020 | (a) << 16)
#define NIX_AF_TL3X_PIR(a)                      (0x1030 | (a) << 16)
#define NIX_AF_TL3X_SCHED_STATE(a)              (0x1040 | (a) << 16)
#define NIX_AF_TL3X_SHAPE_STATE(a)              (0x1050 | (a) << 16)
#define NIX_AF_TL3X_POINTERS(a)                 (0x1060 | (a) << 16)
#define NIX_AF_TL3X_SW_XOFF(a)                  (0x1070 | (a) << 16)
#define NIX_AF_TL3X_TOPOLOGY(a)                 (0x1080 | (a) << 16)
#define NIX_AF_TL3X_PARENT(a)                   (0x1088 | (a) << 16)
#define NIX_AF_TL3X_GREEN(a)                    (0x1090 | (a) << 16)
#define NIX_AF_TL3X_YELLOW(a)                   (0x10A0 | (a) << 16)
#define NIX_AF_TL3X_RED(a)                      (0x10B0 | (a) << 16)
#define NIX_AF_TL3X_MD_DEBUG0(a)                (0x10C0 | (a) << 16)
#define NIX_AF_TL3X_MD_DEBUG1(a)                (0x10C8 | (a) << 16)
#define NIX_AF_TL3X_MD_DEBUG2(a)                (0x10D0 | (a) << 16)
#define NIX_AF_TL3X_MD_DEBUG3(a)                (0x10D8 | (a) << 16)
#define NIX_AF_TL3A_DEBUG                       (0x10e0)
#define NIX_AF_TL3B_DEBUG                       (0x10f0)
#define NIX_AF_TL4X_SCHEDULE(a)                 (0x1200 | (a) << 16)
#define NIX_AF_TL4X_SHAPE(a)                    (0x1210 | (a) << 16)
#define NIX_AF_TL4X_CIR(a)                      (0x1220 | (a) << 16)
#define NIX_AF_TL4X_PIR(a)                      (0x1230 | (a) << 16)
#define NIX_AF_TL4X_SCHED_STATE(a)              (0x1240 | (a) << 16)
#define NIX_AF_TL4X_SHAPE_STATE(a)              (0x1250 | (a) << 16)
#define NIX_AF_TL4X_POINTERS(a)                 (0x1260 | (a) << 16)
#define NIX_AF_TL4X_SW_XOFF(a)                  (0x1270 | (a) << 16)
#define NIX_AF_TL4X_TOPOLOGY(a)                 (0x1280 | (a) << 16)
#define NIX_AF_TL4X_PARENT(a)                   (0x1288 | (a) << 16)
#define NIX_AF_TL4X_GREEN(a)                    (0x1290 | (a) << 16)
#define NIX_AF_TL4X_YELLOW(a)                   (0x12A0 | (a) << 16)
#define NIX_AF_TL4X_RED(a)                      (0x12B0 | (a) << 16)
#define NIX_AF_TL4X_MD_DEBUG0(a)                (0x12C0 | (a) << 16)
#define NIX_AF_TL4X_MD_DEBUG1(a)                (0x12C8 | (a) << 16)
#define NIX_AF_TL4X_MD_DEBUG2(a)                (0x12D0 | (a) << 16)
#define NIX_AF_TL4X_MD_DEBUG3(a)                (0x12D8 | (a) << 16)
#define NIX_AF_TL4A_DEBUG                       (0x12e0)
#define NIX_AF_TL4B_DEBUG                       (0x12f0)
#define NIX_AF_MDQX_SCHEDULE(a)                 (0x1400 | (a) << 16)
#define NIX_AF_MDQX_SHAPE(a)                    (0x1410 | (a) << 16)
#define NIX_AF_MDQX_CIR(a)                      (0x1420 | (a) << 16)
#define NIX_AF_MDQX_PIR(a)                      (0x1430 | (a) << 16)
#define NIX_AF_MDQX_SCHED_STATE(a)              (0x1440 | (a) << 16)
#define NIX_AF_MDQX_SHAPE_STATE(a)              (0x1450 | (a) << 16)
#define NIX_AF_MDQX_POINTERS(a)                 (0x1460 | (a) << 16)
#define NIX_AF_MDQX_SW_XOFF(a)                  (0x1470 | (a) << 16)
#define NIX_AF_MDQX_PARENT(a)                   (0x1480 | (a) << 16)
#define NIX_AF_MDQX_MD_DEBUG(a)                 (0x14C0 | (a) << 16)
#define NIX_AF_MDQX_PTR_FIFO(a)                 (0x14D0 | (a) << 16)
#define NIX_AF_MDQA_DEBUG                       (0x14e0)
#define NIX_AF_MDQB_DEBUG                       (0x14f0)
#define NIX_AF_TL3_TL2X_CFG(a)                  (0x1600 | (a) << 18)
#define NIX_AF_TL3_TL2X_BP_STATUS(a)            (0x1610 | (a) << 16)
#define NIX_AF_TL3_TL2X_LINKX_CFG(a, b)         (0x1700 | (a) << 16 | (b) << 3)
#define NIX_AF_RX_FLOW_KEY_ALGX_FIELDX(a, b)    (0x1800 | (a) << 18 | (b) << 3)
#define NIX_AF_TX_MCASTX(a)                     (0x1900 | (a) << 15)
#define NIX_AF_TX_VTAG_DEFX_CTL(a)              (0x1A00 | (a) << 16)
#define NIX_AF_TX_VTAG_DEFX_DATA(a)             (0x1A10 | (a) << 16)
#define NIX_AF_RX_BPIDX_STATUS(a)               (0x1A20 | (a) << 17)
#define NIX_AF_RX_CHANX_CFG(a)                  (0x1A30 | (a) << 15)
#define NIX_AF_CINT_TIMERX(a)                   (0x1A40 | (a) << 18)
#define NIX_AF_LSO_FORMATX_FIELDX(a, b)         (0x1B00 | (a) << 16 | (b) << 3)
#define NIX_AF_LFX_CFG(a)		(0x4000 | (a) << 17)
#define NIX_AF_LFX_SQS_CFG(a)		(0x4020 | (a) << 17)
#define NIX_AF_LFX_TX_CFG2(a)		(0x4028 | (a) << 17)
#define NIX_AF_LFX_SQS_BASE(a)		(0x4030 | (a) << 17)
#define NIX_AF_LFX_RQS_CFG(a)		(0x4040 | (a) << 17)
#define NIX_AF_LFX_RQS_BASE(a)		(0x4050 | (a) << 17)
#define NIX_AF_LFX_CQS_CFG(a)		(0x4060 | (a) << 17)
#define NIX_AF_LFX_CQS_BASE(a)		(0x4070 | (a) << 17)
#define NIX_AF_LFX_TX_CFG(a)		(0x4080 | (a) << 17)
#define NIX_AF_LFX_TX_PARSE_CFG(a)	(0x4090 | (a) << 17)
#define NIX_AF_LFX_RX_CFG(a)		(0x40A0 | (a) << 17)
#define NIX_AF_LFX_RSS_CFG(a)		(0x40C0 | (a) << 17)
#define NIX_AF_LFX_RSS_BASE(a)		(0x40D0 | (a) << 17)
#define NIX_AF_LFX_QINTS_CFG(a)		(0x4100 | (a) << 17)
#define NIX_AF_LFX_QINTS_BASE(a)	(0x4110 | (a) << 17)
#define NIX_AF_LFX_CINTS_CFG(a)		(0x4120 | (a) << 17)
#define NIX_AF_LFX_CINTS_BASE(a)	(0x4130 | (a) << 17)
#define NIX_AF_LFX_RX_IPSEC_CFG0(a)	(0x4140 | (a) << 17)
#define NIX_AF_LFX_RX_IPSEC_CFG1(a)	(0x4148 | (a) << 17)
#define NIX_AF_LFX_RX_IPSEC_DYNO_CFG(a)	(0x4150 | (a) << 17)
#define NIX_AF_LFX_RX_IPSEC_DYNO_BASE(a)	(0x4158 | (a) << 17)
#define NIX_AF_LFX_RX_IPSEC_SA_BASE(a)	(0x4170 | (a) << 17)
#define NIX_AF_LFX_TX_STATUS(a)		(0x4180 | (a) << 17)
#define NIX_AF_LFX_RX_VTAG_TYPEX(a, b)	(0x4200 | (a) << 17 | (b) << 3)
#define NIX_AF_LFX_LOCKX(a, b)		(0x4300 | (a) << 17 | (b) << 3)
#define NIX_AF_LFX_TX_STATX(a, b)	(0x4400 | (a) << 17 | (b) << 3)
#define NIX_AF_LFX_RX_STATX(a, b)	(0x4500 | (a) << 17 | (b) << 3)
#define NIX_AF_LFX_RSS_GRPX(a, b)	(0x4600 | (a) << 17 | (b) << 3)
#define NIX_AF_RX_NPC_MC_RCV		(0x4700)
#define NIX_AF_RX_NPC_MC_DROP		(0x4710)
#define NIX_AF_RX_NPC_MIRROR_RCV	(0x4720)
#define NIX_AF_RX_NPC_MIRROR_DROP	(0x4730)
#define NIX_AF_RX_ACTIVE_CYCLES_PCX(a)	(0x4800 | (a) << 16)
#define NIX_AF_LINKX_CFG(a)		(0x4010 | (a) << 17)
#define NIX_AF_MDQX_IN_MD_COUNT(a)	(0x14e0 | (a) << 16)
#define NIX_AF_SMQX_STATUS(a)		(0x730 | (a) << 16)
#define NIX_AF_MDQX_OUT_MD_COUNT(a)	(0xdb0 | (a) << 16)

#define NIX_PRIV_AF_INT_CFG		(0x8000000)
#define NIX_PRIV_LFX_CFG		(0x8000010)
#define NIX_PRIV_LFX_INT_CFG		(0x8000020)
#define NIX_AF_RVU_LF_CFG_DEBUG		(0x8000030)

#define NIX_AF_LINKX_BASE_MASK		GENMASK_ULL(11, 0)
#define NIX_AF_LINKX_RANGE_MASK		GENMASK_ULL(19, 16)
#define NIX_AF_LINKX_MCS_CNT_MASK	GENMASK_ULL(33, 32)

#define NIX_CONST_MAX_BPIDS		GENMASK_ULL(23, 12)
#define NIX_CONST_SDP_CHANS		GENMASK_ULL(11, 0)

#define NIX_AF_MDQ_PARENT_MASK         GENMASK_ULL(24, 16)
#define NIX_AF_TL4_PARENT_MASK         GENMASK_ULL(23, 16)
#define NIX_AF_TL3_PARENT_MASK         GENMASK_ULL(23, 16)
#define NIX_AF_TL2_PARENT_MASK         GENMASK_ULL(20, 16)

/* SSO */
#define SSO_AF_CONST			(0x1000)
#define SSO_AF_CONST1			(0x1008)
#define SSO_AF_BLK_RST			(0x10f8)
#define SSO_AF_LF_HWGRP_RST		(0x10e0)
#define SSO_AF_RVU_LF_CFG_DEBUG		(0x3800)
#define SSO_PRIV_LFX_HWGRP_CFG		(0x10000)
#define SSO_PRIV_LFX_HWGRP_INT_CFG	(0x20000)

/* SSOW */
#define SSOW_AF_RVU_LF_HWS_CFG_DEBUG	(0x0010)
#define SSOW_AF_LF_HWS_RST		(0x0030)
#define SSOW_PRIV_LFX_HWS_CFG		(0x1000)
#define SSOW_PRIV_LFX_HWS_INT_CFG	(0x2000)

/* TIM */
#define TIM_AF_CONST			(0x90)
#define TIM_PRIV_LFX_CFG		(0x20000)
#define TIM_PRIV_LFX_INT_CFG		(0x24000)
#define TIM_AF_RVU_LF_CFG_DEBUG		(0x30000)
#define TIM_AF_BLK_RST			(0x10)
#define TIM_AF_LF_RST			(0x20)

/* CPT */
#define CPT_AF_CONSTANTS0               (0x0000)
#define CPT_AF_CONSTANTS1               (0x1000)
#define CPT_AF_DIAG                     (0x3000)
#define CPT_AF_ECO                      (0x4000)
#define CPT_AF_FLTX_INT(a)              (0xa000ull | (u64)(a) << 3)
#define CPT_AF_FLTX_INT_W1S(a)          (0xb000ull | (u64)(a) << 3)
#define CPT_AF_FLTX_INT_ENA_W1C(a)      (0xc000ull | (u64)(a) << 3)
#define CPT_AF_FLTX_INT_ENA_W1S(a)      (0xd000ull | (u64)(a) << 3)
#define CPT_AF_PSNX_EXE(a)              (0xe000ull | (u64)(a) << 3)
#define CPT_AF_PSNX_EXE_W1S(a)          (0xf000ull | (u64)(a) << 3)
#define CPT_AF_PSNX_LF(a)               (0x10000ull | (u64)(a) << 3)
#define CPT_AF_PSNX_LF_W1S(a)           (0x11000ull | (u64)(a) << 3)
#define CPT_AF_EXEX_CTL2(a)             (0x12000ull | (u64)(a) << 3)
#define CPT_AF_EXEX_STS(a)              (0x13000ull | (u64)(a) << 3)
#define CPT_AF_EXE_ERR_INFO             (0x14000)
#define CPT_AF_EXEX_ACTIVE(a)           (0x16000ull | (u64)(a) << 3)
#define CPT_AF_INST_REQ_PC              (0x17000)
#define CPT_AF_INST_LATENCY_PC          (0x18000)
#define CPT_AF_RD_REQ_PC                (0x19000)
#define CPT_AF_RD_LATENCY_PC            (0x1a000)
#define CPT_AF_RD_UC_PC                 (0x1b000)
#define CPT_AF_ACTIVE_CYCLES_PC         (0x1c000)
#define CPT_AF_EXE_DBG_CTL              (0x1d000)
#define CPT_AF_EXE_DBG_DATA             (0x1e000)
#define CPT_AF_EXE_REQ_TIMER            (0x1f000)
#define CPT_AF_EXEX_CTL(a)              (0x20000ull | (u64)(a) << 3)
#define CPT_AF_EXE_PERF_CTL             (0x21000)
#define CPT_AF_EXE_DBG_CNTX(a)          (0x22000ull | (u64)(a) << 3)
#define CPT_AF_EXE_PERF_EVENT_CNT       (0x23000)
#define CPT_AF_EXE_EPCI_INBX_CNT(a)     (0x24000ull | (u64)(a) << 3)
#define CPT_AF_EXE_EPCI_OUTBX_CNT(a)    (0x25000ull | (u64)(a) << 3)
#define CPT_AF_EXEX_UCODE_BASE(a)       (0x26000ull | (u64)(a) << 3)
#define CPT_AF_LFX_CTL(a)               (0x27000ull | (u64)(a) << 3)
#define CPT_AF_LFX_CTL2(a)              (0x29000ull | (u64)(a) << 3)
#define CPT_AF_CPTCLK_CNT               (0x2a000)
#define CPT_AF_PF_FUNC                  (0x2b000)
#define CPT_AF_LFX_PTR_CTL(a)           (0x2c000ull | (u64)(a) << 3)
#define CPT_AF_GRPX_THR(a)              (0x2d000ull | (u64)(a) << 3)
#define CPT_AF_CTL                      (0x2e000ull)
#define CPT_AF_XEX_THR(a)               (0x2f000ull | (u64)(a) << 3)
#define CPT_PRIV_LFX_CFG                (0x41000)
#define CPT_PRIV_AF_INT_CFG             (0x42000)
#define CPT_PRIV_LFX_INT_CFG            (0x43000)
#define CPT_AF_LF_RST                   (0x44000)
#define CPT_AF_RVU_LF_CFG_DEBUG         (0x45000)
#define CPT_AF_BLK_RST                  (0x46000)
#define CPT_AF_RVU_INT                  (0x47000)
#define CPT_AF_RVU_INT_W1S              (0x47008)
#define CPT_AF_RVU_INT_ENA_W1S          (0x47010)
#define CPT_AF_RVU_INT_ENA_W1C          (0x47018)
#define CPT_AF_RAS_INT                  (0x47020)
#define CPT_AF_RAS_INT_W1S              (0x47028)
#define CPT_AF_RAS_INT_ENA_W1S          (0x47030)
#define CPT_AF_RAS_INT_ENA_W1C          (0x47038)
#define CPT_AF_CTX_FLUSH_TIMER          (0x48000ull)
#define CPT_AF_CTX_ERR                  (0x48008ull)
#define CPT_AF_CTX_ENC_ID               (0x48010ull)
#define CPT_AF_CTX_MIS_PC		(0x49400ull)
#define CPT_AF_CTX_HIT_PC		(0x49408ull)
#define CPT_AF_CTX_AOP_PC		(0x49410ull)
#define CPT_AF_CTX_AOP_LATENCY_PC       (0x49418ull)
#define CPT_AF_CTX_IFETCH_PC            (0x49420ull)
#define CPT_AF_CTX_IFETCH_LATENCY_PC    (0x49428ull)
#define CPT_AF_CTX_FFETCH_PC            (0x49430ull)
#define CPT_AF_CTX_FFETCH_LATENCY_PC    (0x49438ull)
#define CPT_AF_CTX_WBACK_PC             (0x49440ull)
#define CPT_AF_CTX_WBACK_LATENCY_PC     (0x49448ull)
#define CPT_AF_CTX_PSH_PC               (0x49450ull)
#define CPT_AF_CTX_PSH_LATENCY_PC       (0x49458ull)
#define CPT_AF_CTX_CAM_DATA(a)          (0x49800ull | (u64)(a) << 3)
#define CPT_AF_RXC_CFG1                 (0x50000ull)
#define CPT_AF_RXC_TIME                 (0x50010ull)
#define CPT_AF_RXC_TIME_CFG             (0x50018ull)
#define CPT_AF_RXC_DFRG                 (0x50020ull)
#define CPT_AF_RXC_ACTIVE_STS           (0x50028ull)
#define CPT_AF_RXC_ZOMBIE_STS           (0x50030ull)
#define CPT_AF_X2PX_LINK_CFG(a)         (0x51000ull | (u64)(a) << 3)

#define AF_BAR2_ALIASX(a, b)            (0x9100000ull | (a) << 12 | (b))
#define CPT_AF_BAR2_SEL                 0x9000000
#define CPT_AF_BAR2_ALIASX(a, b)        AF_BAR2_ALIASX(a, b)

#define CPT_AF_LF_CTL2_SHIFT 3
#define CPT_AF_LF_SSO_PF_FUNC_SHIFT 32

#define CPT_LF_CTL                      0x10
#define CPT_LF_INPROG                   0x40
#define CPT_LF_Q_SIZE                   0x100
#define CPT_LF_Q_INST_PTR               0x110
#define CPT_LF_Q_GRP_PTR                0x120
#define CPT_LF_CTX_FLUSH                0x510

#define NPC_AF_BLK_RST                  (0x00040)

/* NPC */
#define NPC_AF_CFG			(0x00000)
#define NPC_AF_ACTIVE_PC		(0x00010)
#define NPC_AF_CONST			(0x00020)
#define NPC_AF_CONST1			(0x00030)
#define NPC_AF_BLK_RST			(0x00040)
#define NPC_AF_MCAM_SCRUB_CTL		(0x000a0)
#define NPC_AF_KCAM_SCRUB_CTL		(0x000b0)
#define NPC_AF_CONST2			(0x00100)
#define NPC_AF_CONST3			(0x00110)
#define NPC_AF_KPUX_CFG(a)		(0x00500 | (a) << 3)
#define NPC_AF_PCK_CFG			(0x00600)
#define NPC_AF_PCK_DEF_OL2		(0x00610)
#define NPC_AF_PCK_DEF_OIP4		(0x00620)
#define NPC_AF_PCK_DEF_OIP6		(0x00630)
#define NPC_AF_PCK_DEF_IIP4		(0x00640)
#define NPC_AF_INTFX_HASHX_RESULT_CTRL(a, b)	(0x006c0 | (a) << 4 | (b) << 3)
#define NPC_AF_INTFX_HASHX_MASKX(a, b, c)  (0x00700 | (a) << 5 | (b) << 4 | (c) << 3)
#define NPC_AF_KEX_LDATAX_FLAGS_CFG(a)	(0x00800 | (a) << 3)
#define NPC_AF_INTFX_HASHX_CFG(a, b)  (0x00b00 | (a) << 6 | (b) << 4)
#define NPC_AF_INTFX_SECRET_KEY0(a)	(0x00e00 | (a) << 3)
#define NPC_AF_INTFX_SECRET_KEY1(a)	(0x00e20 | (a) << 3)
#define NPC_AF_INTFX_SECRET_KEY2(a)	(0x00e40 | (a) << 3)
#define NPC_AF_INTFX_KEX_CFG(a)		(0x01010 | (a) << 8)
#define NPC_AF_PKINDX_ACTION0(a)	(0x80000ull | (a) << 6)
#define NPC_AF_PKINDX_ACTION1(a)	(0x80008ull | (a) << 6)
#define NPC_AF_PKINDX_CPI_DEFX(a, b)	(0x80020ull | (a) << 6 | (b) << 3)
#define NPC_AF_KPUX_ENTRYX_CAMX(a, b, c) \
		(0x100000 | (a) << 14 | (b) << 6 | (c) << 3)
#define NPC_AF_KPUX_ENTRYX_ACTION0(a, b) \
		(0x100020 | (a) << 14 | (b) << 6)
#define NPC_AF_KPUX_ENTRYX_ACTION1(a, b) \
		(0x100028 | (a) << 14 | (b) << 6)
#define NPC_AF_KPUX_ENTRY_DISX(a, b)	(0x180000 | (a) << 6 | (b) << 3)
#define NPC_AF_CPIX_CFG(a)		(0x200000 | (a) << 3)
#define NPC_AF_INTFX_LIDX_LTX_LDX_CFG(a, b, c, d) \
		(0x900000 | (a) << 16 | (b) << 12 | (c) << 5 | (d) << 3)
#define NPC_AF_INTFX_LDATAX_FLAGSX_CFG(a, b, c) \
		(0x980000 | (a) << 16 | (b) << 12 | (c) << 3)
#define NPC_AF_INTFX_MISS_STAT_ACT(a)	(0x1880040 + (a) * 0x8)
#define NPC_AF_INTFX_MISS_ACT(a)	(0x1a00000 | (a) << 4)
#define NPC_AF_INTFX_MISS_TAG_ACT(a)	(0x1b00008 | (a) << 4)
#define NPC_AF_MCAM_BANKX_HITX(a, b)	(0x1c80000 | (a) << 8 | (b) << 4)
#define NPC_AF_LKUP_CTL			(0x2000000)
#define NPC_AF_LKUP_DATAX(a)		(0x2000200 | (a) << 4)
#define NPC_AF_LKUP_RESULTX(a)		(0x2000400 | (a) << 4)
#define NPC_AF_INTFX_STAT(a)		(0x2000800 | (a) << 4)
#define NPC_AF_DBG_CTL			(0x3000000)
#define NPC_AF_DBG_STATUS		(0x3000010)
#define NPC_AF_KPUX_DBG(a)		(0x3000020 | (a) << 8)
#define NPC_AF_IKPU_ERR_CTL		(0x3000080)
#define NPC_AF_KPUX_ERR_CTL(a)		(0x30000a0 | (a) << 8)
#define NPC_AF_MCAM_DBG			(0x3001000)
#define NPC_AF_DBG_DATAX(a)		(0x3001400 | (a) << 4)
#define NPC_AF_DBG_RESULTX(a)		(0x3001800 | (a) << 4)

#define NPC_AF_EXACT_MEM_ENTRY(a, b)	(0x300000 | (a) << 15 | (b) << 3)
#define NPC_AF_EXACT_CAM_ENTRY(a)	(0xC00 | (a) << 3)
#define NPC_AF_INTFX_EXACT_MASK(a)	(0x660 | (a) << 3)
#define NPC_AF_INTFX_EXACT_RESULT_CTL(a)(0x680 | (a) << 3)
#define NPC_AF_INTFX_EXACT_CFG(a)	(0xA00 | (a) << 3)
#define NPC_AF_INTFX_EXACT_SECRET0(a)	(0xE00 | (a) << 3)
#define NPC_AF_INTFX_EXACT_SECRET1(a)	(0xE20 | (a) << 3)
#define NPC_AF_INTFX_EXACT_SECRET2(a)	(0xE40 | (a) << 3)

#define NPC_AF_MCAMEX_BANKX_CAMX_INTF(a, b, c) ({			   \
	u64 offset;							   \
									   \
	offset = (0x1000000ull | (a) << 10 | (b) << 6 | (c) << 3);	   \
	if (rvu->hw->npc_ext_set)					   \
		offset = (0x8000000ull | (a) << 8 | (b) << 22 | (c) << 3); \
	offset; })

#define NPC_AF_MCAMEX_BANKX_CAMX_W0(a, b, c) ({				   \
	u64 offset;							   \
									   \
	offset = (0x1000010ull | (a) << 10 | (b) << 6 | (c) << 3);	   \
	if (rvu->hw->npc_ext_set)					   \
		offset = (0x8000010ull | (a) << 8 | (b) << 22 | (c) << 3); \
	offset; })

#define NPC_AF_MCAMEX_BANKX_CAMX_W1(a, b, c) ({				   \
	u64 offset;							   \
									   \
	offset = (0x1000020ull | (a) << 10 | (b) << 6 | (c) << 3);	   \
	if (rvu->hw->npc_ext_set)					   \
		offset = (0x8000020ull | (a) << 8 | (b) << 22 | (c) << 3); \
	offset; })

#define NPC_AF_MCAMEX_BANKX_CFG(a, b) ({				   \
	u64 offset;							   \
									   \
	offset = (0x1800000ull | (a) << 8 | (b) << 4);			   \
	if (rvu->hw->npc_ext_set)					   \
		offset = (0x8000038ull | (a) << 8 | (b) << 22);		   \
	offset; })

#define NPC_AF_MCAMEX_BANKX_ACTION(a, b) ({				   \
	u64 offset;							   \
									   \
	offset = (0x1900000ull | (a) << 8 | (b) << 4);			   \
	if (rvu->hw->npc_ext_set)					   \
		offset = (0x8000040ull | (a) << 8 | (b) << 22);		   \
	offset; })							   \

#define NPC_AF_MCAMEX_BANKX_TAG_ACT(a, b) ({				   \
	u64 offset;							   \
									   \
	offset = (0x1900008ull | (a) << 8 | (b) << 4);			   \
	if (rvu->hw->npc_ext_set)					   \
		offset = (0x8000048ull | (a) << 8 | (b) << 22);		   \
	offset; })							   \

#define NPC_AF_MCAMEX_BANKX_STAT_ACT(a, b) ({				   \
	u64 offset;							   \
									   \
	offset = (0x1880000ull | (a) << 8 | (b) << 4);			   \
	if (rvu->hw->npc_ext_set)					   \
		offset = (0x8000050ull | (a) << 8 | (b) << 22);		   \
	offset; })							   \

#define NPC_AF_MATCH_STATX(a) ({					   \
	u64 offset;							   \
									   \
	offset = (0x1880008ull | (a) << 8);				   \
	if (rvu->hw->npc_ext_set)					   \
		offset = (0x8000078ull | (a) << 8);			   \
	offset; })							   \

/* NDC */
#define NDC_AF_CONST			(0x00000)
#define NDC_AF_CLK_EN			(0x00020)
#define NDC_AF_CTL			(0x00030)
#define NDC_AF_BANK_CTL			(0x00040)
#define NDC_AF_BANK_CTL_DONE		(0x00048)
#define NDC_AF_INTR			(0x00058)
#define NDC_AF_INTR_W1S			(0x00060)
#define NDC_AF_INTR_ENA_W1S		(0x00068)
#define NDC_AF_INTR_ENA_W1C		(0x00070)
#define NDC_AF_ACTIVE_PC		(0x00078)
#define NDC_AF_CAMS_RD_INTERVAL		(0x00080)
#define NDC_AF_BP_TEST_ENABLE		(0x001F8)
#define NDC_AF_BP_TEST(a)		(0x00200 | (a) << 3)
#define NDC_AF_BLK_RST			(0x002F0)
#define NDC_PRIV_AF_INT_CFG		(0x002F8)
#define NDC_AF_HASHX(a)			(0x00300 | (a) << 3)
#define NDC_AF_PORTX_RTX_RWX_REQ_PC(a, b, c) \
		(0x00C00 | (a) << 5 | (b) << 4 | (c) << 3)
#define NDC_AF_PORTX_RTX_RWX_OSTDN_PC(a, b, c) \
		(0x00D00 | (a) << 5 | (b) << 4 | (c) << 3)
#define NDC_AF_PORTX_RTX_RWX_LAT_PC(a, b, c) \
		(0x00E00 | (a) << 5 | (b) << 4 | (c) << 3)
#define NDC_AF_PORTX_RTX_CANT_ALLOC_PC(a, b) \
		(0x00F00 | (a) << 5 | (b) << 4)
#define NDC_AF_BANKX_HIT_PC(a)		(0x01000 | (a) << 3)
#define NDC_AF_BANKX_MISS_PC(a)		(0x01100 | (a) << 3)
#define NDC_AF_BANKX_LINEX_METADATA(a, b) \
		(0x10000 | (a) << 12 | (b) << 3)

/* LBK */
#define LBK_CONST			(0x10ull)
#define LBK_LINK_CFG_P2X		(0x400ull)
#define LBK_LINK_CFG_X2P		(0x408ull)
#define LBK_CONST_CHANS			GENMASK_ULL(47, 32)
#define LBK_CONST_DST			GENMASK_ULL(31, 28)
#define LBK_CONST_SRC			GENMASK_ULL(27, 24)
#define LBK_CONST_BUF_SIZE		GENMASK_ULL(23, 0)
#define LBK_LINK_CFG_RANGE_MASK		GENMASK_ULL(19, 16)
#define LBK_LINK_CFG_ID_MASK		GENMASK_ULL(11, 6)
#define LBK_LINK_CFG_BASE_MASK		GENMASK_ULL(5, 0)

/* APR */
#define	APR_AF_LMT_CFG			(0x000ull)
#define	APR_AF_LMT_MAP_BASE		(0x008ull)
#define	APR_AF_LMT_CTL			(0x010ull)
#define APR_LMT_MAP_ENT_DIS_SCH_CMP_SHIFT	23
#define APR_LMT_MAP_ENT_SCH_ENA_SHIFT		22
#define APR_LMT_MAP_ENT_DIS_LINE_PREF_SHIFT	21
#define LMTST_THROTTLE_MASK		GENMASK_ULL(38, 35)
#define LMTST_WR_PEND_MAX		15

#endif /* RVU_REG_H */
