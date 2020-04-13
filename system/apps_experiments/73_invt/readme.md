# Motor inverter RS485 interface

Supports Invt Good drive inverters with MAX485 RS485 interface, serial config 19200 bauds, 8 data bits, even parity, one stop bit (8E1).

Connection (works with 3V internal source):

| LA104 PIN   | LA104        | RS485 adapter   |
|-------------|--------------|-----------------|
| GND         | GND          | GND             |
| P1          | UART-TX      | DI              |
| P2          | UART-RX      | RO              |
| P3          | Direction    | DE              |
| P4          | Direction    | RE              |
| VCC         | 3V           | VCC             |
|             |              | A (RS485+)      |
|             |              | B (RS485-)      |
