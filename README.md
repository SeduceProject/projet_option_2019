# Projet option 2019

## Installation

This project is based on the [platformio](https://platformio.org/install/cli) framework. It can be installed via the *pip* package manager via the following command:

```shell
pip3 install -U platformio
```

To test that the installation worked, try the following command
```shell
pio --version
```

## Project structure

## Compilation

To compile the project, go to the root of the project and run the following command:

```shell
pio run
```

If the build is successful, the produced firmware should be located at the following path:

```shell
.pio/build/esp12e/firmware.elf
```

## Uploading a new firmware


First locate the location of the Serial port. On an unix like system (Linux and MacOS), it should be located in the `/dev` folder. To identify it, unplug the arduino, list the available devices:

```shell
ls /dev # with the arduino is unplugged
```

and then plug the arduino, and list all devices availables in `/dev`:

```shell
ls /dev # with the arduino plugged
```

The device should have appeared. Let say it is `/dev/ttyUSB1`, to upload the firmware on the arduino, run the following command:

```shell
pio run --target upload --upload-port /dev/ttyUSB1
```
