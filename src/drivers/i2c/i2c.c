/*
Cubesat Space Protocol - A small network-layer protocol designed for Cubesats
Copyright (C) 2012 GomSpace ApS (http://www.gomspace.com)
Copyright (C) 2012 AAUSAT3 Project (http://aausat3.space.aau.dk)

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

/* I2C Driver */

/* Headers and Macros */

#include <stdio.h>
#include <stdint.h>

// #include <stdint.h>
// #include <stdlib.h>
// #include <stdio.h>
// #include <unistd.h>
// #include <time.h>
// #include <string.h>
// #include <errno.h>

// #include <pthread.h>
// #include <semaphore.h>

// #include <sys/types.h>
// #include <sys/socket.h>
// #include <sys/ioctl.h>
// #include <net/if.h>
// #include <sys/queue.h>
// #include <sys/uio.h>
// #include <sys/time.h>
// #include <net/if.h>

#include <linux/i2c.h>
#include <linux/module.h>
#include <linux/init.h>
#include <kernel.h>
#include <linux/interrupt.h>

#include <csp/csp.h>
#include <csp/interfaces/csp_if_i2c.h>
#include <csp/drivers/i2c.h>

MODULE_AUTHOR("stonepresto");
MODULE_DESCRIPTION("Driver for devices utilizing the network-layer CSP protocol.");

/* End Headers and Macros */


/* Function Prototypes (not declared in i2c.h) */

	//TODO: csp_device_suspend
	//TODO: csp_device_resume
	//TODO: csp_device_remove
static int csp_device_probe(struct i2c_client * client, struct i2c_device_id *idp);
static ssize_t csp_device_power_on(struct csp_device *dev, struct device_attribute *attr, char *buf);

/* End Function Prototypes */


/* Structure Definition */

static struct i2c_device_id csp_device_id[] = {
	{"csp_device", 0 },
	{}
};

/* Power manangement structure, needs only suspend and resume for I2C */
static const struct dev_pm_ops csp_device_pm_ops = {
	.suspend = csp_device_suspend, /* Handler for when system goes to sleep */
	.resume = csp_device_resume /* Handler for system wake */
};

/* Driver to register with system */
static struct i2c_driver csp_driver = {
	.probe = csp_device_probe,
	.remove = __devexit_p(csp_device_remove), /* Function will never be invoked by PCI handler */
	.id_table = csp_device_id,
	.class = "CSP_DEVICE"
	.driver = {
		.pm = &csp_device_pm_ops,
		.owner = THIS_MODULE, //TODO
		.name = "csp_device"
	}
};
/* Immediately register the device module */
module_i2c_driver(csp_driver);

/* End Structure Definition */


/* Globals */

#define POWER_REG 0x10

/* End Globals */


/* Functions */

/**
 * Initialise the I2C driver
 *
 * @param handle Which I2C bus (if more than one exists)
 * @param mode I2C device mode. Must be either I2C_MASTER or I2C_SLAVE
 * @param addr Own slave address
 * @param speed Bus speed in kbps
 * @param queue_len_tx Length of transmit queue
 * @param queue_len_rx Length of receive queue
 * @param callback If this value is set, the driver will call this function instead of using an RX queue
 * @return Error code
 */
int i2c_init(int handle, int mode, uint8_t addr, uint16_t speed, int queue_len_tx, int queue_len_rx, i2c_callback_t callback)
{
	//TODO: What are all these inputs about...
	return i2c_add_driver(&csp_driver);
};
module_init(i2c_init);

/**
 * Send I2C frame via the selected device
 *
 * @param handle Handle to the device
 * @param frame Pointer to I2C frame
 * @param timeout Ticks to wait
 * @return Error code
 */
int i2c_send(int handle, i2c_frame_t * frame, uint16_t timeout)
{
	//TODO: What is AC? See ref 6 for purpose behind logical OR
	err = i2c_smbus_write_block_data(handle,POWER_REG | AC, sizeof(frame), frame);
	if (err < 0)
		pr_err("%s: POWER_REGs i2c write failed\n",__func__);

	return err;
};


static int csp_device_probe(struct i2c_client * client, struct i2c_device_id *idp)
{
	// ???
	return 0;
}

static ssize_t csp_device_power_on(struct csp_device *dev, struct device_attribute *attr, char *buf)
{
	// ???
	return (ssize_t) 0;
}

/* End Functions */


/* References 

	s32 i2c_smbus_read_block_data(struct i2c_client *client, u8 command, u8 *values);
	s32 i2c_smbus_write_block_data(struct i2c_client *client, u8 command, u8 length, const u8 *values);

1. https://elixir.bootlin.com/linux/
2. https://www.kernel.org/doc/Documentation/i2c/writing-clients
3. http://renjucnair.blogspot.ca/2012/01/writing-i2c-client-driver.html
4. https://witekio.com/blog/i2c-2-6-32-linux-kernel/
5. http://www.diegm.uniud.it/loghi/CE/slides/usense.pdf
6. https://learn.sparkfun.com/tutorials/i2c#protocol

*/
