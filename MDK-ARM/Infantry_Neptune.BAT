SET PATH=D:\Program Files\Keil_v5\ARM\ARMCC\Bin;C:\Program Files\PlasticSCM5\server;C:\Program Files\PlasticSCM5\client;C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.3\bin;C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v11.3\libnvvp;C:\Program Files (x86)\Common Files\Intel\Shared Libraries\redist\intel64_win\compiler;C:\Program Files (x86)\Common Files\Oracle\Java\javapath;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Windows\System32\OpenSSH\;D:\Program Files\Polyspace\R2020b\runtime\win64;D:\Program Files\Polyspace\R2020b\bin;D:\Program Files\Polyspace\R2020b\polyspace\bin;C:\Program Files\Git\cmd;C:\Program Files\NVIDIA Corporation\Nsight Compute 2021.1.0\;C:\Program Files\PuTTY\;C:\Program Files (x86)\Windows Kits\8.1\Windows Performance Toolkit\;D:\Program Files (x86)\Tencent\微信web开发者工具\dll;C:\Users\GDDG8\anaconda3;C:\Users\GDDG8\anaconda3\Library\mingw-w64\bin;C:\Users\GDDG8\anaconda3\Library\usr\bin;C:\Users\GDDG8\anaconda3\Library\bin;C:\Users\GDDG8\anaconda3\Scripts;C:\Users\GDDG8\AppData\Local\Microsoft\WindowsApps;D:\Program_Files\Microsoft VS Code\bin;C:\Users\GDDG8\mingw64\bin;C:\Program Files\LOVE;D:\@TinyPrograms\@BBDown;C:\Program Files\wkhtmltopdf\bin;C:\Users\GDDG8\MYenv;D:\@TinyPrograms\QtScrcpy\QtScrcpy-win-x86-v1.7.1;
SET CPU_TYPE=STM32F405RGTx
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=Infantry_Neptune
SET CPU_CLOCK=0x0A037A00
"D:\Program Files\Keil_v5\ARM\ARMCC\Bin\ArmAsm" --Via "infantry_neptune\startup_stm32f405xx._ia"
"D:\Program Files\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "infantry_neptune\main.__i"
"D:\Program Files\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "infantry_neptune\gpio.__i"
"D:\Program Files\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "infantry_neptune\freertos.__i"
"D:\Program Files\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "infantry_neptune\adc.__i"
"D:\Program Files\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "infantry_neptune\can.__i"
"D:\Program Files\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "infantry_neptune\dac.__i"
"D:\Program Files\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "infantry_neptune\dma.__i"
"D:\Program Files\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "infantry_neptune\i2c.__i"
"D:\Program Files\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "infantry_neptune\spi.__i"
"D:\Program Files\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "infantry_neptune\tim.__i"
"D:\Program Files\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "infantry_neptune\usart.__i"
"D:\Program Files\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "infantry_neptune\stm32f4xx_it.__i"
"D:\Program Files\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "infantry_neptune\stm32f4xx_hal_msp.__i"
"D:\Program Files\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "infantry_neptune\stm32f4xx_hal_timebase_tim.__i"
"D:\Program Files\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "infantry_neptune\stm32f4xx_hal_adc.__i"
"D:\Program Files\Keil_v5\ARM\ARMCC\Bin\ArmLink" --Via "Infantry_Neptune\Infantry_Neptune.lnp"
