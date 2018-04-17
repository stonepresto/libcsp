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

#include <unistd.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.c>

#include <csp/csp.h>
#include <csp/interfaces/csp_if_i2c.h>
#include <csp/drivers/i2c.h>

/* Function Prototypes (not declared in i2c.h) */

static int csp_device_suspend(struct i2c_client *client, pm_message_t msg);
static int csp_device_resume(struct i2c_client *client);
static int __devexit csp_device_remove(struct i2c_client *client);
static int csp_device_probe(struct i2c_client * client, struct i2c_device_id *idp);
static ssize_t csp_device_power_on(struct csp_device *dev, struct device_attribute *attr, char *buf);

/* End Function Prototypes */

/* Globals */

static int file_i2c;

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
/* TODO:
	- Implement args
*/
	int err = 0;

	char *filename = (char*)"/dev/i2c-1";
	if ((file_i2c = open(filename, O_RDWR)) < 0)
	{
		printf("Failed to open the i2c busi\n");
		err = file_i2c;
		goto exit;
	}

exit:
	return err;
};


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
/*
	TODO:
	- Implement timeout
*/
	int err = 0;

	if ((err = ioctl(file_i2c, handle, frame->dest)) < 0)
	{
		printf("Failed to access the bus or talk to slave.\n");
		goto exit;
	}

	if ((err = write(file_i2c, frame->data, frame->len)) != frame->len)
	{
		printf("Failed to write to the i2c bus.\n");
	}

exit:
	return err;
};

/* End Functions */


/* References 

1. https://elixir.bootlin.com/linux/
2. https://www.kernel.org/doc/Documentation/i2c/writing-clients
3. http://renjucnair.blogspot.ca/2012/01/writing-i2c-client-driver.html
4. https://witekio.com/blog/i2c-2-6-32-linux-kernel/
5. http://www.diegm.uniud.it/loghi/CE/slides/usense.pdf
6. https://learn.sparkfun.com/tutorials/i2c#protocol


