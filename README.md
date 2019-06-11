# EspIrrigation

[![Build Status](https://travis-ci.org/lutzb91/espirrigation.svg?branch=master)](https://travis-ci.org/lutzb91/espirrigation)

## Architecture
The following image shows the internal Architecture of espirrigation.

![Architecture](https://raw.githubusercontent.com/lutzb91/espirrigation/master/drawio/system.png)

## Nodes
EspIrrigation provides the following nodes as ranges from `1` to `NUM_VALVES`. `n` is the corresponding valve.

- `valve/open_n`: (true|false)
- `valve/duration_n`: time in seconds
- `valve/order_n`: [0-NUM_VALVES]

### open
The `open` nodes define whether the corresponding valves are opened or closed. If the node is set to `true` the valve is opened for the given time, else if the node is set to `false` the valve closes immediately.

### duration
The `duration` nodes define the duration a valve is opened. These durations apply in auto mode as well as in manual mode.

### order
The `order` nodes define the order the valves are opened in auto mode. Only the first order elements greater than 0 are taken into account.

Example:
- `valve/order_1`: 2
- `valve/order_2`: 1
- `valve/order_3`: 0
- `valve/order_4`: 3

In auto mode the first valve opened is valve number 2, followed by valve number 1. All order elements after `order_3` are not taken into account as `order_3` is 0 and thus stops the auto mode.