#pragma once

#define SPI3_CR1     ((volatile uint16_t*)0x40003C00)
#define SPI3_SR      ((volatile uint16_t*)0x40003C08)
#define SPI3_DR      ((volatile uint16_t*)0x40003C0C)
#define SPI_TXE      0x0002
#define SPI_RXNE     0x0001
#define SPI_BSY      0x0080

#define RCC_APB1ENR  ((volatile uint32_t*)0x4002101C)
#define GPIOB_CRL    ((volatile uint32_t*)0x40010C00)
#define GPIOB_BSRR   ((volatile uint32_t*)0x40010C10)
#define GPIOB_BRR    ((volatile uint32_t*)0x40010C14)
#define GPIOD_CRH    ((volatile uint32_t*)0x40011404)
#define GPIOD_BSRR   ((volatile uint32_t*)0x40011410)
#define GPIOE_CRL    ((volatile uint32_t*)0x40011800)
#define GPIOE_CRH    ((volatile uint32_t*)0x40011804)
#define GPIOE_ODR    ((volatile uint32_t*)0x4001180C)

#define FPGA_CDONE   ((volatile uint32_t*)0x4222010c)
#define FPGA_nRST    ((volatile uint32_t*)0x4221819c)
#define FPGA_SS      ((volatile uint32_t*)0x42218198)

class CFpga
{
    uint8_t SpiTransfer(uint8_t data)
    {
        while (*SPI3_SR & SPI_RXNE) (void)*SPI3_DR;
        *SPI3_DR = data;
        while (!(*SPI3_SR & SPI_TXE));
        while (!(*SPI3_SR & SPI_RXNE));
        while (*SPI3_SR & SPI_BSY);
        return (uint8_t)*SPI3_DR;
    }

    void SpiCs(bool active)
    {
        uint32_t crl = *GPIOB_CRL;
        crl &= ~(0xFu << 28);
        crl |= (0x2u << 28);
        *GPIOB_CRL = crl;
        if (active) *GPIOB_BRR = (1 << 7);
        else        *GPIOB_BSRR = (1 << 7);
    }

public:
    bool Flash(CBufferedReader& fw)
//    bool Flash(char* name)
    {
//        CBufferedReader fw;
//        if ( !fw.Open(name) )
//            return false;

        int count = 0xb720;
        volatile uint16_t* ptrSpi = (uint16_t*)0x40003C0C;
        volatile uint32_t* bit_SPI3_txe = (volatile uint32_t *)0x42078104;
        volatile uint32_t* cdone = (volatile uint32_t*)0x4222010c;
        volatile int counter;

        // reset
        *((volatile uint32_t*)0x4221819c) = 0;
        *((volatile uint32_t*)0x42218198) = 0;
        counter = 7;
        while (counter--);

        *((volatile uint32_t*)0x42218198) = 1;
        counter = 0x1baf;
        while (counter--);

        CStream spiStream((uint8_t*)ptrSpi, 1);
        while (count--)
        {
            fw >> spiStream;
            while (bit_SPI3_txe == 0);
        }

        *((volatile uint32_t*)0x4221819c) = 1;

//        fw.Close();
        BIOS::SYS::DelayMs(10);
        return !!*cdone;
    }

    void SpiInit()
    {
        *RCC_APB1ENR |= (1u << 15);
        *SPI3_CR1 = 0;
        *SPI3_CR1 = (1u << 6) | (1u << 2) | (1u << 9) | (1u << 8) | (3u << 3);
        *GPIOB_BSRR = (1 << 7);
        while (*SPI3_SR & SPI_RXNE) (void)*SPI3_DR;

        // SPI sync
        SpiCs(true);
        for (int i = 0; i < 5; i++) SpiTransfer(0xFF);
        SpiCs(false);
    }

    bool IsConfigured()
    {
        return !!*FPGA_CDONE;
    }

    void WriteReg(uint8_t cmd, uint32_t value)
    {
        SpiCs(true);
        SpiTransfer(cmd);
        SpiTransfer((uint8_t)(value >> 24));
        SpiTransfer((uint8_t)(value >> 16));
        SpiTransfer((uint8_t)(value >> 8));
        SpiTransfer((uint8_t)(value));
        SpiCs(false);
    }

    uint8_t ReadVersion()
    {
        SpiCs(true);
        SpiTransfer(0x01);
        uint8_t ver = SpiTransfer(0x00);
        SpiTransfer(0x00);
        SpiTransfer(0x00);
        SpiTransfer(0x00);
        SpiCs(false);
        return ver;
    }

    void BusRelease()
    {
        *GPIOE_ODR = 0x0000;
        *GPIOE_CRL = 0x88888888;
        *GPIOE_CRH = 0x88888888;

        uint32_t crh = *GPIOD_CRH;
        crh &= ~(0xF << 4);
        crh |= (0x8 << 4);
        *GPIOD_CRH = crh;
        *GPIOD_BSRR = (1 << 9);
    }

    void BusReclaim()
    {
        *GPIOE_CRL = 0x33333333;
        *GPIOE_CRH = 0x33333333;

        uint32_t crh = *GPIOD_CRH;
        crh &= ~(0xF << 4);
        crh |= (0x3 << 4);
        *GPIOD_CRH = crh;
        *GPIOD_BSRR = (1 << 9);
    }
};
