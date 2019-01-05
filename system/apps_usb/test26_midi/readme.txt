Based on incredible MIOS project by Thorsten Klose: https://github.com/polluxsynth/mios32.git

connections:

MidiOut p4   -> LA104.3V (inverse logic)
        p5   -> LA104.P1 (TX) through 220ohm resistor (to limit the current through opto coupler)

MidiIn  p4   -> LA104.3V (inverse logic)
        p5   -> LA104.P2 (RX) through 5k ohm resistor (to limit the current pn input pin protection diodes when reverse voltage is applied)
        p5   -> MidiIn.p4 through 1k ohm resistor (to form current loop between p4&p5)


        

 