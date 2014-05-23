#include "usb.h"
#include "usb_descriptors.h"

/* Buffer to be used for control requests. */
uint8_t usbd_control_buffer[128];

static int hid_control_request(usbd_device *usbd_dev, struct usb_setup_data *req, uint8_t **buf, uint16_t *len,
			void (**complete)(usbd_device *usbd_dev, struct usb_setup_data *req))
{
	(void)complete;
	(void)usbd_dev;

	switch(req->bmRequestType){
		case 0x81:
			switch(req->bRequest){
				case USB_REQ_GET_DESCRIPTOR:
					if(req->wValue==0x2200){
						*buf = (uint8_t *)hid_report_descriptor;
						*len = sizeof(hid_report_descriptor);
						return 1;
					}else if(req->wValue==0x2100){
						*buf = (uint8_t *)USBD_HID_Desc;
						*len = sizeof(USBD_HID_Desc);
					}
					return 0;
				default:
					return 0;
			}
			break;
		default:
			return 0;
	}

}

static void hid_set_config(usbd_device *usbd_dev, uint16_t wValue)
{
	(void)wValue;
	(void)usbd_dev;

	usbd_ep_setup(usbd_dev, 0x81, USB_ENDPOINT_ATTR_INTERRUPT, 8, NULL);

	usbd_register_control_callback(
				usbd_dev,
				USB_REQ_TYPE_STANDARD | USB_REQ_TYPE_INTERFACE,
				USB_REQ_TYPE_TYPE | USB_REQ_TYPE_RECIPIENT,
				hid_control_request);

	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB_DIV8);
	/* SysTick interrupt every N clock pulses: set reload to N-1 */
	systick_set_reload(99999);
	systick_interrupt_enable();
	systick_counter_enable();
}

usbd_device *my_usb_device;

uint8_t *USBD_HID_GetStruct (uint8_t key0, uint8_t modifier)
{
  static uint8_t HID_Buffer[8];

  HID_Buffer[0] = modifier;
  HID_Buffer[1] = 0;
  HID_Buffer[2] = key0;
  HID_Buffer[3] = 0;
  HID_Buffer[4] = 0;
  HID_Buffer[5] = 0;
  HID_Buffer[6] = 0;
  HID_Buffer[7] = 0;

  return HID_Buffer;
}

void usb_send_packet(const void *buf, int len){
	while(usbd_ep_write_packet(my_usb_device, 0x81, buf, len) == 0);
}

void usb_setup(void)
{

	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_OTGFS);

	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE,
			GPIO9 | GPIO11 | GPIO12);
	gpio_set_af(GPIOA, GPIO_AF10, GPIO9 | GPIO11 | GPIO12);

	my_usb_device = usbd_init(&otgfs_usb_driver, &dev, &config, usb_strings, 3, usbd_control_buffer, sizeof(usbd_control_buffer));
	usbd_register_set_config_callback(my_usb_device, hid_set_config);

	nvic_enable_irq(NVIC_OTG_FS_IRQ);
}

void
otg_fs_isr(void)
{
	usbd_poll(my_usb_device);
}
