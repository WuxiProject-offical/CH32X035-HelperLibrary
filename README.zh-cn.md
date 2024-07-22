# CH32X035 Helper Libraries

一些适用于CH32X035系列芯片的有用的中间件库。
To read English version of this file，please turn to [README.zh-cn.md](README.zh-cn.md)。  

## 该仓库包含以下内容

- USB
  - **[KeyboardAndMouse](USB/KeyboardAndMouse)** 一个简单的键盘鼠标模拟库。
- SPI
  - **[Master](SPI/Master)** 一个简单的SPI主机模式函数库。
- 链接脚本文件
  - **[Link_highcode_62k.ld](link_file/Link_highcode_62k.ld)** 提供 `__attribute__((section(".highcode")))` （在RAM中运行代码，称`highcode`）的特性。
- 启动汇编文件
  - **[startup_ch32x035_highcode_user.S](startup_file/startup_ch32x035_highcode_user.S)** 提供 `highcode` 的特性。
  - **[startup_ch32x035_highcode_machine.S](startup_file/startup_ch32x035_highcode_machine.S)** 提供 `highcode` 的特性。除此之外，内核运行在机器模式下。

## License

本库使用 **GPLv3** 许可证. 阅读 [LICENSE](LICENSE) 文件以获得更多信息。  
![GPLv3 Logo](https://www.gnu.org/graphics/gplv3-with-text-136x68.png)  
