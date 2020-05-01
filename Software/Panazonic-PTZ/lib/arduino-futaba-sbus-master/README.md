This is a project to implement Futaba's S-BUS protocol. It's partially based on a community project FUTABA_SBUS, but the API is different which suits my particular needs more.
The library supports:
 - as many S-BUS receivers as many UART's on the arduino (3 on Mega for example)
 - event based programming model
 - pass-through with or without data alteration (see examples)
 - no need for custom Serial library
 
Usage
-----

First create the object and call begin on it with a Serial port (standard Arduino).

        FutabaSBUS sbus;
        
        void setup() {
                sbus.begin(Serial);
        }

or

        FutabaSBUS sbus_second;
        
        void setup() {
                sbus_second.begin(Serial2);
        }

You need to call the receive() method on your sbus instances in your main loop. You can also use a timer for that, it's up to you.

You can also enable pass-through passing the second argument for the begin call:

        sbus.begin(Serial, true);

Lastly there's a third argument to supply an alternate buad rate. Some people found that the 100000 baud is not so stable and in the range of 97000 - 99000 they got better results.

        sbus.begin(Serial, false, 99000);


I hope you'll find a use for it. Good luck.


Szabolcs
