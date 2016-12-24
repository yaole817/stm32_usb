// test_libusb1.cpp : 定义控制台应用程序的入口点。
//
#include <stdio.h>
#include <iostream>
#include "libusb.h"
using namespace std;

#define m_dev_VENDOR    0x046D		/* Vendor ID of the m_dev         */ 
#define m_dev_PRODUCT   0xC077		/* Product ID of the m_dev        */ 
#define m_dev_DATA_SIZE 0x12       /* Number of bytes to get at once */ 

struct libusb_device* getMyDevice(libusb_device **devs,int venderId,int productID);

int main()
{
	int cnt;
	libusb_device **devs;
	libusb_device *myDev;
	libusb_context *ctx = NULL;    //a libusb session  

	cnt = libusb_init(NULL);
	if (cnt < 0){
		printf("usb_find_busses failed - returned: %i\n", cnt); 
		return cnt; 
	}

	cnt = libusb_get_device_list(ctx, &devs); //获取所有的usb设备并存至devs
	if (cnt < 0){
		printf("usb_find_devices failed - returned: %i\n", cnt); 
		return cnt; 
	}
	myDev = getMyDevice(devs,m_dev_VENDOR,m_dev_PRODUCT); //获取所要找的设备
	if (myDev == NULL){
		cout<<"device is not find"<<endl;
		return 0;
	}
	struct libusb_device_descriptor desc;
	libusb_get_device_descriptor(myDev, &desc);
	cout<<" vender id is :"<<desc.idVendor<<endl;
	cout<<"product id is :"<<desc.idProduct<<endl;
	
	
	libusb_device_handle* myDeviceHandle=libusb_open_device_with_vid_pid(ctx,m_dev_VENDOR,m_dev_PRODUCT);
	if(myDeviceHandle == NULL){
		cout<<"can not open device"<<endl;
		return 0;
	}
	libusb_free_device_list(devs, 1); //free the list, unref the devices in it
	if(libusb_kernel_driver_active(myDeviceHandle, 0) == 1) { //find out if kernel driver is attached
		cout<<"Kernel Driver Active"<<endl;

		if(libusb_detach_kernel_driver(myDeviceHandle, 0) == 0) //detach it
			cout<<"Kernel Driver Detached!"<<endl;
	}

	cnt = libusb_claim_interface(myDeviceHandle, 0); //claim interface 0 (the first) of device (mine had jsut 1)
	if(cnt < 0) {
		cout<<"Cannot Claim Interface"<<endl;
		return 1;
	}
	cout<<"Claimed Interface"<<endl;

	int size;  
	unsigned char datain[1024]={};  
	for(int i=0;i<300;i++)  
	{  
		int rr = libusb_interrupt_transfer(
				myDeviceHandle,  
				0x81,  
				datain,  
				4,  
				&size,  
				0
				);  
		if (rr==0){
			std::cout<<"libusb_interrupt_transfer rr: "<<rr<<endl;  
			cout<<"size: "<<size<<endl;  
			printf("data: ");  
			if (sizeof(datain)!=0){
				for(int j=0; j<size; j++)  
						printf("%02x ", (unsigned char)(datain[j]));  
				printf("\n");  
			}
		}
	}
	
	//libusb_free_device_list(devs, 1);
	libusb_close(myDeviceHandle);
	libusb_exit(NULL);
	return 0;
}

struct libusb_device* getMyDevice(libusb_device **devs,int venderId,int productID){
	libusb_device *dev;
	libusb_device *myDev=NULL;
	int i = 0, j = 0;
	uint8_t path[8]; 

	while ((dev = devs[i++]) != NULL) {
		struct libusb_device_descriptor desc;
		int r = libusb_get_device_descriptor(dev, &desc);
		if (r < 0) {
			break;
		}
		if(desc.idVendor == venderId && desc.idProduct == productID ){
			myDev=dev;
			break;
		}
	/*	printf("%04x:%04x (bus %d, device %d)",
			desc.idVendor, desc.idProduct,
			libusb_get_bus_number(dev),
			libusb_get_device_address(dev));*/

		r = libusb_get_port_numbers(dev, path, sizeof(path));

	}
	return myDev;
}
/*
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
*/