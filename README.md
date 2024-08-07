# CH32X035 Helper Libraries

Useful middleware libraries for CH32X035 series.  
要阅读该文件的简体中文版本，请转到[README.zh-cn.md](README.zh-cn.md)。  

## This repository includes

- USB
  - **[KeyboardAndMouse](USB/KeyboardAndMouse)** A simple keyboard and mouse simulation library.
  - **[Queued](USB/Queued)** A queued processing middleware library helps handle command-response style USB communication easier.
- SPI
  - **[Master](SPI/Master)** A simple SPI master mode library.
- I2C
  - **[Master](I2C/Master)** A simple I2C master mode library.
- Link File
  - **[Link_highcode_62k.ld](link_file/Link_highcode_62k.ld)** Provides `__attribute__((section(".highcode")))` (run in RAM, calls`highcode`)feature.
- Startup File
  - **[startup_ch32x035_highcode_user.S](startup_file/startup_ch32x035_highcode_user.S)** Provides `highcode` feature.
  - **[startup_ch32x035_highcode_machine.S](startup_file/startup_ch32x035_highcode_machine.S)** Provides `highcode` feature. Core is running at machine mode.

## License

This library is under **GPLv3** license. Read [LICENSE](LICENSE) for more information.  
![GPLv3 Logo](https://www.gnu.org/graphics/gplv3-with-text-136x68.png)  
