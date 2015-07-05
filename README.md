# libpubsub-cpp
Publish/Subscribe framework for C++

## Overview
- Topic based Publish/Subscribe framework on top of JSON-RPC notifications
- UDP Broadcast connectors for peer detection
- JSON-RPC Client/Server for subscription management
- JSON-RPC Client/Server for application specific notifications (topics)
- pubsubstub: tool to generate peer stubs based on specification

## Specification Syntax

It is a JSON file containing 2 sections:

- List of topics (containing parameter information)
	- name: name of the topic
	- params: can be ommmited, otherwhise contains parameter names + types
- List of peers (containing publish/subscribe information)
	- name: name of the peer class to be generated
	- publish: list of topics, the peer can publish (can be ommited)
	- subscribe: list of topic, the peer can subscribe (can be ommited)

```
{
	"notifications": [
		{
			"name": "tempChanged",
			"params": {"temp": 23.4}
		},
		{
			"name": "humidityChanged",
			"params": {"humidity": 0.65}
		},
		{
			"name": "airspeedChanged",
			"params": {"speeed": 45.3}
		},
		{
			"name": "shutterChanged",
			"params": {"open": false}
		}
	],
	"peers": [
		{
			"name": "TempSensor",
			"publish": ["tempChanged"]
		},
		{
			"name": "AirspeedSensor",
			"publish": ["airspeedChanged"]
		},
		{
			"name": "HumiditySensor",
			"publish": ["humidityChanged"]
		},
		{
			"name": "Shutter",
			"publish": ["shutterChanged"],
			"subscribe": ["airspeedChanged"]
		},
		{
			"name": "Monitor",
			"subscribe": ["tempChanged", "humidityChanged", 
					"airspeedChanged", "shutterChanged"]
		}
	]
}
```

## Example

1. Call pubsubstub with above topic specification file

## Build from source

The framework currently only works under UNIX based environments.

### Dependencies
**Debian**

```sh
sudo apt-get install libjsonrpccpp-dev libjsonrpccpp-tools
```

**Arch Linux**

For Arch Linux there is a [PKGBUILD provided in the AUR](https://aur.archlinux.org/packages/libjson-rpc-cpp/).

```sh
sudo aura -A libjson-rpc-cpp
```

**Gentoo Linux**

```sh
sudo emerge dev-cpp/libjson-rpc-cpp
```

**Mac OS X**

For OS X a [Brew](http://brew.sh) package is available:
```sh
brew install libjson-rpc-cpp
```

### Compilation
```sh
git clone https://github.com/cinemast/libpubsub-cpp.git
cd libpubsub-cpp
mkdir -p build
cd build
cmake ..
make
sudo make install
sudo ldconfig
```

