# node-lpt

High-level bindings to parallel port for node.js.

**Important**: this module works only under linux through the `parport` driver,
so you might need to compile kernel or modules (on gentoo mostly).

## Installation

    $ npm install lpt

## Usage

```javascript
var lpt = require('lpt')
  , port = new lpt.Port(0, 'byte', false) // open /dev/parport0 with forced `byte` mode set
  // , port = new lpt.Port(0) // the same

console.log(port.data) // read data register
console.log(port.status.busy) // get /BUSY pin status

port.data = 123 // write data register
port.control.init = true // write to control register
```

## API

Constructor receives the following parameters:
  * `port number`
  * `parallel port mode` - optional. Can be:
    * `spp`
    * `nibble`
    * `byte` - default
    * `epp`
    * `ecp`
  * `negotiate port` - optional. If `true` will try IEEE 1284 compliant mode negotiation.

`Port` class provides next fields:

| Field           | Type | Description   | Read/Write | Pin num |
| --------------- | ---- | ------------- |:----------:| -------:|
| data            | byte | Data register | R/W        | 2..9    |
| control.init    | bool | INIT          | R/W        | 16      |
| control.select  | bool | /SELECT_IN *  | R/W        | 17      |
| control.autofd  | bool | /AUTO_FEED *  | R/W        | 14      |
| control.strobe  | bool | /STROBE *     | R/W        | 1       |
| status.ack      | bool | ACK           | R          | 10      |
| status.busy     | bool | /BUSY *       | R          | 11      |
| status.error    | bool | ERROR         | R          | 15      |
| status.select   | bool | SELECT        | R          | 13      |
| status.paperOut | bool | PAPEROUT      | R          | 12      |
| -               | -    | Ground        | -          | 18..25  |

*_Note that this pins are inverted by `node-lpt` itself_

**_You can read control pins only after you wrote something to it. Before that moment you can't rely on pin status_

## Useful links

* [Standard Parallel Port](http://users.utcluj.ro/~baruch/media/siee/labor/Standard-Parallel-Port.pdf)
* [Using in Linux](http://www.labbookpages.co.uk/electronics/parallelPort.html)
* [Parallel port interfacing made easy](http://www.epanorama.net/circuits/parallel_output.html)
* [ECP Description](http://retired.beyondlogic.org/ecp/ecp.htm)

## License

MIT
