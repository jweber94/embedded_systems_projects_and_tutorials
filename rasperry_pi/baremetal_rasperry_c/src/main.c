#include "BCM2837.h"

typedef unsigned int uint;

uint read_32(void* reg)
{
    /*
    *   Reads the current value from an 32 bit adress
    */
    uint* source_u = (uint*)reg;
    return *source_u;
}

void write_32(void* reg, uint value)
{
    /*
    *   Write to an 32 bit address a specific value
    */
    
    // We can hand over an arbitrary value for the register which is casted to a 32 bit unsigned int, which defines the adresses
    uint* dest_u = (uint*)reg;
    *dest_u = value;
    return;
}

int main(void)
{
    /*
    *   Setting up the 
    */  
    uint current_state = read_32(BCM2837_GPFSEL1);
    // update the current state
    current_state |= (1 << 3); // setting GPIO 21 with the bit pattern 001 in LSB fashion to make it an output pin, see datasheet for details. This value is currently only in RAM - We need to write it back to the register
    write_32(BCM2837_GPFSEL1, current_state);

    /*
    *   Blink the LED by using the BCM2837_GPSET0 and BCM2837_GPCLR0 register to make the pin high and low
    */
    while (1)
    {
        write_32(BCM2837_GPSET0, (1 << 21));

        // pause execution - this is needed since we do not have a sleep function in bare metal by default, since we have no OS
        for (int i = 0; i < 0x800000; i++)
        {
            continue;
        }

        write_32(BCM2837_GPCLR0, (1 << 21));
    
        // pause execution - this is needed since we do not have a sleep function in bare metal by default, since we have no OS
        for (int i = 0; i < 0x800000; i++)
        {
            continue;
        }

    }
    
    return 0;
}