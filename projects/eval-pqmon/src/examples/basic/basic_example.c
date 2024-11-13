/*******************************************************************************
 *   @file   basic_example.c
 *   @brief  PQM firmware, called by main
 *   @author Robert Budai (robert.budai@analog.com)
 ********************************************************************************
 * Copyright (c) 2024 Analog Devices, Inc.
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

/******************************************************************************/
/***************************** Include Files **********************************/
/******************************************************************************/

#ifdef BASIC_EXAMPLE
#include "basic_example.h"
#endif

/******************************************************************************/
/************************ Functions Definitions *******************************/
/******************************************************************************/

int32_t pqm_init(struct pqm_desc **desc, struct pqm_init_para *param)
{
	struct pqm_desc *d;
	d = (struct pqm_desc *)no_os_calloc(1, sizeof(*d));

	if (!d)
		return -ENOMEM;

	d->ext_buff = param->ext_buff;
	d->ext_buff_len = param->ext_buff_len;
	for (int i = 0; i < TOTAL_PQM_CHANNELS; i++) {
		for (int j = 0; j < MAX_CH_ATTRS; j++) {
			d->pqm_ch_attr[i][j] = 0;
		}
	}
	for (int i = 0; i < PQM_DEVICE_ATTR_NUMBER; i++) {
		d->pqm_global_attr[i] = param->dev_global_attr[i];
	}
	*desc = d;

	return 0;
}

int32_t pqm_remove(struct pqm_desc *desc)
{
	if (!desc)
		return -EINVAL;
	no_os_free(desc);

	return 0;
}

int32_t update_pqm_channels(void *dev, uint32_t mask)
{
	struct pqm_desc *desc;

	if (!dev)
		return -ENODEV;

	desc = dev;
	desc->active_ch = mask;

	return 0;
}

int32_t close_pqm_channels(void *dev)
{
	struct pqm_desc *desc;

	if (!dev)
		return -ENODEV;

	desc = dev;
	desc->active_ch = 0;

	return 0;
}

#if defined(PQM_CONN_SERIAL)
int32_t enable_RS482_driver()
{
	// Enable driver and receiver of ADM2587, used only by serial connection
	int status;
	struct no_os_gpio_sec *enable_ADM2587;
	struct nhd_c12832a1z_dev *nhd_c12832a1z_device;

	struct max_gpio_init_param gpio_extra_ip_ADM2587 = {
		.vssel = MXC_GPIO_VSSEL_VDDIOH,
	};

	struct no_os_gpio_init_param gpio_ADM2587_ip = {
		.port = 2,
		.number = 15,
		.pull = NO_OS_PULL_NONE,
		.platform_ops = &max_gpio_ops,
		.extra = &gpio_extra_ip_ADM2587,
	};

	status = no_os_gpio_get(&enable_ADM2587, &gpio_ADM2587_ip);
	if (status)
		return status;
	status = no_os_gpio_set_value(enable_ADM2587, 1);
	return status;
}
#endif

#if defined(PQM_CONN_T1L)
int32_t setup_t1l_connection()
{
	int ret;
	uint8_t adin1110_mac_address[6] = {0x00, 0x18, 0x80, 0x03, 0x25, 0x60};
	const struct no_os_gpio_init_param adin1110_int_ip = {
		.port = 2,
		.number = 6,
		.pull = NO_OS_PULL_UP,
		.platform_ops = &max_gpio_ops,
		.extra = GPIO_EXTRA,
	};

	const struct no_os_gpio_init_param adin1110_rst_gpio_ip = {
		.port = 2,
		.number = 1,
		.pull = NO_OS_PULL_UP,
		.platform_ops = &max_gpio_ops,
		.extra = GPIO_EXTRA,
	};

	const struct no_os_gpio_init_param adin1110_swpd_ip = {
		.port = 2,
		.number = 25,
		.pull = NO_OS_PULL_UP,
		.platform_ops = &max_gpio_ops,
		.extra = GPIO_EXTRA,
	};

	const struct no_os_gpio_init_param adin1110_tx2p4_ip = {
		.port = 2,
		.number = 10,
		.pull = NO_OS_PULL_DOWN,
		.platform_ops = &max_gpio_ops,
		.extra = GPIO_EXTRA,
	};

	const struct no_os_gpio_init_param adin1110_mssel_ip = {
		.port = 2,
		.number = 9,
		.pull = NO_OS_PULL_NONE,
		.platform_ops = &max_gpio_ops,
		.extra = GPIO_EXTRA,
	};

	const struct no_os_gpio_init_param adin1110_cfg0_ip = {
		.port = 2,
		.number = 3,
		.pull = NO_OS_PULL_NONE,
		.platform_ops = &max_gpio_ops,
		.extra = GPIO_EXTRA,
	};

	const struct no_os_gpio_init_param adin1110_cfg1_ip = {
		.port = 2,
		.number = 0,
		.pull = NO_OS_PULL_UP,
		.platform_ops = &max_gpio_ops,
		.extra = GPIO_EXTRA,
	};
	const struct no_os_spi_init_param adin1110_spi_ip = {
		.device_id = 2,
		.max_speed_hz = 15000000,
		.bit_order = NO_OS_SPI_BIT_ORDER_MSB_FIRST,
		.mode = NO_OS_SPI_MODE_0,
		.platform_ops = SPI_OPS,
		.chip_select = 0,
		.extra = ADIN_SPI_EXTRA,
	};
	struct adin1110_init_param adin1110_ip = {
		.chip_type = ADIN1110,
		.comm_param = adin1110_spi_ip,
		.reset_param = adin1110_rst_gpio_ip,
		.append_crc = false,
	};
	struct lwip_network_param lwip_ip = {
		.platform_ops = &adin1110_lwip_ops,
		.mac_param = &adin1110_ip,
	};
	struct lwip_network_desc *lwip_desc;

	struct no_os_gpio_desc *adin1110_swpd_gpio;
	struct no_os_gpio_desc *adin1110_tx2p4_gpio;
	struct no_os_gpio_desc *adin1110_mssel_gpio;
	struct no_os_gpio_desc *adin1110_cfg0_gpio;
	struct no_os_gpio_desc *adin1110_cfg1_gpio;
	struct no_os_gpio_desc *adin1110_int_gpio;

	no_os_gpio_get(&adin1110_cfg0_gpio, &adin1110_cfg0_ip);
	no_os_gpio_get(&adin1110_swpd_gpio, &adin1110_swpd_ip);
	no_os_gpio_get(&adin1110_tx2p4_gpio, &adin1110_tx2p4_ip);
	no_os_gpio_get(&adin1110_mssel_gpio, &adin1110_mssel_ip);
	no_os_gpio_get(&adin1110_cfg1_gpio, &adin1110_cfg1_ip);
	no_os_gpio_get(&adin1110_int_gpio, &adin1110_int_ip);
	no_os_gpio_direction_output(adin1110_swpd_gpio, 1);
	no_os_gpio_direction_output(adin1110_tx2p4_gpio, 0);
	no_os_gpio_direction_output(adin1110_mssel_gpio, 1);
	no_os_gpio_direction_output(adin1110_cfg1_gpio, 1);
	no_os_gpio_direction_output(adin1110_cfg0_gpio, 1);
	no_os_gpio_direction_input(adin1110_int_gpio);

	memcpy(adin1110_ip.mac_address, adin1110_mac_address, NETIF_MAX_HWADDR_LEN);
	memcpy(lwip_ip.hwaddr, adin1110_mac_address, NETIF_MAX_HWADDR_LEN);

	ret = no_os_lwip_init(&lwip_desc, &lwip_ip);
	if (ret) {
		return ret;
	}


}
#endif

int basic_pqm_firmware()
{

	struct no_os_uart_desc *uart_desc;
	struct pqm_desc *pqm_desc;
	struct iio_app_desc *app;
	struct iio_app_init_param app_init_param = {0};
	struct iio_data_buffer buff = {
		.buff = (void *)iio_data_buffer_loc,
		.size = MAX_SIZE_BASE_ADDR_WITH_SIZE,
	};
	pqlibExample.no_os_cb_desc = NULL;

	memset(iio_data_buffer_loc, 0, MAX_SIZE_BASE_ADDR_WITH_SIZE);

	int status = SYS_STATUS_SUCCESS;

	status = init_lcd();
	if (status)
		goto exit;

	status = no_os_uart_init(&uart_desc, &uart_ip_stdio);
	if (status)
		goto exit;

	no_os_uart_stdio(uart_desc);

#if defined(PQM_CONN_SERIAL)
	status = enable_RS482_driver();
	if (status)
		goto exit;
#endif

	status = afe_init();
	if (status != SYS_STATUS_SUCCESS) {
		printf("AFE Init failed \n\r");
		goto exit;
	}

	status = pqm_measurement_init();
	if (status != SYS_STATUS_SUCCESS) {
		printf("Pqm Measurement Init failed \n\r");
		goto exit;
	}

	status = pqm_init(&pqm_desc, &pqm_ip);
	if (status) {
		printf("PQM initialization failed \n\r");
		goto exit;
	}

	status = pqm_start_measurement(false);
	if (status != SYS_STATUS_SUCCESS) {
		printf("PQM could not start measurements, status: %d \n\r", status);
		goto exit;
	}
	printf("Mesurements started \n\r");

	struct iio_app_device devices[] = {
		IIO_APP_DEVICE("pqm", pqm_desc, &pqm_iio_descriptor, &buff, NULL, NULL)
	};

	app_init_param.devices = devices;
	app_init_param.nb_devices = NO_OS_ARRAY_SIZE(devices);

#if defined(PQM_CONN_USB)
	app_init_param.uart_init_params = iio_demo_usb_ip;
#elif defined(PQM_CONN_SERIAL)
	app_init_param.uart_init_params = iio_demo_serial_ip;
#elif defined(PQM_CONN_T1L)
	app_init_param.uart_init_params = iio_demo_serial_ip;
	app_init_param.lwip_param.platform_ops = &adin1110_lwip_ops;
	app_init_param.lwip_param.mac_param = &adin1110_ip;
	app_init_param.lwip_param.extra = NULL;
#endif

	app_init_param.post_step_callback = &(pqm_one_cycle);
	status = iio_app_init(&app, app_init_param);

	if (status)
		goto exit;

	status = iio_app_run(app);

	iio_app_remove(app);

exit:
	pqm_remove(pqm_desc);
	no_os_uart_remove(uart_desc);
	return status;
}
