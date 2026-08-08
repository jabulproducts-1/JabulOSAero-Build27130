#include "jabulos.h"

typedef struct {
    u8 seconds;
    u8 minutes;
    u8 hours;
} rtc_time_t;

static u8 cmos_read(u8 reg) {
    outb(0x70, reg);
    io_wait();
    return inb(0x71);
}

static bool rtc_updating(void) {
    return (cmos_read(0x0A) & 0x80) != 0;
}

static u8 bcd_to_binary(u8 value) {
    return (u8)((value & 0x0F) + ((value / 16) * 10));
}

static void rtc_read_time(rtc_time_t* out_time) {
    u8 seconds;
    u8 minutes;
    u8 hours;
    u8 status_b;

    if (out_time == NULL) {
        return;
    }

    while (rtc_updating()) {
    }

    seconds = cmos_read(0x00);
    minutes = cmos_read(0x02);
    hours = cmos_read(0x04);
    status_b = cmos_read(0x0B);

    if ((status_b & 0x04) == 0) {
        seconds = bcd_to_binary(seconds);
        minutes = bcd_to_binary(minutes);
        hours = bcd_to_binary((u8)(hours & 0x7F));
    }

    if ((status_b & 0x02) == 0 && (hours & 0x80) != 0) {
        hours = (u8)(((hours & 0x7F) + 12) % 24);
    }

    out_time->seconds = seconds;
    out_time->minutes = minutes;
    out_time->hours = hours;
}

void rtc_read_time_string(char* out_buffer) {
    rtc_time_t time;

    rtc_read_time(&time);

    out_buffer[0] = (char)('0' + (time.hours / 10));
    out_buffer[1] = (char)('0' + (time.hours % 10));
    out_buffer[2] = ':';
    out_buffer[3] = (char)('0' + (time.minutes / 10));
    out_buffer[4] = (char)('0' + (time.minutes % 10));
    out_buffer[5] = ':';
    out_buffer[6] = (char)('0' + (time.seconds / 10));
    out_buffer[7] = (char)('0' + (time.seconds % 10));
    out_buffer[8] = '\0';
}

u32 rtc_read_seconds_of_day(void) {
    rtc_time_t time;

    rtc_read_time(&time);
    return (u32)time.hours * 3600u + (u32)time.minutes * 60u + (u32)time.seconds;
}
