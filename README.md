# esp32-tiny
esp32-tiny is a bare-metal library developed for the ESP32 WROOM-32 module. It provides direct hardware access, bypassing the ESP-IDF FreeRTOS framework and Arduino libraries, to offer fundamental low-level peripheral interaction.
This library was created as a research and educational resource for exploring the intricacies of complex microcontrollers.
Currently, it utilizes the ESP-IDF toolchain, which introduces an inherent watchdog timer. Consequently, a specific ESP-IDF function is required for its deactivation.
Please be aware that this project, developed and maintained by a single individual, may contain bugs.