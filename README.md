## Kine:fanatic
### Bluetooth LE collection of micro:bit accelerometer data

This code was developed as part of the CUES Hackathon 2016. The project is classification of body motions based on accelerometer data. This is implemented by 3 micro:bits attached to the arm at the shoulder, elbow and wrist transmitting their accelerometer data (using `kf_transmitter.cpp`) to the hub which sends all the data over the serial output to a computer. The further processing was prototyped in Matlab (code not in this repo).

The code uses the Lancaster University [microbit-dal](https://github.com/lancaster-university/microbit-dal) and bases code on [their proximity heart sample](https://github.com/lancaster-university/microbit-samples/tree/master/source/examples/proximity-heart). If you want to run this code, I suggest you look at their examples first to understand how to use the tools.