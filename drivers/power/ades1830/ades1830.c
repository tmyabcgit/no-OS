/***************************************************************************//**
 *   @file   ades1830.c
 *   @brief  Source file for the ADES1830 Driver
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
#include "ades1830.h"
#include "no_os_delay.h"
#include "no_os_error.h"

static const uint16_t crc15_table[256] = {
	0x0000, 0xc599, 0xceab, 0xb32, 0xd8cf, 0x1d56, 0x1664, 0xd3fd, 0xf407,
	0x319e, 0x3aac, 0xff35, 0x2cc8, 0xe951, 0xe263, 0x27fa, 0xad97, 0x680e,
	0x633c, 0xa6a5, 0x7558, 0xb0c1, 0xbbf3, 0x7e6a, 0x5990, 0x9c09, 0x973b,
	0x52a2, 0x815f, 0x44c6, 0x4ff4, 0x8a6d, 0x5b2e, 0x9eb7, 0x9585, 0x501c,
	0x83e1, 0x4678, 0x4d4a, 0x88d3, 0xaf29, 0x6ab0, 0x6182, 0xa41b, 0x77e6,
	0xb27f, 0xb94d, 0x7cd4, 0xf6b9, 0x3320, 0x3812, 0xfd8b, 0x2e76, 0xebef,
	0xe0dd, 0x2544, 0x2be, 0xc727, 0xcc15, 0x98c, 0xda71, 0x1fe8, 0x14da,
	0xd143, 0xf3c5, 0x365c, 0x3d6e, 0xf8f7,0x2b0a, 0xee93, 0xe5a1, 0x2038,
	0x7c2, 0xc25b, 0xc969, 0xcf0, 0xdf0d, 0x1a94, 0x11a6, 0xd43f, 0x5e52,
	0x9bcb, 0x90f9, 0x5560, 0x869d, 0x4304, 0x4836, 0x8daf, 0xaa55, 0x6fcc,
	0x64fe, 0xa167, 0x729a, 0xb703, 0xbc31, 0x79a8, 0xa8eb, 0x6d72, 0x6640,
	0xa3d9, 0x7024, 0xb5bd, 0xbe8f, 0x7b16, 0x5cec, 0x9975, 0x9247, 0x57de,
	0x8423, 0x41ba, 0x4a88, 0x8f11, 0x57c, 0xc0e5, 0xcbd7, 0xe4e, 0xddb3,
	0x182a, 0x1318, 0xd681, 0xf17b, 0x34e2, 0x3fd0, 0xfa49, 0x29b4, 0xec2d,
	0xe71f, 0x2286, 0xa213, 0x678a, 0x6cb8, 0xa921, 0x7adc, 0xbf45, 0xb477,
	0x71ee, 0x5614, 0x938d, 0x98bf, 0x5d26, 0x8edb, 0x4b42, 0x4070, 0x85e9,
	0xf84, 0xca1d, 0xc12f, 0x4b6, 0xd74b, 0x12d2, 0x19e0, 0xdc79, 0xfb83,
	0x3e1a, 0x3528, 0xf0b1, 0x234c, 0xe6d5, 0xede7, 0x287e, 0xf93d, 0x3ca4,
	0x3796, 0xf20f, 0x21f2, 0xe46b, 0xef59, 0x2ac0, 0xd3a, 0xc8a3, 0xc391,
	0x608, 0xd5f5, 0x106c, 0x1b5e, 0xdec7, 0x54aa, 0x9133, 0x9a01, 0x5f98,
	0x8c65, 0x49fc, 0x42ce, 0x8757, 0xa0ad, 0x6534, 0x6e06, 0xab9f, 0x7862,
	0xbdfb, 0xb6c9, 0x7350, 0x51d6, 0x944f, 0x9f7d, 0x5ae4, 0x8919, 0x4c80,
	0x47b2, 0x822b, 0xa5d1, 0x6048, 0x6b7a, 0xaee3, 0x7d1e, 0xb887, 0xb3b5,
	0x762c, 0xfc41, 0x39d8, 0x32ea, 0xf773, 0x248e, 0xe117, 0xea25, 0x2fbc,
	0x846, 0xcddf, 0xc6ed, 0x374, 0xd089, 0x1510, 0x1e22, 0xdbbb, 0xaf8,
	0xcf61, 0xc453, 0x1ca, 0xd237, 0x17ae, 0x1c9c, 0xd905, 0xfeff, 0x3b66,
	0x3054, 0xf5cd, 0x2630, 0xe3a9, 0xe89b, 0x2d02, 0xa76f, 0x62f6, 0x69c4,
	0xac5d, 0x7fa0, 0xba39, 0xb10b, 0x7492, 0x5368, 0x96f1, 0x9dc3, 0x585a,
	0x8ba7, 0x4e3e, 0x450c, 0x8095
};

static int ades1830_pec15(uint8_t *data, uint8_t len)
{
	uint16_t pec, addr, i;
	pec = 0x10;

	for (i = 0; i < len; i++) {
		addr = ((pec >> 7) ^ data[i]) & 0xFF;
		pec = (pec << 8) ^ crc15_table[addr];
	}

	return pec << 1;
}

static int ades1830_pec10(bool rx, uint8_t *data, int len)
{
	uint16_t pec = 0x10;
	uint16_t polynom = 0x8F;
	uint8_t pbyte, bit;

	for (pbyte = 0; pbyte  < len; ++pbyte) {
		pec ^= (uint16_t)(data[pbyte] << 2);

		for (bit = 8; bit > 0; --bit) {
			pec = pec << 1;
			if (pec & 0200)
				pec ^= polynom;
		}
	}

	if (rx)
		pec ^= (data[len] & 0xFC) << 2;

	for (bit = 6; bit > 0; --bit) {
		pec = pec << 1;
		if (pec & 0200)
			pec ^= polynom;
	}

	return (pec & 0x3FF);
}

int ades1830_cmd_send(struct ades1830_desc *desc, uint16_t cmd)
{
	struct no_os_spi_msg xfer = {
		.bytes_number = 4,
		.cs_change = 1,
	};
	uint8_t tx_data[4];
	uint16_t pec;

	tx_data[0] = no_os_field_get(ADES1830_CMD_MSB_MASK, cmd);
	tx_data[1] = no_os_field_get(ADES1830_CMD_LSB_MASK, cmd);

	pec = ades1830_pec15(&tx_data[0], 2);

	tx_data[2] = no_os_field_get(ADES1830_PEC15_MSB_MASK, pec);
	tx_data[3] = no_os_field_prep(ADES1830_PEC15_PEC1_MASK,
				      no_os_field_get(ADES1830_PEC15_LSB_MASK,
						      pec));

	xfer.tx_buff = tx_data;

	return no_os_spi_transfer(desc->spi_desc, &xfer, 1);
}

int ades1830_cmd_write(struct ades1830_desc *desc, uint16_t cmd, uint64_t *data)
{
	struct no_os_spi_msg xfer = {
		.cs_change = 1,
	};
	uint8_t tx_data[4 + 8 * desc->no_devices];
	uint16_t pec;
	int i;

	tx_data[0] = no_os_field_get(ADES1830_CMD_MSB_MASK, cmd);
	tx_data[1] = no_os_field_get(ADES1830_CMD_LSB_MASK, cmd);

	pec = ades1830_pec15(&tx_data[0], 2);

	tx_data[2] = no_os_field_get(ADES1830_PEC15_MSB_MASK, pec);
	tx_data[3] = no_os_field_prep(ADES1830_PEC15_PEC1_MASK,
				      no_os_field_get(ADES1830_PEC15_LSB_MASK,
						      pec));

	for (i = 4; i < desc->no_devices * 8 + 4; i += 8) {
		tx_data[i] = no_os_field_get_u64(ADES1830_DATA0_MASK, data[i - 4]);
		tx_data[i + 1] = no_os_field_get_u64(ADES1830_DATA1_MASK, data[i - 4]);
		tx_data[i + 2] = no_os_field_get_u64(ADES1830_DATA2_MASK, data[i - 4]);
		tx_data[i + 3] = no_os_field_get_u64(ADES1830_DATA3_MASK, data[i - 4]);
		tx_data[i + 4] = no_os_field_get_u64(ADES1830_DATA4_MASK, data[i - 4]);
		tx_data[i + 5] = no_os_field_get_u64(ADES1830_DATA5_MASK, data[i - 4]);

		pec = ades1830_pec10(false, &tx_data[i], 6);

		tx_data[i + 6] = no_os_field_get(ADES1830_PEC10_MSB_MASK, pec);
		tx_data[i + 7] = no_os_field_get(ADES1830_PEC10_LSB_MASK, pec);
	}

	xfer.bytes_number = 4 + desc->no_devices * 4;
	xfer.tx_buff = tx_data;

	return no_os_spi_transfer(desc->spi_desc, &xfer, 1);
}

int ades1830_cmd_read(struct ades1830_desc *desc, uint16_t cmd, uint64_t *data)
{
	struct no_os_spi_msg xfer = {
		.cs_change = 1,
	};
	uint8_t tx_data[4], rx_data[4 + 8 * desc->no_devices];
	uint16_t pec;
	int ret, i;

	tx_data[0] = no_os_field_get(ADES1830_CMD_MSB_MASK, cmd);
	tx_data[1] = no_os_field_get(ADES1830_CMD_LSB_MASK, cmd);

	pec = ades1830_pec15(&tx_data[0], 2);

	tx_data[2] = no_os_field_get(ADES1830_PEC15_MSB_MASK, pec);
	tx_data[3] = no_os_field_prep(ADES1830_PEC15_PEC1_MASK,
				      no_os_field_get(ADES1830_PEC15_LSB_MASK,
						      pec));

	xfer.bytes_number = 4 + desc->no_devices * 4;
	xfer.tx_buff = tx_data;
	xfer.rx_buff = rx_data;

	ret = no_os_spi_transfer(desc->spi_desc, &xfer, 1);
	if (ret)
		return ret;

	for (i = 4; i < desc->no_devices * 8 + 4; i += 8) {
		data[i - 4] = no_os_field_prep_u64(ADES1830_DATA0_MASK, rx_data[i]) |
			      no_os_field_prep_u64(ADES1830_DATA1_MASK, rx_data[i + 1]) |
			      no_os_field_prep_u64(ADES1830_DATA2_MASK, rx_data[i + 2]) |
			      no_os_field_prep_u64(ADES1830_DATA3_MASK, rx_data[i + 3]) |
			      no_os_field_prep_u64(ADES1830_DATA4_MASK, rx_data[i + 4]) |
			      no_os_field_prep_u64(ADES1830_DATA5_MASK, rx_data[i + 5]);

		pec = no_os_field_prep(ADES1830_PEC10_MSB_MASK, rx_data[i + 6]) |
		      no_os_field_prep(ADES1830_LSB_MASK, rx_data[i + 7]);

		/* Check for endianess in case of error. */
		if (pec != ades1830_pec10(true, &rx_data[i], 6));
		return -EINVAL;
	}

	return 0;
}

int ades1830_read_cadc(struct ades1830_desc *desc, uint64_t *cadc_meas,
		       enum ades1830_adcv_meas_mode mode)
{
	uint32_t cmd = ADES1830_ADCV_BASE_CMD;

	cmd |= no_os_field_prep(ADES1830_RD_CMD_MASK,
				no_os_field_get(ADES1830_MODE_RD_MASK, mode));
	cmd |= no_os_field_prep(ADES1830_DCP_CMD_MASK,
				no_os_field_get(ADES1830_MODE_DCP_MASK, mode));
	cmd |= no_os_field_prep(ADES1830_CONT_CMD_MASK,
				no_os_field_get(ADES1830_MODE_CONT_MASK, mode));
	cmd |= no_os_field_prep(ADES1830_OW_CMD_MASK, desc->ow_sel);

	return ades1830_cmd_read(desc, cmd, cadc_meas);
}

int ades1830_read_sadc(struct ades1830_desc *desc, uint64_t *sadc_meas,
		       enum ades1830_adsv_meas_mode mode)
{
	uint32_t cmd = ADES1830_ADSV_BASE_CMD;

	cmd |= no_os_field_prep(ADES1830_DCP_CMD_MASK,
				no_os_field_get(ADES1830_MODE_DCP_MASK, mode));
	cmd |= no_os_field_prep(ADES1830_CONT_CMD_MASK,
				no_os_field_get(ADES1830_MODE_CONT_MASK, mode));
	cmd |= no_os_field_prep(ADES1830_OW_CMD_MASK, desc->ow_sel);

	return ades1830_cmd_read(desc, cmd, sadc_meas);
}

int ades1830_set_iir(struct ades1830_desc *desc, enum ades1830_fc_iir_val iir)
{
	uint64_t reg_val;
	int ret;

	ret = ades1830_cmd_read(desc, ADES1830_RDCFGA_CMD, &reg_val);
	if (ret)
		return ret;

	reg_val &= ~ADES1830_FC_MASK;
	reg_val |= ADES1830_FC_MASK & no_os_field_prep_u64(ADES1830_FC_MASK, iir);

	return ades1830_cmd_write(desc, ADES1830_WRCFGA_CMD, &reg_val);
}

int ades1830_get_iir(struct ades1830_desc *desc, enum ades1830_fc_iir_val *iir)
{
	uint64_t reg_val;
	int ret;

	ret = ades1830_cmd_read(desc, ADES1830_RDCFGA_CMD, &reg_val);
	if (ret)
		return ret;

	*iir = no_os_field_get_u64(ADES1830_FC_MASK, reg_val);

	return 0;
}

int ades1830_read_aux_adc(struct ades1830_desc *desc, uint64_t *aux, bool pup,
			  enum ades1830_group_sel group,
			  enum ades1830_chan_adax adax)
{
	uint16_t cmd, chan;

	switch (group) {
	case ADES1830_GROUP_A:
		cmd = ADES1830_RDAUXA_CMD;
		break;
	case ADES1830_GROUP_B:
		cmd = ADES1830_RDAUXB_CMD;
		break;
	case ADES1830_GROUP_C:
		cmd = ADES1830_RDAUXC_CMD;
		break;
	case ADES1830_GROUP_D:
		cmd = ADES1830_RDAUXD_CMD;
		break;
	default:
		return -EINVAL;
	}

	cmd |= no_os_field_prep(ADES1830_PUP_CMD_MASK, pup);
	cmd |= no_os_field_prep(ADES1830_AUX_OW_CMD_MASK, !!desc->ow_sel);

	chan = no_os_field_get(ADES1830_CH_CMD_MASK, adax);
	cmd |= no_os_field_prep(ADES1830_CH_CMD_MASK, chan);

	chan = no_os_field_get(ADES1830_CH4_ADAX_MASK, adax);
	cmd |= no_os_field_prep(ADES1830_CH4_CMD_MASK, chan);

	return ades1830_cmd_read(desc, cmd, aux);
}

int ades1830_read_aux2_adc(struct ades1830_desc *desc, uint64_t *aux,
			   enum ades1830_group_sel group,
			   enum ades1830_chan_adax adax)
{
	uint16_t cmd, chan;

	switch (group) {
	case ADES1830_GROUP_A:
		cmd = ADES1830_RDRAXA_CMD;
		break;
	case ADES1830_GROUP_B:
		cmd = ADES1830_RDRAXB_CMD;
		break;
	case ADES1830_GROUP_C:
		cmd = ADES1830_RDRAXC_CMD;
		break;
	case ADES1830_GROUP_D:
		cmd = ADES1830_RDRAXD_CMD;
		break;
	default:
		return -EINVAL;
	}

	chan = no_os_field_get(ADES1830_CH_CMD_MASK, adax);
	cmd |= no_os_field_prep(ADES1830_CH_CMD_MASK, chan);

	return ades1830_cmd_read(desc, cmd, aux);
}

int ades1830_set_pwm_discharge(struct ades1830_desc *desc, uint8_t cell_nb,
			       enum ades1830_pwm_discharge_duty duty)
{
	uint64_t pwm_reg;
	uint16_t cmd;
	int ret;

	if (cell_nb > 12)
		cmd = ADES1830_RDPWMB_CMD;
	else
		cmd = ADES1830_RDPWMA_CMD;

	ret = ades1830_cmd_read(desc, cmd, &pwm_reg);
	if (ret)
		return ret;

	pwm_reg &= ~ADES1830_PWMX_MASK(cell_nb);
	pwm_reg |= ADES1830_PWMX_MASK(cell_nb) & no_os_field_prep_u64(
			   ADES1830_PWMX_MASK(cell_nb), duty);

	if (cell_nb > 12)
		cmd = ADES1830_WRPWMB_CMD;
	else
		cmd = ADES1830_WRPWMA_CMD;

	return ades1830_cmd_write(desc, cmd, &pwm_reg);
}

int ades1830_set_discharge(struct ades1830_desc *desc, bool enable,
			   enum ades1830_discharge_timeout timeout,
			   uint8_t timeout_val)
{
	uint64_t cfgb_reg;
	int ret;

	if (timeout_val > 64)
		return -EINVAL;

	ret = ades1830_cmd_read(desc, ADES1830_RDCFGB_CMD, &cfgb_reg);
	if (ret)
		return ret;

	timeout_val |= no_os_field_prep(ADES1830_DTRNG_MASK, timeout);
	timeout_val |= no_os_field_prep(ADES1830_DTMEN_MASK, enable);

	cfgb_reg &= ~ADES1830_DISCHARGE_MASK;
	cfgb_reg |= ADES1830_DISCHARGE_MASK & no_os_field_prep_u64(
			    ADES1830_DISCHARGE_MASK, timeout_val);

	return ades1830_cmd_write(desc, ADES1830_WRCFGB_CMD, &cfgb_reg);
}

int ades1830_get_discharge(struct ades1830_desc *desc, bool *enable,
			   enum ades1830_discharge_timeout *timeout,
			   uint8_t *timeout_val)
{
	uint64_t cfgb_reg;
	uint8_t tmp;
	int ret;

	ret = ades1830_cmd_read(desc, ADES1830_RDCFGB_CMD, &cfgb_reg);
	if (ret)
		return ret;

	tmp = no_os_field_get(ADES1830_DISCHARGE_MASK, cfgb_reg);

	*timeout_val = no_os_field_get(ADES1830_DCTO_MASK, tmp);
	*timeout = no_os_field_get(ADES1830_DTRNG_MASK, tmp);
	*enable = no_os_field_get(ADES1830_DTMEN_MASK, tmp);

	return 0;
}

int ades1830_start_lpcm(struct ades1830_desc *desc, bool monitor)
{
	uint64_t reg_val;
	int ret;

	ret = ades1830_cmd_read(desc, ADES1830_RDCMCFG_CMD, &reg_val);
	if (ret)
		return ret;

	reg_val &= ~ADES1830_LPCM_MONITOR_MASK;
	reg_val |= ADES1830_LPCM_MONITOR_MASK & no_os_field_prep_u64(
			   ADES1830_LPCM_MONITOR_MASK, monitor);

	reg_val &= ~ADES1830_LPCM_NO_DEV_MASK;
	reg_val |= ADES1830_LPCM_NO_DEV_MASK & no_os_field_prep_u64(
			   ADES1830_LPCM_NO_DEV_MASK, desc->no_devices);

	return ades1830_cmd_send(desc, ADES1830_CMEN_CMD);
}

int ades1830_set_lpcm_hb_period(struct ades1830_desc *desc,
				enum ades1830_lpcm_hb_period hb_per)
{
	uint64_t reg_val;
	int ret;

	ret = ades1830_cmd_read(desc, ADES1830_RDCMCFG_CMD, &reg_val);
	if (ret)
		return ret;

	reg_val &= ~ADES1830_HB_PER_MASK;
	reg_val |= ADES1830_HB_PER_MASK & no_os_field_prep_u64(ADES1830_HB_PER_MASK,
			hb_per);

	return ades1830_cmd_write(desc, ADES1830_WRCMCFG_CMD, &reg_val);
}

int ades1830_get_lpcm_hb_period(struct ades1830_desc *desc,
				enum ades1830_lpcm_hb_period *hb_per)
{
	uint64_t reg_val;
	int ret;

	ret = ades1830_cmd_read(desc, ADES1830_RDCMCFG_CMD, &reg_val);
	if (ret)
		return ret;

	*hb_per = no_os_field_get_u64(ADES1830_HB_PER_MASK, reg_val);

	return 0;
}

int ades1830_set_lpcm_bridge(struct ades1830_desc *desc, bool bridgeless_to,
			     enum ades1830_lpcm_bridgeless_per brdgl_per)
{
	uint64_t reg_val;
	int ret;

	ret = ades1830_cmd_read(desc, ADES1830_RDCMCFG_CMD, &reg_val);
	if (ret)
		return ret;

	reg_val &= ~ADES1830_BRDGL_TO_MASK;
	reg_val |= ADES1830_BRDGL_TO_MASK & no_os_field_prep_u64(ADES1830_BRDGL_TO_MASK,
			bridgeless_to);

	reg_val &= ~ADES1830_BRDGL_PER_MASK;
	reg_val |= ADES1830_BRDGL_PER_MASK & no_os_field_prep_u64(
			   ADES1830_BRDGL_PER_MASK, brdgl_per);

	return ades1830_cmd_write(desc, ADES1830_WRCMCFG_CMD, &reg_val);
}

int ades1830_get_lpcm_bridge(struct ades1830_desc *desc, bool *bridgeless_to,
			     enum ades1830_lpcm_bridgeless_per *brdgl_per)
{
	uint64_t reg_val;
	int ret;

	ret = ades1830_cmd_read(desc, ADES1830_RDCMCFG_CMD, &reg_val);
	if (ret)
		return ret;

	*bridgeless_to = no_os_field_get_u64(ADES1830_BRDGL_TO_MASK, reg_val);
	*brdgl_per = no_os_field_get_u64(ADES1830_BRDGL_PER_MASK, reg_val);

	return 0;
}

int ades1830_set_lpcm_tx(struct ades1830_desc *desc,
			 enum ades1830_lpcm_tx_dir tx_dir)
{
	uint64_t reg_val;
	int ret;

	ret = ades1830_cmd_read(desc, ADES1830_RDCMCFG_CMD, &reg_val);
	if (ret)
		return ret;

	reg_val &= ~ADES1830_TX_DIR_MASK;
	reg_val |= ADES1830_TX_DIR_MASK & no_os_field_prep_u64(ADES1830_TX_DIR_MASK,
			tx_dir);

	return ades1830_cmd_write(desc, ADES1830_WRCMCFG_CMD, &reg_val);
}

int ades1830_get_lpcm_tx(struct ades1830_desc *desc,
			 enum ades1830_lpcm_tx_dir *tx_dir)
{
	uint64_t reg_val;
	int ret;

	ret = ades1830_cmd_read(desc, ADES1830_RDCMCFG_CMD, &reg_val);
	if (ret)
		return ret;

	*tx_dir = no_os_field_get_u64(ADES1830_TX_DIR_MASK, reg_val);

	return 0;
}

int ades1830_set_lpcm_gpio(struct ades1830_desc *desc,
			   enum ades1830_lcpm_gpio_interrupt gpio_int)
{
	uint64_t reg_val;
	int ret;

	ret = ades1830_cmd_read(desc, ADES1830_RDCMCFG_CMD, &reg_val);
	if (ret)
		return ret;

	reg_val &= ~ADES1830_GPIO_INT_MASK;
	reg_val |= ADES1830_GPIO_INT_MASK & no_os_field_prep_u64(ADES1830_GPIO_INT_MASK,
			gpio_int);

	return ades1830_cmd_write(desc, ADES1830_WRCMCFG_CMD, &reg_val);
}

int ades1830_get_lpcm_gpio(struct ades1830_desc *desc,
			   enum ades1830_lcpm_gpio_interrupt *gpio_int)
{
	uint64_t reg_val;
	int ret;

	ret = ades1830_cmd_read(desc, ADES1830_RDCMCFG_CMD, &reg_val);
	if (ret)
		return ret;

	*gpio_int = no_os_field_get_u64(ADES1830_GPIO_INT_MASK, reg_val);

	return 0;
}

int ades1830_set_lpcm_cell_mask(struct ades1830_desc *desc,
				uint32_t cell_mask)
{
	uint64_t reg_val;
	int ret;

	ret = ades1830_cmd_read(desc, ADES1830_RDCMCFG_CMD, &reg_val);
	if (ret)
		return ret;

	reg_val &= ~ADES1830_CELL_MASK;
	reg_val |= ADES1830_CELL_MASK & no_os_field_prep_u64(ADES1830_CELL_MASK,
			cell_mask);

	return ades1830_cmd_write(desc, ADES1830_WRCMCFG_CMD, &reg_val);
}

int ades1830_get_lpcm_cell_mask(struct ades1830_desc *desc,
				uint32_t *cell_mask)
{
	uint64_t reg_val;
	int ret;

	ret = ades1830_cmd_read(desc, ADES1830_RDCMCFG_CMD, &reg_val);
	if (ret)
		return ret;

	*cell_mask = no_os_field_get_u64(ADES1830_CELL_MASK, reg_val);

	return 0;
}

int ades1830_set_lpcm_gpio_mask(struct ades1830_desc *desc,
				uint16_t gpio_mask)
{
	uint64_t reg_val;
	int ret;

	ret = ades1830_cmd_read(desc, ADES1830_RDCMCFG_CMD, &reg_val);
	if (ret)
		return ret;

	reg_val &= ~ADES1830_GPIO_MASK;
	reg_val |= ADES1830_GPIO_MASK & no_os_field_prep_u64(ADES1830_GPIO_MASK,
			gpio_mask);

	return ades1830_cmd_write(desc, ADES1830_WRCMCFG_CMD, &reg_val);
}

int ades1830_get_lpcm_gpio_mask(struct ades1830_desc *desc,
				uint16_t *gpio_mask)
{
	uint64_t reg_val;
	int ret;

	ret = ades1830_cmd_read(desc, ADES1830_RDCMCFG_CMD, &reg_val);
	if (ret)
		return ret;

	*gpio_mask = no_os_field_get_u64(ADES1830_GPIO_MASK, reg_val);

	return 0;
}

int ades1830_set_lpcm_thr(struct ades1830_desc *desc, int32_t v_thr_mv,
			  enum ades1830_thr_type thr_type)
{
	uint64_t reg_val;
	uint64_t mask;
	uint16_t cmd;
	int ret;

	ret = ades1830_cmd_read(desc, ADES1830_RDCMCELLT_CMD, &reg_val);
	if (ret)
		return ret;

	return ades1830_cmd_write(desc, ADES1830_WRCMCELLT_CMD, &reg_val);

	switch (thr_type) {
	case ADES1830_CELL_UV_THR:
	case ADES1830_CELL_OV_THR:
	case ADES1830_CELL_DV_THR:
		ret = ades1830_cmd_read(desc, ADES1830_RDCMCELLT_CMD, &reg_val);
		if (ret)
			return ret;

		cmd = ADES1830_WRCMCELLT_CMD;

		break;
	case ADES1830_GPIO_UV_THR:
	case ADES1830_GPIO_OV_THR:
	case ADES1830_GPIO_DV_THR:
		ret = ades1830_cmd_read(desc, ADES1830_RDCMGPIOT_CMD, &reg_val);
		if (ret)
			return ret;

		cmd = ADES1830_WRCMGPIOT_CMD;

		break;
	default:
		return -EINVAL;
	}

	switch (thr_type) {
	case ADES1830_CELL_UV_THR:
	case ADES1830_GPIO_UV_THR:
		v_thr_mv = ((v_thr_mv - 1500) * 10) / 24;
		mask = ADES1830_UV_THR_MASK;

		break;
	case ADES1830_CELL_OV_THR:
	case ADES1830_GPIO_OV_THR:
		v_thr_mv = ((v_thr_mv - 1500) * 10) / 24;
		mask = ADES1830_OV_THR_MASK;

		break;
	case ADES1830_CELL_DV_THR:
	case ADES1830_GPIO_DV_THR:
		v_thr_mv = ((v_thr_mv) * 10) / 24;
		mask = ADES1830_DV_THR_MASK;

		break;
	default:
		return -EINVAL;
	}

	reg_val &= ~mask;
	reg_val |= mask & no_os_field_prep_u64(mask, v_thr_mv);

	return ades1830_cmd_write(desc, cmd, &reg_val);
}

int ades1830_get_lpcm_thr(struct ades1830_desc *desc, int32_t *v_thr_mv,
			  enum ades1830_thr_type thr_type)
{
	uint64_t reg_val;
	uint16_t cmd;
	uint16_t tmp;
	int ret;

	switch (thr_type) {
	case ADES1830_CELL_UV_THR:
	case ADES1830_CELL_OV_THR:
	case ADES1830_CELL_DV_THR:
		ret = ades1830_cmd_read(desc, ADES1830_RDCMCELLT_CMD, &reg_val);
		if (ret)
			return ret;

		break;
	case ADES1830_GPIO_UV_THR:
	case ADES1830_GPIO_OV_THR:
	case ADES1830_GPIO_DV_THR:
		ret = ades1830_cmd_read(desc, ADES1830_RDCMGPIOT_CMD, &reg_val);
		if (ret)
			return ret;

		break;
	default:
		return -EINVAL;
	}

	switch (thr_type) {
	case ADES1830_CELL_UV_THR:
	case ADES1830_GPIO_UV_THR:
		tmp = no_os_field_get_u64(ADES1830_UV_THR_MASK, reg_val);
		*v_thr_mv = ((tmp * 24) / 10) + 1500;

		return 0;
	case ADES1830_CELL_OV_THR:
	case ADES1830_GPIO_OV_THR:
		tmp = no_os_field_get_u64(ADES1830_OV_THR_MASK, reg_val);
		*v_thr_mv = ((tmp * 24) / 10) + 1500;

		return 0;
	case ADES1830_CELL_DV_THR:
	case ADES1830_GPIO_DV_THR:
		tmp = no_os_field_get_u64(ADES1830_DV_THR_MASK, reg_val);
		*v_thr_mv = ((tmp * 24) / 10);

		return 0;
	default:
		return -EINVAL;
	}
}

int ades1830_clear_adc_mem(struct ades1830_desc *desc,
			   enum ades1830_clr_adc_mem clr_adc)
{
	return ades1830_cmd_send(desc, clr_adc);
}

int ades1830_clear_flag(struct ades1830_desc *desc, enum ades1830_flt_flag flt)
{
	return ades1830_cmd_write(desc, ADES1830_CLRFLAG_CMD,
				  no_os_field_prep_u64(flt, 1));
}

int ades1830_clear_ov_and_uv(struct ades1830_desc *desc, uint8_t cell_nb,
			     bool uv)
{
	uint64_t mask;

	mask = uv ? ADES1830_CxUV_MASK(cell_nb) : ADES1830_CxOV_MASK(cell_nb);

	return ades1830_cmd_write(desc, ADES1830_CLOVUV_CMD,
				  no_os_field_prep_u64(mask, 1));
}

int ades1830_stop_lpcm(struct ades1830_desc *desc)
{
	int ret, i = 0;

	for (i = 0; i < desc->no_devices; i++) {
		ret = ades1830_cmd_send(desc, ADES1830_CMDIS_CMD);
		if (ret)
			return ret;

		no_os_udelay(6600);
	}

	return 0;
}

int ades1830_clear_cm_flag(struct ades1830_desc *desc)
{
	return ades1830_cmd_write(desc, ADES1830_CLRCMFLAG_CMD,
				  ADES1830_CLR_CM_FLAG_VAL);
}

int ades1830_soft_reset(struct ades1830_desc *desc)
{
	return ades1830_cmd_send(desc, ADES1830_SRST_CMD);
}

int ades1830_enable_ref(struct ades1830_desc *desc, bool enable)
{
	uint64_t reg_val;
	int ret;

	ret = ades1830_cmd_read(desc, ADES1830_RDCFGA_CMD, &reg_val);
	if (ret)
		return ret;

	reg_val &= ~ADES1830_REFON_MASK;
	reg_val |= ADES1830_REFON_MASK & no_os_field_prep_u64(ADES1830_REFON_MASK, 1);

	return ades1830_cmd_write(desc, ADES1830_WRCFGA_CMD, reg_val);
}

int ades1830_init(struct ades1830_desc **desc,
		  struct ades1830_init_param *init_param)
{
	struct ades1830_desc *descriptor;
	uint64_t reg_val;
	int ret, i;

	descriptor = (struct ades1830_desc *)no_os_calloc(sizeof(*descriptor), 1);
	if (!descriptor)
		return -ENOMEM;

	ret = no_os_spi_init(&descriptor->spi_desc, init_param->spi_param);
	if (ret)
		goto exit;

	descriptor->no_devices = init_param->no_devices;

	ret = ades1830_cmd_read(desc, ADES1830_RDSTATC_CMD, &reg_val);
	if (ret)
		goto remove_spi;

	if (no_os_field_get(ADES1830_TMODCHK_MASK, reg_val)) {
		ret = ades1830_clear_flag(descriptor, ADES1830_CL_TMODE);
		if (ret)
			goto remove_spi;
	}

	ret = ades1830_set_discharge(desc, false, ADES1830_DISCHARGE_MIN_RANGE,
				     ADES1830_INIT_DISCHARGE_VAL_16MIN);
	if (ret)
		goto remove_spi;

	ret = ades1830_enable_ref(desc, true);
	if (ret)
		goto remove_spi;

	*desc = descriptor;

remove_spi:
	no_os_spi_remove(descriptor->spi_desc);
exit:
	no_os_free(descriptor);

	return ret;
}

int ades1830_remove(struct ades1830_desc *desc)
{
	int ret;

	ades1830_set_discharge(desc, true, ADES1830_DISCHARGE_MIN_RANGE,
			       ADES1830_REMOVE_DISCHARGE_VAL_16MIN);

	no_os_mdelay(2000);

	no_os_spi_remove(desc->spi_desc);
	no_os_free(desc);

	return 0;
}
