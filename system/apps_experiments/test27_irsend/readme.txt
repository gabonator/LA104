Controls samsung TV using IR

Interrupt based non blocking IR transmitter. Connect IR led diode 
through current limiting resistor between GND and P3. Use encoders 
to change volume/channel. F1 turns TV on/off.


// https://github.com/Waelson/Comandos-IR/blob/master/TV%20Samsung.txt
// based on https://github.com/joeferner/stm32-wifi-ir
// and https://github.com/z3t0/Arduino-IRremote/blob/master/ir_Samsung.cpp
// resources: http://elektrolab.wz.cz/katalog/samsung_protocol.pdf
// (nec protocol, different timing) https://www.sbprojects.net/knowledge/ir/nec.php
// https://codebender.cc/sketch:81232#IR%20Send%20Samsung%20TV%20Power.ino
// http://irdb.tk/codes/
