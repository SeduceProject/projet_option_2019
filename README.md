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

Wether you want to upload via Serial port or Over the Air, you need to comment or uncomment the oath line in the `platformio.ini` file. This line is needed in case of an over the air upload secured by a password.

### Serial upload
First locate the location of the Serial port. You can list the available ports thanks to the following command:

```shell
pio device list
```

The device should have appeared. Let say it is `/dev/ttyUSB1`, to upload the firmware on the arduino, run the following command:

```shell
pio run --target upload --upload-port /dev/ttyUSB1
```
### Over the Air
To update Over The Air, you first need to have uploaded the firmware once via Serial port.
Get the IP adress of the card. It is normally printed on the card's Serial Monitor when just connected to the Wifi. Otherwise, you can list available services thanks to the following command:

```shell
pio device list --mdns --logical
```

The service should have appeared, with a type `_arduino._tcp.local`. Then, let's say our IP adress is `192.168.0.46`. We can now upload Over the Air thanks to the following command:

```shell
pio run --target upload --upload-port 192.168.0.46
```

## Testing the SNMP server

To test the SNMP server, get your card's IP. You can then enter in a terminal window the following command:

```shell
snmpget -v 1 -c public 192.168.0.46 1.3.6.1.4.1.5.1
```

It will return the temperature of the first sensor times 100.

In order to get the temperature of the sensor n°N, change the last number of the OID, as following : `1.3.6.1.4.1.5.N`. You can combine several requests by adding OIDs at the end of the line, as for example :

```shell
nmpget -v 1 -c public 192.168.0.46 1.3.6.1.4.1.5.1 1.3.6.1.4.1.5.2 1.3.6.1.4.1.5.3 1.3.6.1.4.1.5.4 1.3.6.1.4.1.5.5 1.3.6.1.4.1.5.6 1.3.6.1.4.1.5.7 1.3.6.1.4.1.5.8 1.3.6.1.4.1.5.9 1.3.6.1.4.1.5.10 1.3.6.1.4.1.5.11 1.3.6.1.4.1.5.12 1.3.6.1.4.1.5.13 1.3.6.1.4.1.5.14 1.3.6.1.4.1.5.15 1.3.6.1.4.1.5.16
```

You can also test by running `python ./src/requests.py`, but the script, which will return all 16 temperatures, but it is not optimized for the moment.