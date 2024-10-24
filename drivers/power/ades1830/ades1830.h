/***************************************************************************//**
 *   @file   ades1830.h
 *   @brief  Header file for the ADES1830 Driver
 *   @author Radu Sabau (radu.sabau@analog.com)
********************************************************************************
 * Copyright 2024(c) Analog Devices, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of Analog Devices, Inc. nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ANALOG DEVICES, INC. “AS IS” AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL ANALOG DEVICES, INC. BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/
#ifndef __ADES1830_H__
#define __ADES1830_H__

#include "no_os_spi.h"
#include "no_os_util.h"
#include "no_os_units.h"

#define ADES1830_WRCFGA_CMD			0x01
#define ADES1830_WRCFGB_CMD			0x24
#define ADES1830_RDCFGA_CMD			0x02
#define ADES1830_RDCFGB_CMD			0x26
#define ADES1830_RDCVA_CMD			0x04
#define ADES1830_RDCVB_CMD			0x06
#define ADES1830_RDCVC_CMD			0x08
#define ADES1830_RDCVD_CMD			0x0A
#define ADES1830_RDCVE_CMD			0x09
#define ADES1830_RDCVF_CMD			0x0B
#define ADES1830_RDCVALL_CMD			0x0C
#define ADES1830_RDACA_CMD			0x44
#define ADES1830_RDACB_CMD			0x46
#define ADES1830_RDACC_CMD			0x48
#define ADES1830_RDACD_CMD			0x4A
#define ADES1830_RDACE_CMD			0x49
#define ADES1830_RDACF_CMD			0x4B
#define ADES1830_RDACALL_CMD			0x4C
#define ADES1830_RDSVA_CMD			0x03
#define ADES1830_RDSVB_CMD			0x05
#define ADES1830_RDSVC_CMD			0x07
#define ADES1830_RDSVD_CMD			0x0D
#define ADES1830_RDSVE_CMD			0x0E
#define ADES1830_RDSVF_CMD			0x0F
#define ADES1830_RDSALL_CMD			0x10
#define ADES1830_RDCSALL_CMD			0x11
#define ADES1830_RDACSALL_CMD			0x51
#define ADES1830_RDFCA_CMD			0x12
#define ADES1830_RDFCB_CMD			0x13
#define ADES1830_RDFCC_CMD			0x14
#define ADES1830_RDFCD_CMD			0x15
#define ADES1830_RDFCE_CMD			0x16
#define ADES1830_RDFCF_CMD			0x17
#define ADES1830_RDFCALL_CMD			0x18
#define ADES1830_RDAUXA_CMD			0x19
#define ADES1830_RDAUXB_CMD			0x1A
#define ADES1830_RDAUXC_CMD			0x1B
#define ADES1830_RDAUXD_CMD			0x1F
#define ADES1830_RDRAXA_CMD			0x1C
#define ADES1830_RDRAXB_CMD			0x1D
#define ADES1830_RDRAXC_CMD			0x1E
#define ADES1830_RDRAXD_CMD			0x25
#define ADES1830_RDSTATA_CMD			0x30
#define ADES1830_RDSTATB_CMD			0x31
#define ADES1830_RDSTATC_CMD			0x32
#define ADES1830_RDSTATD_CMD			0x33
#define ADES1830_RDSTATE_CMD			0x34
#define ADES1830_RDASALL_CMD			0x35
#define ADES1830_WRPWMA_CMD			0x20
#define ADES1830_RDPWMA_CMD			0x22
#define ADES1830_WRPWMB_CMD			0x21
#define ADES1830_RDPWMB_CMD			0x23
#define ADES1830_CMDIS_CMD			0x40
#define ADES1830_CMEN_CMD			0x41
#define ADES1830_CMHB_CMD			0x43
#define ADES1830_WRCMCFG_CMD			0x58
#define ADES1830_RDCMCFG_CMD			0x59
#define ADES1830_WRCMCELLT_CMD			0x5A
#define ADES1830_RDCMCELLT_CMD			0x5B
#define ADES1830_WRCMGPIOT_CMD			0x5C
#define ADES1830_RDCMGPIOT_CMD			0x5D
#define ADES1830_CLRCMFLAG_CMD			0x5E
#define ADES1830_RDCMFLAG_CMD			0x5F

#define ADES1830_ADCV_BASE_CMD			0x260
#define ADES1830_ADSV_BASE_CMD			0x168
#define ADES1830_ADAX_BASE_CMD			0x410
#define ADES1830_ADAX2_BASE_CMD			0x100

#define ADES1830_MODE_RD_MASK			NO_OS_BIT(2)
#define ADES1830_MODE_DCP_MASK			NO_OS_BIT(1)
#define ADES1830_MODE_CONT_MASK			NO_OS_BIT(0)

#define ADES1830_RD_CMD_MASK			NO_OS_BIT(8)
#define ADES1830_CONT_CMD_MASK			NO_OS_BIT(7)
#define ADES1830_DCP_CMD_MASK			NO_OS_BIT(4)
#define ADES1830_RSTF_CMD_MASK			NO_OS_BIT(2)
#define ADES1830_OW_CMD_MASK			NO_OS_GENMASK(1, 0)
#define ADES1830_AUX_OW_CMD_MASK		NO_OS_BIT(8)
#define ADES1830_PUP_CMD_MASK			NO_OS_BIT(7)
#define ADES1830_CH_CMD_MASK			NO_OS_GENMASK(3, 0)
#define ADES1830_CH4_ADAX_MASK			NO_OS_BIT(4)
#define ADES1830_CH4_CMD_MASK			NO_OS_BIT(6)

#define ADES1830_CLRCELL_CMD			0x711
#define ADES1830_CLRFC_CMD			0x714
#define ADES1830_CLRAUX_CMD			0x712
#define ADES1830_CLRSPIN_CMD			0x716
#define ADES1830_CLRFLAG_CMD			0x717
#define ADES1830_CLOVUV_CMD			0x715
#define ADES1830_PLADC_CMD			0x718
#define ADES1830_PLCADC_CMD			0x71C
#define ADES1830_PLSADC_CMD			0x71D
#define ADES1830_PLAUX_CMD			0x71E
#define ADES1830_PLAUX2_CMD			0x71F
#define ADES1830_WRCOMM_CMD			0x721
#define ADES1830_RDCOMM_CMD			0x722
#define ADES1830_STCOMM_CMD			0x723
#define ADES1830_MUTE_CMD			0x28
#define ADES1830_UNMUTE_CMD			0x29
#define ADES1830_RDSID_CMD			0x2C
#define ADES1830_RSTCC_CCMDD			0x2E
#define ADES1830_SNAP_CMD			0x2D
#define ADES1830_UNSNAP_CMD			0x2F
#define ADES1830_SRST_CMD			0x27
#define ADES1830_ULRR_CMD			0x38
#define ADES1830_WRRR_CMD			0x39
#define ADES1830_RDRR_CMD			0x3A

#define ADES1830_MSB_MASK			NO_OS_GENMASK(15, 8)
#define ADES1830_LSB_MASK			NO_OS_GENMASK(7, 0)

#define ADES1830_DATA0_MASK			NO_OS_GENMASK(7, 0)
#define ADES1830_DATA1_MASK			NO_OS_GENMASK(15, 8)
#define ADES1830_DATA2_MASK			NO_OS_GENMASK(23, 16)
#define ADES1830_DATA3_MASK			NO_OS_GENMASK(31, 24)
#define ADES1830_DATA4_MASK			NO_OS_GENMASK(39, 32)
#define ADES1830_DATA5_MASK			NO_OS_GENMASK(47, 40)

#define ADES1830_CMD_MSB_MASK			NO_OS_GENMASK(10, 8)
#define ADES1830_CMD_LSB_MASK			NO_OS_GENMASK(7, 0)

#define ADES1830_PEC15_MSB_MASK			NO_OS_GENMASK(14, 7)
#define ADES1830_PEC15_LSB_MASK			NO_OS_GENMASK(6, 0)
#define ADES1830_PEC15_PEC1_MASK		NO_OS_GENMASK(7, 1)

#define ADES1830_PEC10_MSB_MASK			NO_OS_GENMASK(9, 8)
#define ADES1830_PEC10_LSB_MASK			NO_OS_GENMASK(7, 0)

#define ADES1830_CTH_MASK			NO_OS_GENMASK(2, 0)
#define ADES1830_SOAKON_MASK			NO_OS_BIT(7)
#define ADES1830_OWRNG_MASK			NO_OS_BIT(6)
#define ADES1830_OWA_MASK			NO_OS_GENMASK(5, 3) /* Will be used for soak time set. Later used in UTIL API. */
#define ADES1830_GPOX_MASK(offset)		NO_OS_BIT(offset - 1)
#define ADES1830_FC_MASK			NO_OS_GENMASK(2, 0)
#define ADES1830_COMM_BK_MASK			NO_OS_BIT(3)
#define ADES1830_MUTE_ST_MASK			NO_OS_BIT(4)
#define ADES1830_SNAP_ST_MASK			NO_OS_BIT(5)

#define ADES1830_DTMEN_MASK			NO_OS_BIT(7)
#define ADES1830_DTRNG_MASK			NO_OS_BIT(6)
#define ADES1830_DCTO_MASK			NO_OS_GENMASK(5, 0)
#define ADES1830_DISCHARGE_MASK			NO_OS_GENMASK(31, 24)

#define ADES1830_CSXFLT_MASK(offset)		NO_OS_BIT(offset)
#define ADES1830_CTMSB_MASK			NO_OS_GENMASK(4, 0)
#define ADES1830_CTLSB_MASK			ADES1830_LSB_MASK
#define ADES1830_VA_OV_MASK			NO_OS_BIT(7)
#define ADES1830_VA_UV_MASK			NO_OS_BIT(6)
#define ADES1830_VD_OV_MASK			NO_OS_BIT(5)
#define ADES1830_VD_UV_MASK			NO_OS_BIT(4)
#define ADES1830_CED_MASK			NO_OS_BIT(3)
#define ADES1830_CMED_MASK			NO_OS_BIT(2)
#define ADES1830_SED_MASK			NO_OS_BIT(1)
#define ADES1830_SMED_MASK			NO_OS_BIT(0)
#define ADES1830_VDEL_MASK			NO_OS_BIT(7)
#define ADES1830_VDE_MASK			NO_OS_BIT(6)
#define ADES1830_COMP_MASK			NO_OS_BIT(5)
#define ADES1830_SPIFLT_MASK			NO_OS_BIT(4)
#define ADES1830_SLEEP_MASK			NO_OS_BIT(3)
#define ADES1830_THSD_MASK			NO_OS_BIT(2)
#define ADES1830_TMODCHK_MASK			NO_OS_BIT(1)
#define ADES1830_OSCCHK_MASK			NO_OS_BIT(0)

#define ADES1830_CxUV_MASK(offset)		NO_OS_BIT(((offset - 1) % 4) * 2)
#define ADES1830_CxOV_MASK(offset)		NO_OS_BIT((((offset - 1) % 4) * 2) + 1)
#define ADES1830_GPX_MASK(offset)		NO_OS_BIT((offset - 1) % 8)
#define ADES1830_REV_MASK			NO_OS_GENMASK(7, 4)
#define ADES1830_PWMX_MASK(offset)		((offset % 2) ? NO_OS_GENMASK(7, 4) : NO_OS_GENMASK(3, 0))

#define ADES1830_UV_THR_MASK			NO_OS_GENMASK(11, 0)
#define ADES1830_OV_THR_MASK			NO_OS_GENMASK(23, 12)
#define ADES1830_DV_THR_MASK			NO_OS_GENMASK(35, 24)

#define ADES1830_LPCM_MONITOR_MASK		NO_OS_BIT(7)
#define ADES1830_LPCM_NO_DEV_MASK		NO_OS_GENMASK(15, 8)

#define ADES1830_HB_PER_MASK			NO_OS_GENMASK(6, 4)
#define ADES1830_BRDGL_TO_MASK			NO_OS_BIT(3)
#define ADES1830_BRDGL_PER_MASK			NO_OS_GENMASK(2, 0)
#define ADES1830_TX_DIR_MASK			NO_OS_BIT(37)
#define ADES1830_GPIO_INT_MASK			NO_OS_GENMASK(36, 34)
#define ADES1830_CELL_MASK			NO_OS_GENMASK(33, 16)
#define ADES1830_GPIO_MASK			NO_OS_GENMASK(47, 38)

#define ADES1830_ICOM_MASK			NO_OS_GENMASK(7, 4)
#define ADES1830_FCOM_MASK			NO_OS_GENMASK(3, 0)

#define ADES1830_REFON_MASK			NO_OS_BIT(7)

#define ADES1830_FLAGD_VAL(offset)		NO_OS_BIT(offset)
#define ADES1830_CLR_CM_FLAG_VAL		0xFFFF
#define ADES1830_INIT_DISCHARGE_VAL_16MIN	0x0F
#define ADES1830_REMOVE_DISCHARGE_VAL_16MIN	0x00

enum ades1830_chan_adax {
	ADES1830_ALL_ADAX,
	ADES1830_GPIO1_ADAX,
	ADES1830_GPIO2_ADAX,
	ADES1830_GPIO3_ADAX,
	ADES1830_GPIO4_ADAX,
	ADES1830_GPIO5_ADAX,
	ADES1830_GPIO6_ADAX,
	ADES1830_GPIO7_ADAX,
	ADES1830_GPIO8_ADAX,
	ADES1830_GPIO9_ADAX,
	ADES1830_VREF2_ADAX = 0x10,
	ADES1830_VD_ADAX,
	ADES1830_ITEMP_ADAX,
	ADES1830_VPV_ADAX,
	ADES1830_VMV_ADAX,
	ADES1830_RES_ADAX,
};

enum ades1830_group_sel {
	ADES1830_GROUP_A,
	ADES1830_GROUP_B,
	ADES1830_GROUP_C,
	ADES1830_GROUP_D,
};

enum ades1830_cont_sel {
	ADES1830_SINGLE_TO_STANDBY,
	ADES1830_CONT_MEAS,
};

enum ades1830_ow_sel {
	ADES1830_ALL_OFF_OW,
	ADES1830_EVEN_ON_OW,
	ADES1830_ODD_ON_OW,
	ADES1830_ALL_ON_OW,
};

enum ades1830_ow_aux_sel {
	ADES1830_OFF_AUXOW,
	ADES1830_ON_AUXOW,
};

enum ades1830_pup_sel {
	ADES1830_PD_AUX_CONV,
	ADES1830_PU_AUX_CONV,
};

enum ade1830_discharge_sel {
	ADES1830_NOPERMIT_DISCHARGE,
	ADES1830_PERMIT_DISCHARGE,
};

enum ades1830_reset_iir_sel {
	ADES1830_NO_RESET_IIR,
	ADES1830_RESET_IIR,
};

enum ades1830_err_sel {
	ADES1830_RD_STAT_REG_C_NO_ERROR,
	ADES1830_RD_STAT_REG_C_WITH_ERROR,
};

enum ades1830_cth_val {
	ADES1830_5100UV_CTH,
	ADES1830_8100UV_CTH,
	ADES1830_9000UV_CTH,
	ADES1830_10500UV_CTH,
	ADES1830_15000UV_CTH,
	ADES1830_19950UV_CTH,
	ADES1830_25050UV_CTH,
	ADES1830_40050UV_CTH,
};

enum ades1830_fc_iir_val {
	ADES1830_FILTER_DISABLED,
	ADES1830_110_FC_IIR,
	ADES1830_45_FC_IIR,
	ADES1830_21_FC_IIR,
	ADES1830_10_FC_IIR,
	ADES1830_5_FC_IIR,
	ADES1830_1_25_FC_IIR,
	ADES1830_0_625_FC_IIR,
};

enum ades1830_icom_sel {
	ADES1830_SPI_CSB_LOW_ICOM = 0x08,
	ADES1830_SPI_CSB_HIGH_ICOM = 0x09,
	ADES1830_SPI_CSB_FALLING_ICOM = 0x0A,
	ADES1830_SPI_NO_TRANSMIT_ICOM = 0x0F,
};

enum ades1830_fcom_spi_read_sel {
	ADES1830_SPI_CSB_LOW_RD_FCOM,
	ADES1830_SPI_CSB_HIGH_RD_FCOM = 0x09,
};

enum ades1830_adcv_meas_mode {
	ADES1830_CADC_SINGLE,
	ADES1830_STOP_CADC_GO_CONT,
	ADES1830_STOP_CADC_GO_SINGLE_CADC,
	ADES1830_PWM_INTERRUPT_GO_SINGLE_COMPARE = 4,
	ADES1830_PWM_INTERRUPT_GO_CONT,
	ADES1830_STOP_CADC_GO_SINGLE_BOTH,
};

enum ades1830_adsv_meas_mode {
	ADES1830_PWM_INTERRUPT_SINGLE_SADC,
	ADES1830_SADC_SYNC_CADC_CONT,
	ADES1830_STOP_CADC_GO_SINGLE_CADC,
	ADES1830_NO_PWM_INTERRUPT_SINGLE_SADC,
};

enum ades1830_clr_adc_mem {
	ADES1830_CLR_CELL_VOLTAGE = ADES1830_CLRCELL_CMD,
	ADES1830_CLR_FILTERED_CELL_VOLTAGE = ADES1830_CLRFC_CMD,
	ADES1830_CLR_SVOLTAGE = ADES1830_CLRSPIN_CMD,
	ADES1830_CLR_AUX_AUX2_STATUS = ADES1830_CLRAUX_CMD,
};

enum ades1830_flt_flag {
	ADES1830_CL_CS1_CS9_FLT = NO_OS_BIT(0),
	ADES1830_CL_CS2_CS10_FLT = NO_OS_BIT(1),
	ADES1830_CL_CS3_CS11_FLT = NO_OS_BIT(2),
	ADES1830_CL_CS4_CS12_FLT = NO_OS_BIT(3),
	ADES1830_CL_CS5_CS13_FLT = NO_OS_BIT(4),
	ADES1830_CL_CS6_CS14_FLT = NO_OS_BIT(5),
	ADES1830_CL_CS7_CS15_FLT = NO_OS_BIT(6),
	ADES1830_CL_CS8_CS16_FLT = NO_OS_BIT(7),
	ADES1830_CL_SMED = NO_OS_BIT(32),
	ADES1830_CL_SED = NO_OS_BIT(33),
	ADES1830_CL_CMED = NO_OS_BIT(34),
	ADES1830_CL_CED = NO_OS_BIT(35),
	ADES1830_CL_VDUV = NO_OS_BIT(36),
	ADES1830_CL_VDOV = NO_OS_BIT(37),
	ADES1730_CL_VAUV = NO_OS_BIT(38),
	ADES1830_CL_VAOV = NO_OS_BIT(39),
	ADES1830_CL_OSCCHK = NO_OS_BIT(40),
	ADES1830_CL_TMODE = NO_OS_BIT(41),
	ADES1830_CL_THSD = NO_OS_BIT(42),
	ADES1830_CL_SLEEP = NO_OS_BIT(43),
	ADES1830_CL_SPIFLT = NO_OS_BIT(44),
	ADES1830_CL_VDE = NO_OS_BIT(46),
	ADES1830_CL_VDEL = NO_OS_BIT(47),
};

enum ades1830_pwm_discharge_duty {
	ADES1830_PWM_6_25_DUTY,
	ADES1830_PWM_12_50_DUTY,
	ADES1830_PWM_18_75_DUTY,
	ADES1830_PWM_25_DUTY,
	ADES1830_PWM_31_25_DUTY,
	ADES1830_PWM_37_5_DUTY,
	ADES1830_PWM_43_75_DUTY,
	ADES1830_PWM_50_DUTY,
	ADES1830_PWM_56_25_DUTY,
	ADES1830_PWM_62_5_DUTY,
	ADES1830_PWM_68_75_DUTY,
	ADES1830_PWM_75_DUTY,
	ADES1830_PWM_81_25_DUTY,
	ADES1830_PWM_87_5_DUTY,
	ADES1830_PWM_93_75_DUTY,
	ADES1830_PWM_100_DUTY,
};

enum ades1830_discharge_timeout {
	ADES1830_DISCHARGE_MIN_RANGE,
	ADES1830_DISCHARGE_HR_RANGE,
};

enum ades1830_lpcm_hb_period {
	ADES1830_1S_HB_PERIOD,
	ADES1830_2S_HB_PERIOD,
	ADES1830_4S_HB_PERIOD,
	ADES1830_8S_HB_PERIOD,
	ADES1830_12S_HB_PERIOD,
	ADES1830_16S_HB_PERIOD,
	ADES1830_32S_HB_PERIOD,
};

enum ades1830_lpcm_bridgeless_per {
	ADES1830_1_5S_BTIMEOUT_PERIOD,
	ADES1830_3S_BTIMEOUT_PERIOD,
	ADES1830_6S_BTIMEOUT_PERIOD,
	ADES1830_12S_BTIMEOUT_PERIOD,
	ADES1830_18S_BTIMEOUT_PERIOD,
	ADES1830_24S_BTIMEOUT_PERIOD,
	ADES1830_48S_BTIMEOUT_PERIOD,
};

enum ades1830_lpcm_tx_dir {
	ADES1830_CMHB_OUT_PORT_B,
	ADES1830_CMHB_OUT_PORT_A,
};

enum ades1830_lcpm_gpio_interrupt {
	ADES1830_NO_GPIO_OUTPUT,
	ADES1830_GPIO3_ACTIVE_LOW,
	ADES1830_GPIO3_ACTIVE_HIGH,
	ADES1830_GPIO4_ACTIVE_LOW,
	ADES1830_GPIO4_ACTIVE_HIGH,
	ADES1830_GPIO4_LOW_GPIO3_LOW,
	ADES1830_GPIO4_LOW_GPIO3_HIGH,
	ADES1830_GPIO4_HIGH_GPIO3_LOW,
};

enum ades1830_thr_type {
	ADES1830_CELL_UV_THR,
	ADES1830_CELL_OV_THR,
	ADES1830_CELL_DV_THR,
	ADES1830_GPIO_UV_THR,
	ADES1830_GPIO_OV_THR,
	ADES1830_GPIO_DV_THR,
};

struct ades1830_init_param {
	struct no_os_spi_init_param *spi_param;

	enum ades1830_ow_sel ow_sel;
	uint8_t no_devices;
};

struct ades1830_desc {
	struct no_os_spi_descriptor *spi_desc;

	enum ades1830_ow_sel ow_sel;
	uint8_t no_devices;
};

/** COMM API. */
int ades1830_cmd_send(struct ades1830_desc *desc, uint16_t cmd);

int ades1830_cmd_write(struct ades1830_desc *desc, uint16_t cmd,
		       uint64_t *data);

int ades1830_cmd_read(struct ades1830_desc *desc, uint16_t cmd, uint64_t *data);

int ades1830_read_cadc(struct ades1830_desc *desc, uint64_t *cadc_meas,
		       enum ades1830_adcv_meas_mode mode);

int ades1830_read_sadc(struct ades1830_desc *desc, uint64_t *sadc_meas,
		       enum ades1830_adsv_meas_mode mode);

int ades1830_set_iir(struct ades1830_desc *desc, enum ades1830_fc_iir_val iir);

int ades1830_get_iir(struct ades1830_desc *desc, enum ades1830_fc_iir_val *iir);

int ades1830_read_aux_adc(struct ades1830_desc *desc, uint64_t *aux, bool pup,
			  enum ades1830_group_sel group,
			  enum ades1830_chan_adax adax);

int ades1830_read_aux2_adc(struct ades1830_desc *desc, uint64_t *aux,
			   enum ades1830_group_sel group,
			   enum ades1830_chan_adax adax);

int ades1830_set_pwm_discharge(struct ades1830_desc *desc, uint8_t cell_nb,
			       enum ades1830_pwm_discharge_duty duty);

int ades1830_set_discharge(struct ades1830_desc *desc, bool enable,
			   enum ades1830_discharge_timeout timeout,
			   uint8_t timeout_val);

int ades1830_get_discharge(struct ades1830_desc *desc, bool *enable,
			   enum ades1830_discharge_timeout *timeout,
			   uint8_t *timeout_val);

int ades1830_start_lpcm(struct ades1830_desc *desc, bool monitor);

int ades1830_set_lpcm_hb_period(struct ades1830_desc *desc,
				enum ades1830_lpcm_hb_period hb_per);

int ades1830_get_lpcm_hb_period(struct ades1830_desc *desc,
				enum ades1830_lpcm_hb_period *hb_per);

int ades1830_set_lpcm_bridge(struct ades1830_desc *desc, bool bridgeless_to,
			     enum ades1830_lpcm_bridgeless_per brdgl_per);

int ades1830_get_lpcm_bridge(struct ades1830_desc *desc, bool *bridgeless_to,
			     enum ades1830_lpcm_bridgeless_per *brdgl_per);

int ades1830_set_lpcm_tx(struct ades1830_desc *desc,
			 enum ades1830_lpcm_tx_dir tx_dir);

int ades1830_get_lpcm_tx(struct ades1830_desc *desc,
			 enum ades1830_lpcm_tx_dir *tx_dir);

int ades1830_set_lpcm_gpio(struct ades1830_desc *desc,
			   enum ades1830_lcpm_gpio_interrupt gpio_int);

int ades1830_get_lpcm_gpio(struct ades1830_desc *desc,
			   enum ades1830_lcpm_gpio_interrupt *gpio_int);

int ades1830_set_lpcm_cell_mask(struct ades1830_desc *desc,
				uint32_t cell_mask);

int ades1830_get_lpcm_cell_mask(struct ades1830_desc *desc,
				uint32_t *cell_mask);

int ades1830_set_lpcm_gpio_mask(struct ades1830_desc *desc,
				uint16_t gpio_mask);

int ades1830_get_lpcm_gpio_mask(struct ades1830_desc *desc,
				uint16_t *gpio_mask);

int ades1830_set_lpcm_thr(struct ades1830_desc *desc, int32_t v_thr_mv,
			  enum ades1830_thr_type thr_type);

int ades1830_get_lpcm_thr(struct ades1830_desc *desc, int32_t *v_thr_mv,
			  enum ades1830_thr_type thr_type);

int ades1830_clear_adc_mem(struct ades1830_desc *desc,
			   enum ades1830_clr_adc_mem clr_adc);

int ades1830_clear_flag(struct ades1830_desc *desc, enum ades1830_flt_flag flt);

int ades1830_clear_ov_and_uv(struct ades1830_desc *desc, uint8_t cell_nb,
			     bool uv);

int ades1830_stop_lpcm(struct ades1830_desc *desc);

int ades1830_clear_cm_flag(struct ades1830_desc *desc);

int ades1830_soft_reset(struct ades1830_desc *desc);

int ades1830_init(struct ades1830_desc **desc,
		  struct ades1830_init_param *init_param);

int ades1830_remove(struct ades1830_desc *desc);

#endif /* __ADES1830_H__ */
