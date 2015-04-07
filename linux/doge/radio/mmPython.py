import json

MM_DEVICE_SIZE = 4
MM_NETWORK_SIZE = 0x20

MM_GPIO_SIZE = 0x10
MM_ADC_SIZE = 0x10
MM_UART_SIZE = 0x4
MM_DSP_SIZE = 0x8

MM_PHYSICAL_BAR = 0x0
MM_DEVICE_BASE  = MM_PHYSICAL_BAR
MM_NETWORK_BASE = MM_DEVICE_BASE + MM_DEVICE_SIZE
MM_PHYSICAL_MAX = MM_NETWORK_BASE + MM_NETWORK_SIZE

MM_FUNCTION_BAR = MM_PHYSICAL_MAX
MM_GPIO_BASE    = MM_FUNCTION_BAR
MM_ADC_BASE     = MM_GPIO_BASE + MM_GPIO_SIZE
MM_UART_BASE    = MM_ADC_BASE + MM_ADC_SIZE
MM_DSP_BASE     = MM_UART_BASE + MM_UART_SIZE
MM_FUNCTION_MAX = MM_DSP_BASE + MM_DSP_SIZE

memoryMap = {
   "mm_device_size":MM_DEVICE_SIZE,
   "mm_network_size":MM_NETWORK_SIZE,
   "mm_gpio_size":MM_GPIO_SIZE,
   "mm_adc_size":MM_ADC_SIZE,
   "mm_uart_size":MM_UART_SIZE,
   "mm_dsp_size":MM_DSP_SIZE,

   "mm_physical_bar":MM_PHYSICAL_BAR,
   "mm_device_base":MM_DEVICE_BASE,
   "mm_network_base":MM_NETWORK_BASE,
   "mm_physical_max":MM_PHYSICAL_MAX,
   "mm_function_bar":MM_FUNCTION_BAR,
   "mm_gpio_base":MM_GPIO_BASE,
   "mm_adc_base":MM_ADC_BASE,
   "mm_uart_base":MM_UART_BASE,
   "mm_dsp_base":MM_DSP_BASE,
   "mm_function_max":MM_FUNCTION_MAX
}

with open("mm_msp430_v1.txt", 'w') as file:
   json.dump(memoryMap, file)



#newFile = open("mm_json.txt", 'r')
#newData =  json.load(newFile)
#print(json.dumps(newData))


