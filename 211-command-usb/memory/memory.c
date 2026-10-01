#include "memory.h"
#include "command.h"
#include "device.h"
#include "led.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "hardware/gpio.h"
#include "hardware/regs/addressmap.h"
#include "hardware/regs/sio.h"

extern char __flash_binary_start;
extern char __flash_binary_end;
extern char __boot2_start__;
extern char __boot2_end__;
extern char __etext;
extern char __data_start__;
extern char __data_end__;
extern char __bss_start__;
extern char __bss_end__;
extern char __HeapLimit;
extern char __StackBottom;
extern char __StackTop;

int main(void);
uint32_t data_variable = 100;
uint32_t bss_variable;

static void row(const char *name, uintptr_t start, uintptr_t end)
{
    printf("%-10s 0x%08x 0x%08x %8u\n", name,
           (unsigned)start, (unsigned)end, (unsigned)(end - start));
}

void mem_info(void)
{
    const uintptr_t flash_end = XIP_BASE + PICO_FLASH_SIZE_BYTES;
    // RP2040 Datasheet: 264 KiB SRAM, 16 KiB boot ROM.
    const uint32_t sram_size = 264 * 1024;
    const uint32_t rom_size = 16 * 1024;
    const uint32_t data_size = (uintptr_t)&__data_end__ - (uintptr_t)&__data_start__;
    const uint32_t bss_size = (uintptr_t)&__bss_end__ - (uintptr_t)&__bss_start__;
    const uint32_t boot_size = (uintptr_t)&__boot2_end__ - (uintptr_t)&__boot2_start__;
    const uint32_t text_size = (uintptr_t)&__etext - (uintptr_t)&__boot2_end__;
    const uint32_t image_size = (uintptr_t)&__flash_binary_end - (uintptr_t)&__flash_binary_start;
    const uint32_t heap_size = (uintptr_t)&__HeapLimit - (uintptr_t)&__bss_end__;
    const uint32_t stack_size = (uintptr_t)&__StackTop - (uintptr_t)&__StackBottom;

    printf("area       start      end        size\n");
    row("flash", XIP_BASE, flash_end);
    row("sram", SRAM_BASE, SRAM_BASE + sram_size);
    row("rom", ROM_BASE, ROM_BASE + rom_size);
    row("image", (uintptr_t)&__flash_binary_start, (uintptr_t)&__flash_binary_end);
    row("free", (uintptr_t)&__flash_binary_end, flash_end);
    row("boot2", (uintptr_t)&__boot2_start__, (uintptr_t)&__boot2_end__);
    row("text", (uintptr_t)&__boot2_end__, (uintptr_t)&__etext);
    row("data flash", (uintptr_t)&__etext, (uintptr_t)&__etext + data_size);
    row("data ram", (uintptr_t)&__data_start__, (uintptr_t)&__data_end__);
    row("bss", (uintptr_t)&__bss_start__, (uintptr_t)&__bss_end__);
    row("heap", (uintptr_t)&__bss_end__, (uintptr_t)&__HeapLimit);
    row("stack", (uintptr_t)&__StackBottom, (uintptr_t)&__StackTop);

    printf("total\n");
    printf("  flash image %u = boot2 %u + text %u + data %u\n",
           (unsigned)image_size, (unsigned)boot_size, (unsigned)text_size, (unsigned)data_size);
    printf("  flash free %u of %u\n", (unsigned)(flash_end - (uintptr_t)&__flash_binary_end),
           (unsigned)PICO_FLASH_SIZE_BYTES);
    printf("  ram used %u = data %u + bss %u\n",
           (unsigned)(data_size + bss_size), (unsigned)data_size, (unsigned)bss_size);
    printf("  ram free %u for heap and %u for stack\n", (unsigned)heap_size, (unsigned)stack_size);
}

void fw_info(void)
{
    data_variable++;
    bss_variable++;
    const uint16_t *main_code = (const uint16_t *)((uintptr_t)main & ~1u);
    const uint16_t *fw_code = (const uint16_t *)((uintptr_t)fw_info & ~1u);
    uint32_t stack_variable = 1946;
    uint32_t *heap_variable = malloc(sizeof(uint32_t));
    if (heap_variable != NULL)
    {
        *heap_variable = 1951;
    }

    printf("object          address     value\n");
    printf("main            0x%08x 0x%04x\n", (unsigned)(uintptr_t)main, (unsigned)*main_code);
    printf("fw_info         0x%08x 0x%04x\n", (unsigned)(uintptr_t)fw_info, (unsigned)*fw_code);
    printf("commands        0x%08x\n", (unsigned)(uintptr_t)commands);
    for (uint i = 0; i < command_count; i++)
    {
        printf("- %-13s 0x%08x\n", commands[i].name, (unsigned)(uintptr_t)commands[i].handler);
    }
    printf("DEVICE_NAME     0x%08x %s\n", (unsigned)(uintptr_t)DEVICE_NAME, DEVICE_NAME);
    printf("FIRMWARE_VERSION 0x%08x %s\n", (unsigned)(uintptr_t)FIRMWARE_VERSION, FIRMWARE_VERSION);
    printf("DEVICE_PROJECT  0x%08x %s\n", (unsigned)(uintptr_t)DEVICE_PROJECT, DEVICE_PROJECT);
    printf("DEVICE_REPO     0x%08x %s\n", (unsigned)(uintptr_t)DEVICE_REPO, DEVICE_REPO);
    printf("DEVICE_BOARD    0x%08x %s\n", (unsigned)(uintptr_t)DEVICE_BOARD, DEVICE_BOARD);
    printf("data_variable   0x%08x %u\n", (unsigned)(uintptr_t)&data_variable, (unsigned)data_variable);
    printf("bss_variable    0x%08x %u\n", (unsigned)(uintptr_t)&bss_variable, (unsigned)bss_variable);
    printf("stack_variable  0x%08x %u\n", (unsigned)(uintptr_t)&stack_variable, (unsigned)stack_variable);
    if (heap_variable != NULL)
    {
        printf("heap_variable   0x%08x %u\n", (unsigned)(uintptr_t)heap_variable, (unsigned)*heap_variable);
    }
    else
    {
        printf("heap allocation failed\n");
    }
    free(heap_variable);
}

#define VECTOR_TABLE 0x10000100

void boot_info(void)
{
    const uint32_t *vectors = (const uint32_t *)VECTOR_TABLE;
    uint32_t stack_top = vectors[0];
    uint32_t reset_handler = vectors[1];
    volatile uint32_t *gpio_in = (volatile uint32_t *)(SIO_BASE + SIO_GPIO_IN_OFFSET);
    uint32_t level = (*gpio_in >> led_pin()) & 1u;

    printf("vector table   0x%08x\n", (unsigned)VECTOR_TABLE);
    printf("  stack top    0x%08x\n", (unsigned)stack_top);
    printf("  reset        0x%08x\n", (unsigned)reset_handler);
    printf("  reset (even) 0x%08x\n", (unsigned)(reset_handler & ~1u));
    printf("gpio in        0x%08x\n", (unsigned)(uintptr_t)gpio_in);
    printf("  led bit      %u\n", (unsigned)level);
    printf("  gpio_get     %u\n", (unsigned)gpio_get(led_pin()));
}
