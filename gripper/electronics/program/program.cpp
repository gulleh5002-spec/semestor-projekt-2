#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

// ACS712ELCTR-05B-T kan måle op til 5A
// Den har en sensitivity på 185 mV/A

float calibrateZero()
{
    float sum = 0;
    int samples = 100;

    for (int i = 0; i < samples; i++)
    {
        uint16_t raw = adc_read();
        float voltage = raw * 3.3f / 4095.0f;
        sum += voltage;
        sleep_ms(5);
    }

    return sum / samples;
}

float readVoltage()
{
    float sum = 0;
    int samples = 20;

    for (int i = 0; i < samples; i++)
    {
        sum += adc_read();
    }

    float avg = sum / samples;
    return avg * 3.3f / 4095.0f;
}

int main()
{
    // USB kommunikation
    stdio_init_all();
    while (!stdio_usb_connected())
    {
        sleep_ms(100);
    }

    // Initialiser ADC
    adc_init();
    adc_gpio_init(26);
    adc_select_input(0);

    // Current sensor calibration
    printf("Calibrating...\n");
    sleep_ms(2000); // lad sensor stabilisere sig

    float noLoadVoltage = calibrateZero();
    printf("Zero offset: %.3f V\n", noLoadVoltage);

    float sens = 0.185; // V/A

    while (true)
    {
        float voltage = readVoltage();
        // fordi der en spændingsdeler som halvere strømmen derfor 1/2 af sens
        float current = (voltage - noLoadVoltage) / (sens * 0.5f);

        if (current > -0.02 && current < 0.02)
            current = 0;

        printf("Voltage: %.2f V | Current: %.2f A\n", voltage, current);

        sleep_ms(500);
    }
}