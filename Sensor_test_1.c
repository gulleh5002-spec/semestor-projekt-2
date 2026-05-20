// SDA - GPIO8
// SCL - GPIO9
// LED - GPIO15
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

//  I2C CONFIG
#define I2C_PORT    i2c0
#define I2C_SDA     8
#define I2C_SCL     9
#define I2C_FREQ    400000
#define AS7341_ADDR 0x39

#define LED_PIN   15 


// Minimum percentage of total light for each color to be detected
#define RED_PCT_MIN    38   
#define GREEN_PCT_MIN  26   
#define BLUE_PCT_MIN   30   

#define TOTAL_MIN_SCORE  2000


// Sensor sensitivity
#define SENSOR_GAIN    3
#define SENSOR_ATIME   14
#define SENSOR_ASTEP_L 0xE7
#define SENSOR_ASTEP_H 0x03


// Registers
#define REG_ENABLE   0x80
#define REG_ATIME    0x81
#define REG_ASTEP_L  0xCA
#define REG_ASTEP_H  0xCB
#define REG_CFG0     0xA9
#define REG_CFG1     0xAA
#define REG_CFG6     0xAF
#define REG_STATUS2  0xA3
#define REG_ID       0x92
#define REG_ASTATUS  0x94
#define REG_CH0_L    0x95
#define REG_CH1_L    0x97
#define REG_CH2_L    0x99
#define REG_CH3_L    0x9B
#define REG_CH4_L    0x9D
#define REG_CH5_L    0x9F


// Types
typedef struct {
    uint16_t f1, f2, f3, f4, f5, f6, f7, f8;
    uint16_t clear, nir;
} as7341_data_t;

typedef enum {
    COLOR_RED   = 0,
    COLOR_GREEN = 1,
    COLOR_BLUE  = 2,
} detected_color_t;

// Led
static void led_on(void) {
    gpio_put(LED_PIN, 1);
    sleep_ms(0.1);
}

static void led_off(void) {
    gpio_put(LED_PIN, 0);
}

// I2C primitives
static void wr(uint8_t reg, uint8_t val) {
    uint8_t buf[2] = {reg, val};
    i2c_write_blocking(I2C_PORT, AS7341_ADDR, buf, 2, false);
}

static uint8_t rd(uint8_t reg) {
    uint8_t val = 0;
    i2c_write_blocking(I2C_PORT, AS7341_ADDR, &reg, 1, true);
    i2c_read_blocking(I2C_PORT, AS7341_ADDR, &val, 1, false);
    return val;
}

static uint16_t rd16(uint8_t reg) {
    uint8_t buf[2] = {0};
    i2c_write_blocking(I2C_PORT, AS7341_ADDR, &reg, 1, true);
    i2c_read_blocking(I2C_PORT, AS7341_ADDR, buf, 2, false);
    return (uint16_t)(buf[1] << 8) | buf[0];
}

static void wr_block(uint8_t reg, const uint8_t *data, uint8_t len) {
    uint8_t buf[32];
    buf[0] = reg;
    for (int i = 0; i < len && i < 31; i++) buf[i + 1] = data[i];
    i2c_write_blocking(I2C_PORT, AS7341_ADDR, buf, len + 1, false);
}

// SMUX
static const uint8_t SMUX_F1_F4[20] = {
    0x30, 0x01, 0x00, 0x00, 0x00, 0x42,
    0x00, 0x00, 0x50, 0x00, 0x00, 0x00,
    0x20, 0x04, 0x00, 0x30, 0x01, 0x50,
    0x00, 0x06
};

static const uint8_t SMUX_F5_F8[20] = {
    0x00, 0x00, 0x00, 0x40, 0x02, 0x00,
    0x10, 0x03, 0x50, 0x10, 0x03, 0x00,
    0x00, 0x00, 0x00, 0x24, 0x00, 0x50,
    0x00, 0x06
};

static void smux_run_cmd(uint8_t cmd) {
    uint8_t en = rd(REG_ENABLE);
    wr(REG_ENABLE, en & ~0x02);
    sleep_ms(10);
    wr(REG_CFG6, (cmd & 0x03) << 3);
    sleep_ms(2);
    en = rd(REG_ENABLE);
    wr(REG_ENABLE, en | 0x10);
    for (int i = 0; i < 100; i++) {
        sleep_ms(2);
        if (!(rd(REG_ENABLE) & 0x10)) break;
    }
    sleep_ms(10);
}

static void smux_write_config(const uint8_t cfg[20]) {
    smux_run_cmd(0);
    smux_run_cmd(1);

    uint8_t en = rd(REG_ENABLE);
    wr(REG_ENABLE, en & ~0x02);
    sleep_ms(5);

    uint8_t cfg0 = rd(REG_CFG0);
    wr(REG_CFG0, cfg0 | 0x10);
    sleep_ms(5);
    wr_block(0x00, cfg, 20);
    sleep_ms(5);
    cfg0 = rd(REG_CFG0);
    wr(REG_CFG0, cfg0 & ~0x10);
    sleep_ms(5);

    smux_run_cmd(2);
}

// Measurement
static bool wait_avalid(void) {
    rd(REG_STATUS2);
    for (int i = 0; i < 300; i++) {
        sleep_ms(5);
        if (rd(REG_STATUS2) & 0x40) return true;
    }
    printf("  [timeout]\n");
    return false;
}

static bool measure(uint16_t ch[6]) {
    uint8_t en = rd(REG_ENABLE);
    wr(REG_ENABLE, en & ~0x02);
    sleep_ms(5);
    en = rd(REG_ENABLE);
    wr(REG_ENABLE, en | 0x02);

    if (!wait_avalid()) return false;

    rd(REG_ASTATUS);
    ch[0] = rd16(REG_CH0_L);
    ch[1] = rd16(REG_CH1_L);
    ch[2] = rd16(REG_CH2_L);
    ch[3] = rd16(REG_CH3_L);
    ch[4] = rd16(REG_CH4_L);
    ch[5] = rd16(REG_CH5_L);

    en = rd(REG_ENABLE);
    wr(REG_ENABLE, en & ~0x02);
    sleep_ms(5);
    return true;
}

// init
bool as7341_init(void) {
    wr(REG_ENABLE, 0x00);
    sleep_ms(50);
    wr(REG_ENABLE, 0x01);
    sleep_ms(10);

    uint8_t id = rd(REG_ID);
    if ((id & 0xFC) != 0x24) {
        printf("AS7341: bad ID 0x%02X\n", id);
        return false;
    }

    wr(REG_CFG0, rd(REG_CFG0) & ~0x10);
    sleep_ms(2);

    wr(REG_ATIME,   SENSOR_ATIME);
    wr(REG_ASTEP_L, SENSOR_ASTEP_L);
    wr(REG_ASTEP_H, SENSOR_ASTEP_H);
    wr(REG_CFG1,    SENSOR_GAIN);

    smux_run_cmd(0);

    printf("AS7341: init OK (ID=0x%02X)\n", id);
    printf("Min pct -> Red:%u%%  Green:%u%%  Blue:%u%%  MinTotal:%u\n\n",
           RED_PCT_MIN, GREEN_PCT_MIN, BLUE_PCT_MIN, TOTAL_MIN_SCORE);
    return true;
}

// Read all channels
bool as7341_read(as7341_data_t *d) {
    uint16_t p1[6], p2[6];

    // led_on();

    smux_write_config(SMUX_F1_F4);
    if (!measure(p1)) { led_off(); printf("pass1 failed\n"); return false; }
    d->f1    = p1[0];
    d->f2    = p1[1];
    d->f3    = p1[2];
    d->f4    = p1[3];
    d->clear = p1[4];
    d->nir   = p1[5];

    smux_write_config(SMUX_F5_F8);
    if (!measure(p2)) { led_off(); printf("pass2 failed\n"); return false; }
    d->f5    = p2[0];
    d->f6    = p2[1];
    d->f7    = p2[2];
    d->f8    = p2[3];
    d->clear = (d->clear + p2[4]) / 2;
    d->nir   = (d->nir   + p2[5]) / 2;

    // led_off();
    return true;
}

// Color detection
detected_color_t detect_color(const as7341_data_t *d,
                               uint32_t *out_r, uint32_t *out_g, uint32_t *out_b,
                               uint32_t *out_rpct, uint32_t *out_gpct, uint32_t *out_bpct) {
    uint32_t r = ((uint32_t)d->f7 + d->f8) * 0.65 ;   // 0.7 - 555+590nm = red (F7 is very strong, reduced it for better measurments)
    uint32_t g = ((uint32_t)d->f4 + d->f5)  ;  // 2 515+555nm = green
    uint32_t b = ((uint32_t)d->f2 + d->f3) * 1.3 ;   // 1.6 - 445+480nm = blue
    uint32_t total = r + g + b;

    *out_r = r;
    *out_g = g;
    *out_b = b;

    // Incase of too low score:
    if (total < TOTAL_MIN_SCORE) {
        *out_rpct = 0;
        *out_gpct = 0;
        *out_bpct = 0;
        if (r >= g && r >= b) return COLOR_RED;
        if (g >= r && g >= b) return COLOR_GREEN;
        return COLOR_BLUE;
    }

    // Percentages
    uint32_t rpct = (r * 100) / total;
    uint32_t gpct = (g * 100) / total;
    uint32_t bpct = (b * 100) / total;

    *out_rpct = rpct;
    *out_gpct = gpct;
    *out_bpct = bpct;

    // Thresholds
    if (rpct >= RED_PCT_MIN   && rpct >= gpct && rpct >= bpct) return COLOR_RED;
    if (gpct >= GREEN_PCT_MIN && gpct >= rpct && gpct >= bpct) return COLOR_GREEN;
    if (bpct >= BLUE_PCT_MIN  && bpct >= rpct && bpct >= gpct) return COLOR_BLUE;

    // In case of no thresholds met:
    if (rpct >= gpct && rpct >= bpct) return COLOR_RED;
    if (gpct >= rpct && gpct >= bpct) return COLOR_GREEN;
    return COLOR_BLUE;
}

const char* color_name(detected_color_t c) {
    switch(c) {
        case COLOR_RED:   return "Red";
        case COLOR_GREEN: return "Green";
        case COLOR_BLUE:  return "Blue";
        default:          return "Red";
    }
}

const char* color_binary(detected_color_t c) {
    switch(c) {
        case COLOR_RED:   return "00";
        case COLOR_GREEN: return "01";
        case COLOR_BLUE:  return "10";
        default:          return "00";
    }
}

int main() {
    stdio_init_all();
    sleep_ms(4000);

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    gpio_put(LED_PIN, 1);   // LED konstant tændt

    printf("=== AS7341 Color Detector ===\n");
    printf("Red=00  Green=01  Blue=10\n");
    printf("LED on GPIO%d\n\n", LED_PIN);

    i2c_init(I2C_PORT, I2C_FREQ);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    if (!as7341_init()) {
        printf("Init failed. Halting.\n");
        while (1) tight_loop_contents();
    }

    as7341_data_t data;
    int n = 0;

    while (1) {
        n++;
        if (!as7341_read(&data)) {
            printf("[%d] read error\n", n);
            sleep_ms(500);
            continue;
        }

        uint32_t r, g, b, rpct, gpct, bpct;
        detected_color_t color = detect_color(&data, &r, &g, &b, &rpct, &gpct, &bpct);

        printf("[%d] R:%6u G:%6u B:%6u | Rpct:%3u Gpct:%3u Bpct:%3u | %-7s -> %s\n",
            n, r, g, b, rpct, gpct, bpct, color_name(color), color_binary(color)
        );

        sleep_ms(100); // Time between measurements
    }

    return 0;
}
