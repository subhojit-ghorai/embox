/**
 * \file amba_pnp.h
 *
 * \date 28.01.2009
 * \author Alexandr Batyukov, Alexey Fomin, Eldar Abusalimov
 * \details Driver for devices founded on the Amba Plug&Play bus (Advanced Microcontroller Bus Architecture)
 */

#ifndef AMBA_PNP_H_
#define AMBA_PNP_H_

#include "pnp_id.h"
#include "common.h"

#define TRY_CAPTURE_AHBM_DEV(dev,venID,devID) if (-1 == capture_amba_dev(dev, venID, devID, TRUE, TRUE)){\
	LOG_ERROR("can't capture ahbm dev venID=0x%X, devID=0x%X\n", venID, devID);\
	return -1;\
}
#define TRY_CAPTURE_AHBSL_DEV(dev,venID,devID) if (-1 == capture_amba_dev(dev, venID, devID, TRUE, FALSE)){\
	LOG_ERROR("can't capture ahbsl dev venID=0x%X, devID=0x%X\n", venID, devID);\
	return -1;\
}

#define TRY_CAPTURE_APB_DEV(dev,venID,devID) if (-1 == capture_amba_dev(dev, venID, devID, FALSE, FALSE)){\
	LOG_ERROR("can't capture apb dev venID=0x%X, devID=0x%X\n", venID, devID);\
	return -1;\
}

#define AHB_MASTER_BASE          0xFFFFF000 // max: 64 devices
#define AHB_SLAVE_BASE           0xFFFFF800 // max: 64 devices
#define APB_BASE                 0x800FF000 // max: 512 devices

#define AHB_MASTERS_QUANTITY     0x10
#define AHB_SLAVES_QUANTITY      0x40
#define APB_QUANTITY             0x40

/**
 * \struct AMBA_BAR_INFO
 * Amba Bank Address Register Info.
 * BAR0-BAR3 registers are presented.
 */
typedef struct _AMBA_BAR_INFO {
	UINT32 start;
	BOOL   prefetchable;
	BOOL   cacheable;
	UINT32 mask;
	BYTE   type;
	BOOL   used;
} AMBA_BAR_INFO;

/**
 * \struct AMBA_DEV_INFO
 */
typedef struct _AMBA_DEV_INFO {
	BYTE   venID;
	UINT16 devID;
	BYTE   version;
	BYTE   irq;
} AMBA_DEV_INFO;


struct _AMBA_DEV;
// each device must have handler
typedef void (*HANDLER_DATA_FUNC)(struct _AMBA_DEV *dev);

/**
 * \struct AMBA_DEV
 */
typedef struct _AMBA_DEV{
	AMBA_DEV_INFO      dev_info;     /**< VendorID, DeviceID, version, IRQ */
	AMBA_BAR_INFO      bar[4];
	BYTE               slot;         /**< information about location */
	HANDLER_DATA_FUNC  show_info;    /**< show brief description */
	char               dev_name[16]; /**< logical name */
	BOOL               is_ahb;
	BOOL               is_master;
	UINT32             user_def[3];  /**< info from user registers */
} AMBA_DEV;

static AMBA_DEV *ahbm_devices[AHB_MASTERS_QUANTITY];
static AMBA_DEV *ahbsl_devices[AHB_SLAVES_QUANTITY];
static AMBA_DEV *apb_devices[APB_QUANTITY];

/*
 * pnp_dev must be allocated by caller
 * returns 0 if ok, non-zero otherwise
 */
//int capture_ahbm_dev(AHB_DEV *ahb_dev, BYTE vendor_id, UINT16 device_id);
/*
 * pnp_dev must be allocated by caller
 * returns 0 if ok, non-zero otherwise
 */
//int capture_ahbsl_dev(AHB_DEV *ahb_dev, BYTE vendor_id, UINT16 device_id);

/**
 * Capture amba pnp device.
 * @param apb_dev AMBA_DEV
 * @param vendor_id vendor ID
 * @param device_id device ID
 * @param is_ahb ahb/not ahb
 * @param is_master master/slave
 * @return slot number or -1 if error
 */
int capture_amba_dev(AMBA_DEV *apb_dev, BYTE vendor_id, UINT16 device_id, BOOL is_ahb, BOOL is_master);

/**
 * Fill amba device.
 * @param dev amba device
 * @param slot_number slot number
 * @param is_ahb ahb/not ahb
 * @param is_master master/slave
 * @return TRUE (1) if successed
 * @return FALSE (0) slot is empty
 */
BOOL fill_amba_dev(AMBA_DEV *dev, BYTE slot_number, BOOL is_ahb, BOOL is_master);

/**
 * Free amba device.
 */
int free_amba_dev(AMBA_DEV *dev);

#endif /* AMBA_PNP_H_ */
