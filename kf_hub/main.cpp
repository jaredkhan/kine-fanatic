/*
 * This is to be run on the 'hub' micro:bit. It will receive the
 * acceleromter data that is being transmitted by the kf_transmitters
 * and output it in (approximately) a csv format over the serial link
 */

#include "MicroBit.h"

#if MICROBIT_BLE_ENABLED 
 #ifdef YOTTA_CFG
  #error "This example needs BLE to be disabled. Use the yotta config.json in the proximit-heart directory to do this"
 #else
  #error "This example needs BLE to be disabled in the microbit config file in the microbit-dal: MicroBitConfig.h"
 #endif
#endif

MicroBit    uBit;

// We only want to recognise data from things running this app,
// have array of indicators to check
ManagedString indicators[] = {
    ManagedString("prose_sen_0_"),
    ManagedString("prose_sen_1_"),
    ManagedString("prose_sen_2_")
};

ManagedString separator = ManagedString(",");
ManagedString newline = ManagedString("\n\r");

// return true if the string starts with the indicator
bool check_for_indicator(ManagedString stringToCheck, ManagedString indicator) {
    int16_t length = indicator.length();
    return (stringToCheck.substring(0, length) == indicator);
}

// return the given string with the indicator removed from the front
ManagedString strip_indicator(ManagedString stringToStrip, ManagedString indicator) {
    int16_t indicator_length = indicator.length();
    int16_t data_length = stringToStrip.length() - indicator_length;
    return stringToStrip.substring(indicator_length, data_length);
}

// Store the 3 accelerations
ManagedString data[] = {
  ManagedString("0,0,0,"),
  ManagedString("0,0,0,"),
  ManagedString("0,0,0,")
  // Trailing commas are correct
};

// Take a string received from the radio
void store_data_in_string(ManagedString receivedString) {
    // Iterate through indicators to check
    for (int i = 0; i < 3; i++) {
        if(check_for_indicator(receivedString, indicators[i])) {
            ManagedString newData = strip_indicator(receivedString, indicators[i]);
            data[i] = newData;
            break; // break out, can only match one of the 3 indicators
        }
    }
}

void onData(MicroBitEvent)
{
    // We recieve data as strings as this allows for
    // easy identification of transmitters
    ManagedString s = uBit.radio.datagram.recv();
    store_data_in_string(s);
}

int main()
{
    // Initialise the micro:bit runtime.
    uBit.init();

    //Setup a hander to run when data is received
    uBit.messageBus.listen(MICROBIT_ID_RADIO, MICROBIT_RADIO_EVT_DATAGRAM, onData);
    uBit.radio.enable();

    // Repeatedly print the strengths to the serial
    while(true) {

        unsigned long time = uBit.systemTime();
        int int_time = time;
        ManagedString time_string = ManagedString(int_time);

        uBit.serial.send(time_string + separator + data[0] + data[1] + data[2] + newline);
        uBit.sleep(10);
    }
}
