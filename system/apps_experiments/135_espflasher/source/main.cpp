#include <library.h>
#include "glue.h"
#include "gpio.h"

uint32_t* mGpioStatus = nullptr;
int32_t s_time_end = 0;
enum {
  SERIAL_FLASHER_BOOT_HOLD_TIME_MS=50,
  SERIAL_FLASHER_RESET_HOLD_TIME_MS=100
};

bool reading = false;

void check()
{
    if (*mGpioStatus != 0)
    {
        BIOS::DBG::Print("<<gpio state: %x>>\n", *mGpioStatus);
        *mGpioStatus = 0;
    }
}

void loader_port_delay_ms(uint32_t ms)
{
    BIOS::SYS::DelayMs(ms);
}

static esp_loader_error_t read_char(char *c, uint32_t timeout)
{
    if (!reading) {reading = true; BIOS::DBG::Print(".\nrd: ");}
    check();
    int32_t t0 = BIOS::SYS::GetTick();
    while (BIOS::SYS::GetTick()-t0 < timeout)
    {
        if (BIOS::GPIO::UART::Available())
        {
            *c = BIOS::GPIO::UART::Read();
            BIOS::DBG::Print("%02x ", *c);
            return ESP_LOADER_SUCCESS;
        }
    }
    check();
    return ESP_LOADER_ERROR_TIMEOUT;
}

static esp_loader_error_t read_data(uint8_t *buffer, uint32_t size)
{
    if (!reading) {reading = true; BIOS::DBG::Print(".\nrd: ");}
    check();
    int32_t t0 = BIOS::SYS::GetTick();
    int32_t timeout = 50;
    while (BIOS::SYS::GetTick()-t0 < timeout)
    {
        if (BIOS::GPIO::UART::Available())
        {
            uint8_t d = BIOS::GPIO::UART::Read();
            *buffer++ = d; 
            BIOS::DBG::Print("%02x ", d);

            if (--size == 0)
                return ESP_LOADER_SUCCESS;
        }
    }
    check();
    return ESP_LOADER_ERROR_TIMEOUT;
}

esp_loader_error_t loader_port_write(const uint8_t *data, uint16_t size, uint32_t timeout)
{
    if (reading) {reading = false; BIOS::DBG::Print(".\nwr: ");}
    check();
    while (size--)
    {
      BIOS::DBG::Print("%02x ", *data);
      BIOS::GPIO::UART::Write(*data++);
    }
    check();
    return ESP_LOADER_SUCCESS;
}

esp_loader_error_t loader_port_read(uint8_t *data, uint16_t size, uint32_t timeout)
{
    RETURN_ON_ERROR( read_data(data, size) );
    return ESP_LOADER_SUCCESS;
}


// Set GPIO0 LOW, then assert reset pin for 50 milliseconds.
void loader_port_enter_bootloader(void)
{
    Gpio::SetState(Gpio::BASEB, Gpio::P3, Gpio::StateOutput10Mhz | Gpio::StateOutputPushPull);
    Gpio::SetLevel(Gpio::BASEB, Gpio::P3, 0);
    BIOS::DBG::Print("enter bootloader active\n");

    loader_port_reset_target();
    loader_port_delay_ms(SERIAL_FLASHER_BOOT_HOLD_TIME_MS);
//    Gpio::SetState(Gpio::BASEB, Gpio::P3, Gpio::StateInputPull | Gpio::StateInputFloating);
    Gpio::SetLevel(Gpio::BASEB, Gpio::P3, 1);
    BIOS::DBG::Print("enter bootloader release\n");

    BIOS::SYS::DelayMs(500);
    while (BIOS::GPIO::UART::Available())
        BIOS::GPIO::UART::Read();

    check();
    BIOS::DBG::Print("enter bootloader done\n");
}


void loader_port_reset_target(void)
{
    Gpio::SetState(Gpio::BASEB, Gpio::P4, Gpio::StateOutput10Mhz | Gpio::StateOutputPushPull);
    Gpio::SetLevel(Gpio::BASEB, Gpio::P4, 0);
    BIOS::DBG::Print("enter reset active\n");
    loader_port_delay_ms(SERIAL_FLASHER_RESET_HOLD_TIME_MS);
//    Gpio::SetState(Gpio::BASEB, Gpio::P4, Gpio::StateInputPull | Gpio::StateInputFloating);
    Gpio::SetLevel(Gpio::BASEB, Gpio::P4, 1);
    BIOS::DBG::Print("enter reset release\n");

    BIOS::SYS::DelayMs(500);
    while (BIOS::GPIO::UART::Available())
        BIOS::GPIO::UART::Read();

    check();
    BIOS::DBG::Print("enter reset done\n");
}

void loader_port_start_timer(uint32_t ms)
{
    s_time_end = BIOS::SYS::GetTick() + ms;
}


uint32_t loader_port_remaining_time(void)
{
    int32_t remaining = s_time_end - BIOS::SYS::GetTick(); // / 1000;
    return (remaining > 0) ? (uint32_t)remaining : 0;
}


void loader_port_debug_print(const char *str)
{
    BIOS::DBG::Print("DEBUG: '%s'\n", str);
}

esp_loader_error_t loader_port_change_transmission_rate(uint32_t baudrate)
{
    BIOS::DBG::Print("Set speed %d\n", baudrate);
    BIOS::GPIO::UART::Setup(baudrate, (BIOS::GPIO::UART::EConfig)0);
    Gpio::SetState(Gpio::BASEB, Gpio::P3, Gpio::StateInputPull | Gpio::StateInputFloating);
    Gpio::SetLevel(Gpio::BASEB, Gpio::P3, 1);
    Gpio::SetState(Gpio::BASEB, Gpio::P4, Gpio::StateInputPull | Gpio::StateInputFloating);
    Gpio::SetLevel(Gpio::BASEB, Gpio::P4, 1);
    return ESP_LOADER_SUCCESS;
}

#define BINARY_PATH       "../../binaries/ESP32_AT_Firmware/Firmware.bin"
static void upload_file(const char *path, size_t address)
{
/*
    char *buffer = NULL;

    FILE *image = fopen(path, "r");
    if (image == NULL) {
        printf("Error:Failed to open file %s\n", path);
        return;
    }

    fseek(image, 0L, SEEK_END);
    size_t size = ftell(image);
    rewind(image);

    printf("File %s opened. Size: %u bytes\n", path, size);

    buffer = (char *)malloc(size);
    if (buffer == NULL) {
        printf("Error: Failed allocate memory\n");
        goto cleanup;
    }

    // copy file content to buffer
    size_t bytes_read = fread(buffer, 1, size, image);
    if (bytes_read != size) {
        printf("Error occurred while reading file");
        goto cleanup;
    }

    flash_binary(buffer, size, address);

cleanup:
    fclose(image);
    free(buffer);
*/
}
#define DEFAULT_BAUD_RATE 115200
#define HIGHER_BAUD_RATE  230400

#ifdef _ARM
__attribute__((__section__(".entry")))
#endif
int _main(void)
{
    BIOS::LCD::Bar(CRect(0, 0, BIOS::LCD::Width, BIOS::LCD::Height), 0);
    mGpioStatus = (uint32_t*)BIOS::SYS::GetAttribute(BIOS::SYS::EAttribute::GpioStatus);

    BIOS::GPIO::PinMode(BIOS::GPIO::P1, BIOS::GPIO::Uart);
    BIOS::GPIO::PinMode(BIOS::GPIO::P2, BIOS::GPIO::Uart);

    BIOS::DBG::Print("Connecting...\n");
    loader_port_change_transmission_rate(115200);
    if (connect_to_target(HIGHER_BAUD_RATE) == ESP_LOADER_SUCCESS) {
        BIOS::DBG::Print("Connected %d!\n", esp_loader_get_target());
//        upload_file(BINARY_PATH, 0);
    }
    while (!BIOS::KEY::GetKey());
    loader_port_reset_target();
    BIOS::GPIO::PinMode(BIOS::GPIO::P1, BIOS::GPIO::Input);
    BIOS::GPIO::PinMode(BIOS::GPIO::P2, BIOS::GPIO::Input);
    return 0;
}

void _HandleAssertion(const char* file, int line, const char* cond)
{
    BIOS::DBG::Print("Assertion failed in %s [%d]: %s\n", file, line, cond);
    while (1);
}
