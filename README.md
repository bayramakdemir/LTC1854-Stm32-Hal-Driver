# LTC1854-Stm32-Hal-Driver
stm32 hal driver for stm32 devices

USAGE: 
Create LTC1854 device struct
```
LTC1854 ltc1;
```
initialize ltc1854

```
ltc1.spiHandle = &hspi1;
ltc1.tim = &htim10;
ltc1.CONVST_PORT = CONVST_GPIO_Port;
ltc1.CONVST_PIN = CONVST_Pin;
ltc1.BUSY_C_PORT = BUSY_C_GPIO_Port;
ltc1.BUSY_C_PIN = BUSY_C_Pin;
ltc_initialize(&ltc1);
  ```
  
 start one shot 8 channel conversion
 ```
 get_all_channels(&ltc1);
```
