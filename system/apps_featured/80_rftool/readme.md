# RF Toolkit

Ultimate toolkit for hacking OOK/ASK wireless devices (weather stations, wireless keys...) using CC1101 transceiver
- Hardware / software project running on LA104 (Arm M3 STM32F103)
- useful for capturing weather station signals / all wireless devices using OOK/ASK modulation
- synthesizing fake packets and sending them back to weather stations
- web usb analyser running in web browser with RTL433 library (very popular library for decoding ook modulations)
- spectrum analyser

[![LA104 RF Toolkit](https://img.youtube.com/vi/Gwyi00NKBNg/0.jpg)](https://www.youtube.com/watch?v=Gwyi00NKBNg "LA104 RF Toolkit")

## CC1101 connections


```
         VDD   [1] 2    VDD  -> +3V
P1 <-     SI    3  4    SCK  -> P2
CH4 <-    SO    5  6    GDO2
P3 <-    /CS    7  8    GDO0 -> P4
         GND    9 10    GND  -> GND
```

LA104 cable

| Color  | LA104 | CC1101   |
|--------|-------|----------|
| brown  | CH1   |          |
| red    | CH2   |          |
| orange | CH3   |          |
| yellow | CH4   | 5 - SO   |
| green  | GND   | 10 - GND |
| blue   | P1    | 3 - SI   |
| purple | P2    | 4 - SCK  |
| gray   | P3    | 7 - /CS  |
| white  | P4    | 8 - GDO0 |
| black  | 3V+   | 2 - VDD  |

brown - CH1


## Video narration

- But how do we find the right frequency to tune this device? We have more options - you can either try some of those widely used frequencies for short range wireless devices. For example search in range 433 MHz - 434.79 MHz or in range 868 - 870 MHz
- Or you can also use spectrum analyser as you can see in this video. 
- This machine quickly tunes from low to high frequency and measures the power of radiated energy. You can turn on Max hold mode to see the envelope of the strongest signals in your vicinity. This helps you to estimate the frequency we are looking for. Then we just zoom in and re-center the signal until we get the frequency with required precision. In our case one or two decimal places is perfectly sufficient
- If we keep the analyser tuned onto the same frequency all the time, we can also see the shape of demodulated signal, 
- If you do not have access to spectrum analyser, you can still use LA104 to find out the frequency by using the spectrum analyser application which tunes over selected range of requencies and measures the RSSI level.
- LA104 with Radio frequency toolkit can be used not only for capturing of signals from weather stations, but it can be also used for synthesizing artifical packets to fool the weather station. In this case I captured the packet with temperature of 21.2 degrees of Celsius, I modified the temperature to 18.2 and sent the altered packet back into the weather station.
- We can use the same device with the same software for capturing radio signals from wireless keys and for investigating thier security weaknesses. We can quickly differentate between keys which are sending static codes, keys sending static codes with incrementing counter and keys with rolling code which send different codes every time.
- We can see that the last four codes are completly the same with the length of 64 bits, and the only variable part of the other codes is eight bit incrementing counter which is very easy to hack
- This toolkit currently decodes 5 different modulations, but its compatibility can be very easily extended. 
- For this purpose there is WebUSB signal analyser application running in web browser, which shows the radio signals in great detail with the timing diagram in microseconds and offers all necessary tools for identifing the modulation type and helps to design new demodulation modules for RF toolkit. 
- The best part of this application is the integration of RTL433 library which can decode signals from more than 160 various wireless devices including all popular weather stations, wireless keys, motion and door sensors and many other. And yes, all of this runs right in your web browser no matter if you are using windows, linux or mac.
- This tool is also great for replay attacks. Even for the rolling code keys which transmit different code every time they are pressed. Here we can see the codes after pressing all of the buttons - the application clearly identifies codes for locking, unlocking and trunk opening. Right in this application we can examine the demodulated code in form of hexadecimal numbers and see the timing of the signal. At first sight they look like random numbers, but they are just symetrically encrypted with algorithm called AUT-64. We can also store multiple signals and transmit them later to perform the replay attack. 
