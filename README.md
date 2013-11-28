# node-lpt

High level binding to deal with parallel ports.

**Important**: It works only under linux and uses parport driver,
so you might need to compile kernel with required drivers (for gentoo mostly).

## Installation

    $ npm install lpt

## Usage

```javascript
var lpt = require('lpt')
  , port = new lpt.Port(0) // open /dev/parport0

console.log(port.data) // read data register
console.log(port.control) // read control register
console.log(port.status.busy) // get port status

port.data = 123 // write data register
port.control = 123 // write control register
```

## API

`Port` class provides next fields:

| Field           | Type | Description                    | Read/Write |
| --------------- | ---- | ------------------------------ |:----------:|
| data            | byte | Data register                  | R/W        |
| control         | byte | Control register               | R/W        |
| status.ack      | bool | ACK pin (10)                   | R          |
| status.busy     | bool | /BUSY pin (11) *               | R          |
| status.error    | bool | ERROR pin (15)                 | R          |
| status.select   | bool | /SELECT pin (17)               | R          |
| status.paperOut | bool | PAPEROUT pin (12)              | R          |

*Note that /BUSY pin is inverted by `node-lpt` itself

## License

MIT
