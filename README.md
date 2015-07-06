*Warning: this software is currently in an experimental phase. You will find stable releases in the [github releases](https://github.com/cinemast/libpubsub-cpp/releases) section, as soon as they are ready.*

# libpubsub-cpp
libpubsub-cpp is a [topic based](https://en.wikipedia.org/wiki/Publish%E2%80%93subscribe_pattern#Message_filtering) publish/subscribe framework for C++. It uses
JSON-RPC on top of HTTP as transport layer to deliver topic notifications.

![libjson-rpc-cpp logo](https://github.com/cinemast/libpubsub-cpp/blob/master/dev/logo.png?raw=true)

## Overview
- Topic based Publish/Subscribe framework on top of JSON-RPC notifications
- UDP Broadcast connectors for peer detection
- JSON-RPC Client/Server for subscription management
- JSON-RPC Client/Server for application specific notifications (topics)
- pubsubstub: tool to generate peer stubs based on specification

## Overlay network / Subscription management
- Each peer is a JSON-RPC server and client.
- If a peer wants to publish a new topic type, it broadcasts it through
the network (pubsub.topics).
- If a peer wants to subscribe for a new topic, it broadcasts its intent through
the network (pubsub.interest).
- Broadcasted intents for subscription/publishment are immediately answered by
all affected peers with topic offers (pubsub.offertopic).
- Subscription: call pubsub.subscribe, which returns a subscription id.
- Revocation: call pubsub.unsubscribe with subscription id.

So the interface each peer offers for subscription management can be described
using the [libjson-rpc-cpp IDL](https://github.com/cinemast/libjson-rpc-cpp#step-1-writing-the-specification-file):

```json
[
	{
		"name": "pubsub.interest",
		"params": {"ip": "192.168.13.1", "topic": "sometopic"}
	},
	{
		"name": "pubsub.topics",
		"params": {"ip": "192.168.13.1", "topics": ["sometopic", "sometopic2"]}
	},
	{
		"name": "pubsub.offertopic",
		"params": {"ip": "192.168.13.1", "topics": ["sometopic", "sometopic2"]}
	},
	{
		"name": "pubsub.subscribe",
		"params": {"ip": "192.168.13.1", "notification": "sometopic"},
		"returns": "b65dc7fa-a1ac-4a17-ac12-c693da8131f5"
	},
	{
		"name": "pubsub.unsubscribe",
		"params": {"notificationId": "b65dc7fa-a1ac-4a17-ac12-c693da8131f5"},
		"returns": true
	}
]
```

By implementing these JSON-RPC methods via Broadcast and HTTP for each peer
as client and server, also other languages can be used to join the Publish/Subscribe
network of libpubsub-cpp.

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
	"topics": [
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

The framework currently only supports UNIX based environments.

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

