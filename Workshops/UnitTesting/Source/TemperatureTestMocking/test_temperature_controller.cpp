#include "CppUTest/TestHarness.h"
#include "temperature_controller.hpp"
#include "mock_temperature_sensor.hpp"
#include "mock_heater.hpp"

using namespace temperature;
using namespace temperature::test;

// ============================================================================
// Controller Initialization
// ============================================================================

TEST_GROUP(TemperatureControllerInit) {
    MockTemperatureSensor* sensor;
    MockHeater* heater;
    TemperatureController* controller;

    void setup() override {
        sensor = new MockTemperatureSensor();
        heater = new MockHeater();
        controller = new TemperatureController(*sensor, *heater);
    }

    void teardown() override {
        delete controller;
        delete heater;
        delete sensor;
    }
};

TEST(TemperatureControllerInit, DefaultSetpointIsTwenty) {
    DOUBLES_EQUAL(20.0, controller->getSetpoint(), 0.01);
}

TEST(TemperatureControllerInit, StartsNotInFault) {
    CHECK_FALSE(controller->isInFault());
}

TEST(TemperatureControllerInit, HeaterStartsOff) {
    CHECK_FALSE(heater->isOn());
}

// ============================================================================
// Normal Operation
// ============================================================================

TEST_GROUP(TemperatureControllerNormal) {
    MockTemperatureSensor* sensor;
    MockHeater* heater;
    TemperatureController* controller;

    void setup() override {
        sensor = new MockTemperatureSensor();
        heater = new MockHeater();
        
        ControllerConfig config;
        config.setpoint = 20.0F;
        config.hysteresis = 1.0F;
        
        controller = new TemperatureController(*sensor, *heater, config);
    }

    void teardown() override {
        delete controller;
        delete heater;
        delete sensor;
    }
};

TEST(TemperatureControllerNormal, TurnsHeaterOnWhenTooCold) {
    sensor->setTemperature(15.0F);  // Well below setpoint
    
    controller->update();
    
    CHECK_TRUE(heater->isOn());
}

TEST(TemperatureControllerNormal, TurnsHeaterOffWhenTooHot) {
    heater->turnOn();  // Start with heater on
    sensor->setTemperature(25.0F);  // Well above setpoint
    
    controller->update();
    
    CHECK_FALSE(heater->isOn());
}

TEST(TemperatureControllerNormal, MaintainsStateWithinHysteresis) {
    // Temperature within hysteresis band: heater state unchanged
    sensor->setTemperature(19.5F);  // 0.5 below setpoint, within hysteresis
    
    controller->update();
    
    CHECK_FALSE(heater->isOn());  // Was off, stays off
}

TEST(TemperatureControllerNormal, HeaterStaysOnWithinHysteresis) {
    heater->turnOn();
    sensor->setTemperature(20.5F);  // 0.5 above setpoint, within hysteresis
    
    controller->update();
    
    CHECK_TRUE(heater->isOn());  // Was on, stays on
}

TEST(TemperatureControllerNormal, ReadsTemperatureOnUpdate) {
    sensor->resetReadCount();
    
    controller->update();
    
    LONGS_EQUAL(1U, sensor->getReadCount());
}

TEST(TemperatureControllerNormal, StoresLastReading) {
    constexpr float TEMPERATURE = 23.5F;
    sensor->setTemperature(TEMPERATURE);
    
    controller->update();
    
    DOUBLES_EQUAL(TEMPERATURE, controller->getLastReading(), 0.01);
}

// ============================================================================
// Setpoint Changes
// ============================================================================

TEST_GROUP(TemperatureControllerSetpoint) {
    MockTemperatureSensor* sensor;
    MockHeater* heater;
    TemperatureController* controller;

    void setup() override {
        sensor = new MockTemperatureSensor();
        heater = new MockHeater();
        controller = new TemperatureController(*sensor, *heater);
    }

    void teardown() override {
        delete controller;
        delete heater;
        delete sensor;
    }
};

TEST(TemperatureControllerSetpoint, CanChangeSetpoint) {
    constexpr float NEW_SETPOINT = 25.0F;
    
    controller->setSetpoint(NEW_SETPOINT);
    
    DOUBLES_EQUAL(NEW_SETPOINT, controller->getSetpoint(), 0.01);
}

TEST(TemperatureControllerSetpoint, NewSetpointAffectsControl) {
    sensor->setTemperature(22.0F);
    controller->update();
    CHECK_FALSE(heater->isOn());  // 22 > 20, heater off
    
    controller->setSetpoint(25.0F);  // Raise setpoint
    controller->update();
    CHECK_TRUE(heater->isOn());  // 22 < 25, heater on
}

// ============================================================================
// Fault Handling
// ============================================================================

TEST_GROUP(TemperatureControllerFault) {
    MockTemperatureSensor* sensor;
    MockHeater* heater;
    TemperatureController* controller;

    void setup() override {
        sensor = new MockTemperatureSensor();
        heater = new MockHeater();
        controller = new TemperatureController(*sensor, *heater);
    }

    void teardown() override {
        delete controller;
        delete heater;
        delete sensor;
    }
};

TEST(TemperatureControllerFault, EntersFaultWhenSensorUnhealthy) {
    sensor->setHealthy(false);
    
    controller->update();
    
    CHECK_TRUE(controller->isInFault());
}

TEST(TemperatureControllerFault, TurnsHeaterOffOnFault) {
    heater->turnOn();
    sensor->setHealthy(false);
    
    controller->update();
    
    CHECK_FALSE(heater->isOn());
}

TEST(TemperatureControllerFault, RecoverFromFault) {
    sensor->setHealthy(false);
    controller->update();
    CHECK_TRUE(controller->isInFault());
    
    sensor->setHealthy(true);
    controller->update();
    CHECK_FALSE(controller->isInFault());
}

TEST(TemperatureControllerFault, DoesNotReadTemperatureWhenUnhealthy) {
    sensor->setHealthy(false);
    sensor->resetReadCount();
    
    controller->update();
    
    LONGS_EQUAL(0U, sensor->getReadCount());
}

// ============================================================================
// Hysteresis Behavior
// ============================================================================

TEST_GROUP(TemperatureControllerHysteresis) {
    MockTemperatureSensor* sensor;
    MockHeater* heater;
    TemperatureController* controller;

    void setup() override {
        sensor = new MockTemperatureSensor();
        heater = new MockHeater();
        
        ControllerConfig config;
        config.setpoint = 20.0F;
        config.hysteresis = 2.0F;  // Larger hysteresis for clear testing
        
        controller = new TemperatureController(*sensor, *heater, config);
    }

    void teardown() override {
        delete controller;
        delete heater;
        delete sensor;
    }
};

TEST(TemperatureControllerHysteresis, TurnsOnBelowLowerThreshold) {
    sensor->setTemperature(17.0F);  // More than 2 below setpoint
    
    controller->update();
    
    CHECK_TRUE(heater->isOn());
}

TEST(TemperatureControllerHysteresis, TurnsOffAboveUpperThreshold) {
    heater->turnOn();
    sensor->setTemperature(23.0F);  // More than 2 above setpoint
    
    controller->update();
    
    CHECK_FALSE(heater->isOn());
}

TEST(TemperatureControllerHysteresis, NoChangeAtExactSetpoint) {
    sensor->setTemperature(20.0F);  // Exactly at setpoint
    
    controller->update();
    CHECK_FALSE(heater->isOn());  // Was off, stays off
    
    heater->turnOn();
    controller->update();
    CHECK_TRUE(heater->isOn());  // Was on, stays on
}

TEST(TemperatureControllerHysteresis, PreventsRapidSwitching) {
    // Simulate temperature oscillating around setpoint
    heater->resetCounts();
    
    sensor->setTemperature(17.0F);  // Cold, heater on
    controller->update();
    
    sensor->setTemperature(19.0F);  // Warming, still in band
    controller->update();
    
    sensor->setTemperature(20.0F);  // At setpoint, still in band
    controller->update();
    
    sensor->setTemperature(21.0F);  // Above setpoint, still in band
    controller->update();
    
    // Heater only turned on once, never turned off
    LONGS_EQUAL(1U, heater->getTurnOnCount());
    LONGS_EQUAL(0U, heater->getTurnOffCount());
}