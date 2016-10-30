/*
 * This is to be run on the transmitter micro:bits. They'll then output their
 * accelerometer data over BLE (using the radio api)
 */
 
#include "MicroBit.h"
 
MicroBit    uBit;
int current_mode = -1; // initialise to invalid mode

// Remember previous vals for acceleration
float past_x = 0;
float past_y = 0;
float past_z = 0;

// For the beacon to accept us we'll need to identify ourselves with these strings
ManagedString indicators[] = {
    ManagedString("prose_sen_0_"),
    ManagedString("prose_sen_1_"),
    ManagedString("prose_sen_2_")
};

ManagedString separator = ManagedString(",");

void set_mode(int mode) {
    // Check if the mode is new, otherwise ignore to improve performance
    if (current_mode != mode) {
        // Update the mode
        current_mode = mode;
        // Print the updated mode to the display
        uBit.display.print(ManagedString(mode));
    }
}

void broadcast_accelerometer_data() {
    //time step
    double delta_T = 5.0;
    double alpha = delta_T / (delta_T + 100.0);
    
    
    //get new vals
    double x = past_x + alpha * (uBit.accelerometer.getX() - past_x );
    double y = past_y + alpha * (uBit.accelerometer.getY() - past_y );
    double z = past_z + alpha * (uBit.accelerometer.getZ() - past_z );
    
    int pitch = uBit.accelerometer.getPitch();
    int roll = uBit.accelerometer.getRoll();
    
    double acc_x = uBit.accelerometer.getX() - x;
    double acc_y = uBit.accelerometer.getY() - y;
    double acc_z = uBit.accelerometer.getZ() - z;
    
    ManagedString acc_x_string = ManagedString((int) acc_x);
    ManagedString acc_y_string = ManagedString((int) acc_y);
    ManagedString acc_z_string = ManagedString((int) acc_z);

    // Construct the broadcast
    uBit.radio.datagram.send(indicators[current_mode] + acc_x_string + separator + acc_y_string + separator + acc_z_string + separator);
    
    // Update the previous values for next time
    past_x = x;
    past_y = y;
    past_z = z;
}

int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();
    uBit.radio.enable();
    uBit.radio.setTransmitPower(2);
    
    set_mode(0);

    while(1)
    {
        if (uBit.buttonA.isPressed()) {
            // Decrement mode
            if (current_mode > 0) {
                set_mode(current_mode - 1);
            }
            // Sleep to avoid incrementing too quickly
            uBit.sleep(300);
        } else if (uBit.buttonB.isPressed()) {
            // Increment mode
            if (current_mode < 2) {
                set_mode(current_mode + 1);
            }
            // Sleep to avoid incrementing too quickly
            uBit.sleep(300);
        }

        broadcast_accelerometer_data();

        // Slow down so memory lasts a little longer
        uBit.sleep(10);
    }
}