# Borderland Connection Game
This repository contains [Zephyr-RTOS](https://www.zephyrproject.org/) based application for [Borderland](https://bltrack.slite.com/api/s/channel/7jfLAFr2r9KnYs2BnDFZDZ/Brainstorming) [Connection Game](https://bltrack.slite.com/api/s/channel/7jfLAFr2r9KnYs2BnDFZDZ/Brainstorming)

### Hardware
E73-TBB development board with QFAA nRF52832 chip

### How to build
```
west build
```
or
```
cmake -Bbuild && ninja -C build
```
... which either way results in *./build/zephyr/zephyr.hex* hex-file to be flashed.
