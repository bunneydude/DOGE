#ifndef MSP430_GPIO_H
#define MSP430_GPIO_H

// if bit n is set in mask, then bit n of reg will not be changed
// P1DIR & mask   - this will clear the bits we might want to set
// value & ~mask  - this clears bits in value that are in the write mask range
#define RMW_PORT(reg, value, mask) (reg = (reg & mask) | (value & ~mask))

//Perform a masked read of reg into var
// if bit n is set in mask, then bit n of the result will be 0
#define R_PORT(reg, var, mask) (var = (reg & ~mask))

#endif
