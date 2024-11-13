USB_CONNECTION ?= 0
SERIAL_CONNECTION ?= 0
T1L_CONNECTION ?= 0

BASIC_EXAMPLE = y
IIOD=y

HEAP_SIZE=0x80000
STACK_SIZE=0x20000

ifeq ($(USB_CONNECTION),1)
	ifeq ($(SERIAL_CONNECTION),1 ||$(T1L_CONNECTION),1)
		$(error 'ERROR: Select only one communication interface')
	endif
	CFLAGS += -DPQM_CONN_USB
	CFLAGS += -DNO_OS_USB_UART
endif

ifeq ($(SERIAL_CONNECTION),1)
	ifeq ($(USB_CONNECTION),1 || $(T1L_CONNECTION),1)
		$(error 'ERROR: Select only one communication interface')
	endif
	CFLAGS += -DPQM_CONN_SERIAL
endif

ifeq ($(T1L_CONNECTION),1)
	ifeq ($(USB_CONNECTION),1 || $(SERIAL_CONNECTION),1)
		$(error 'ERROR: Select only one communication interface')
	endif
	CFLAGS += -DPQM_CONN_T1L
	CFLAGS += -DNO_OS_LWIP_NETWORKING
	CFLAGS += -DNO_OS_LWIP_INIT_ONETIME=1
	NO_OS_IP=192.168.97.40
	NO_OS_NETMASK=255.255.0.0
	NO_OS_GATEWAY=0.0.0.0
endif

ifndef PQLIB_PATH
$(error 'No PQM library path defined')
endif

include ../../tools/scripts/generic_variables.mk

CFLAGS_MFLOAT_TYPE=soft
CFLAGS += -DIIO_IGNORE_BUFF_OVERRUN_ERR

include src.mk
include ../../tools/scripts/generic.mk
