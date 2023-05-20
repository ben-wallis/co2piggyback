class CM1106CPassiveSensor : public Component, public Sensor, public UARTDevice {
    public:
    CM1106CPassiveSensor(UARTComponent *parent) : UARTDevice(parent) {}

    float get_setup_priority() const override { return esphome::setup_priority::DATA; }

    // CM11060-C Datasheet: https://en.gassensor.com.cn/Product_files/Specifications/CM1106-C%20Single%20Beam%20NDIR%20CO2%20Sensor%20Module%20Specification.pdf

    enum: unsigned int {
        PREHEATING = 1U,
        SENSOR_ERROR = 2U, 
        OVER_RANGE = 4U,
        UNDER_RANGE = 8U,
        NON_CALIBRATED = 16U,
        LIGHT_AGING = 32U,
        DRIFT = 64U,
        RESERVED = 128U,
    };

    // The number of seconds that the device sends out a ppm reading
    const int DEVICE_UPDATE_SECS = 5;

    // The desired update frequency emitted to ESPHome
    const int TARGET_UPDATE_SECS = 60;
    
    const int UPDATE_DIVISOR = TARGET_UPDATE_SECS / DEVICE_UPDATE_SECS;

    // Initialise the count so that a reading is taken on the first loop after boot
    int count = UPDATE_DIVISOR - 1;

    void loop() override {
        while (available()) {
            // Wait for the 0x16 0x05 0x01 preamble indicating a CO2 reading response
            if(read() == 0x16 && read() == 0x05 && read() == 0x01) {
                int df1 = read();
                int df2 = read();
                int df3 = read();
                
                int ppm = df1 * 256 + df2;

                bool preheating = (df3 & PREHEATING);
                bool sensor_error = (df3 & SENSOR_ERROR) > 0;
                bool over_range = (df3 & OVER_RANGE) > 0;
                bool under_range = (df3 & UNDER_RANGE) > 0;
                bool non_calibrated = (df3 & NON_CALIBRATED) > 0;
                bool light_aging = (df3 & LIGHT_AGING) > 0;
                bool drift = (df3 & drift) > 0;

                // TODO: Log these error states

                // Only publish updates every TARGET_UPDATE_SECS
                if (count++ % UPDATE_DIVISOR == 0) {
                    publish_state(ppm);
                }                
            }
        }   
    }
};