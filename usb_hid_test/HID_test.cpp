#include <iostream>
#include "lusb0_usb.h"
using namespace std;

#define m_dev_VENDOR    0x046D	/* Vendor ID of the m_dev         */ 
#define m_dev_PRODUCT   0xC077	/* Product ID of the m_dev        */ 
#define m_dev_DATA_SIZE 0x12       /* Number of bytes to get at once */ 

struct usb_device* my_usb_init(int idVendor,int idProduct);

int main(int argc, char *argv[]) 
{ 
	int ret, i; 
	char data[m_dev_DATA_SIZE] = {0}; 
	struct usb_device *m_dev = NULL; 
	struct usb_dev_handle *m_dev_handle; 
	usb_init(); 
	m_dev = my_usb_init(m_dev_VENDOR,m_dev_PRODUCT);
	if(!m_dev) 
	{ 
		cout<<"m_dev not found"<<endl; 
		return 1; 
	} 
	m_dev_handle = usb_open(m_dev); 
	if(!m_dev_handle) 
	{ 
		cout<<"Could not open m_dev"<<endl; 
		return 1; 
	} 
  
	//printf("m_dev opened: %s\n", m_dev->filename); 
	//printf("Device Protocol : %d\n",m_dev->descriptor.bDeviceProtocol);
	//printf("Report Length : %d\n",m_dev->descriptor.bLength);
	//printf("Decriptor Type : %d\n",m_dev->descriptor.bDescriptorType);
	//printf("End Points : %d\n",m_dev->config->interface->altsetting->bNumEndpoints);
	//printf("Interface Class : %d\n",m_dev->config->interface->altsetting->bInterfaceClass);
	//printf("Protocol : %d\n",m_dev->config->interface->altsetting->bInterfaceProtocol);
	//printf("Interface Number: %d\n",m_dev->config->interface->altsetting->bInterfaceNumber);
	//printf("Device Filename : %s\n",m_dev->filename);
 //     
	//usb_get_string_simple(m_dev_handle, m_dev->descriptor.iManufacturer, str, sizeof(str));
	//printf("Device Manfucaturer : %s\n",str);
	//usb_get_string_simple(m_dev_handle,m_dev->descriptor.iProduct,str,sizeof(str));
	//printf("Product Name : %s\n",str);
	//usb_get_string_simple(m_dev_handle,m_dev->descriptor.iSerialNumber,str,sizeof(str));
	//printf("Device Serial Number: %s\n",str);
	//printf("End point addresses : 0x%x\n",m_dev->config->interface->altsetting->endpoint->bEndpointAddress);
		//return 0;

	if(usb_set_configuration(m_dev_handle, 1) < 0) 
	{ 
		cout<<"Could not set configuration"<<endl; 
		usb_close(m_dev_handle); 
		return 1; 
	} 
	if(usb_claim_interface(m_dev_handle, 0) < 0) //claim_interface 0指向第一个设备
	{ 
		cout<<"Could not claim interface"<<endl; 
		usb_close(m_dev_handle); 
		return 1; 
	} 
	char key = '\0';
	while(key != 'q') 
	{ 
		ret = usb_interrupt_read(m_dev_handle, 0x81, data, m_dev_DATA_SIZE, 0); 
		//ret = usb_bulk_read(m_dev_handle, 0x81, data, m_dev_DATA_SIZE, 0); 
		cout<<"ret: "<<ret<<endl; 
		for(i = 0; i< ret; i++) 
		{
			cout<<(unsigned char)(data[i])<<" "; 
		}
		cout<<endl;
		key=getchar();
	} 
  
	usb_release_interface(m_dev_handle, 0); 
	usb_close(m_dev_handle); 
	cout<<"m_dev closed"<<endl; 
  
	return 0; 
} 
 struct usb_device* my_usb_init(int idVendor,int idProduct){
	int ret;
	ret = usb_find_busses(); 
	if(ret<0) { 
		cout<<"usb_find_busses failed - returned:" <<ret<<endl; 
		return NULL;
	} 
	cout<<"device bus number  is :  "<<ret<<endl;

	ret = usb_find_devices(); 
	if(ret<0) { 
		cout<<"usb_find_devices failed - returned: "<<ret<<endl;
		return NULL;
	} 
	cout<<"device number is :  "<<ret<<endl;

	struct usb_bus *bus; 
	for(bus = usb_get_busses(); bus; bus = bus->next) 
	{ 
		cout<<"num"<<endl;
		struct usb_device *dev; 
		for(dev = bus->devices; dev; dev = dev->next) 
		{ 
			cout<<"product i= "<<dev->descriptor.idProduct<<"\t"<<"vender id ="<<dev->descriptor.idVendor<<endl;
			if(dev->descriptor.idVendor == idVendor && dev->descriptor.idProduct == idProduct) return dev;
		} 
	} 
}