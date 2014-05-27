STM32-USB-Keyboard
==================

USB-OTG Keyboard on STM32F4-Discovery

Using [libopencm3](http://libopencm3.org/wiki/Main_Page)

## USART

* PA2: TX
* PA3: RX
* Baudrate: 115200

## Package format

11 Bytes / Package

* 0: 'S'
* 1-8: USB HID Package Bytes
* 9: 'E'
* 10: CRC

## CRC

```python
def crc(data):
    crc = 0
    for i in data:
        crc ^= i
        for i in data:
            if crc & 1:
                crc ^= 0x91
            crc>>=1
    return crc
```

## ST-WAT

ST-Wat is a simple daemon that uses stlinky and the stlink interface to send data to the stm32f4-discovery.

See ```host/``` for code. Use ```make USE_STTERM=1``` to replace USART with stlinky.
