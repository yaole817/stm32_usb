// test_libusb1.cpp : 定义控制台应用程序的入口点。
//
#include <stdio.h>
#include <iostream>
#include "libusb.h"
using namespace std;

#define m_dev_VENDOR    0x0CF3		/* Vendor ID of the m_dev         */ 
#define m_dev_PRODUCT   0xE300		/* Product ID of the m_dev        */ 
#define m_dev_DATA_SIZE 0x12       /* Number of bytes to get at once */ 

static void print_devs(libusb_device **devs)
{
	libusb_device *dev;
	int i = 0, j = 0;
	uint8_t path[8]; 

	while ((dev = devs[i++]) != NULL) {
		struct libusb_device_descriptor desc;
		int r = libusb_get_device_descriptor(dev, &desc);
		if (r < 0) {
			cout<<"failed to get device descriptor--returned:"<<r<<endl;
			return;
		}

		printf("%04x:%04x (bus %d, device %d)",
			desc.idVendor, desc.idProduct,
			libusb_get_bus_number(dev), libusb_get_device_address(dev));

		r = libusb_get_port_numbers(dev, path, sizeof(path));
		if (r > 0) {
			printf(" path: %d", path[0]);
			for (j = 1; j < r; j++)
				printf(".%d", path[j]);
		}
		printf("\n");
	}
}

int main()
{
	libusb_device **devs;
	int cnt;

	cnt = libusb_init(NULL);
	if (cnt < 0){
		printf("usb_find_busses failed - returned: %i\n", cnt); 
		return cnt; 
	}

	cnt = libusb_get_device_list(NULL, &devs);
	if (cnt < 0){
		printf("usb_find_devices failed - returned: %i\n", cnt); 
		return cnt; 
	}

	print_devs(devs);
	libusb_free_device_list(devs, 1);


	libusb_exit(NULL);
	return 0;
}
