# node-lpt

High-level bindings to parallel port for node.js.

**Important**: this module works only under linux through the `parport` driver,
so you might need to compile kernel or modules (on gentoo mostly).

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

| Field           | Type | Description   | Read/Write | Pin num |
| --------------- | ---- | ------------- |:----------:| -------:|
| data            | byte | Data register | R/W        | 2..9    |
| control.init    | bool | /INIT         | R/W        | 16      |
| control.select  | bool | /SELECT_IN    | R/W        | 17      |
| control.autofd  | bool | /AUTO_FEED    | R/W        | 14      |
| control.strobe  | bool | /STROBE       | R/W        | 1       |
| status.ack      | bool | ACK           | R          | 10      |
| status.busy     | bool | /BUSY *       | R          | 11      |
| status.error    | bool | ERROR         | R          | 15      |
| status.select   | bool | SELECT        | R          | 13      |
| status.paperOut | bool | PAPEROUT      | R          | 12      |

*_Note that /BUSY pin is inverted by `node-lpt` itself_

## License

MIT
