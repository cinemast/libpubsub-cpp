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

```json
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

### 1. Install the dependencies

The framework has [libjson-rpc-cpp](https://github.com/cinemast/libjson-rpc-cpp) 
as major dependency. By installing it, all further dependencies will be installed.

See [here](https://github.com/cinemast/libjson-rpc-cpp#install-the-framework) how to install libjson-rpc-cpp on UNIX environments.

### 2. Compile libpubsub-cpp
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

