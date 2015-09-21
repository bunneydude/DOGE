#include "doge_radio.h"

#ifdef MSP430
#include <Energia.h>
#include <AIR430BoostFCC.h>
#endif

#include "../radios/radios.h"

#if !RADIO_2400_EN && !RADIO_915_EN && !RADIO_433_EN
#error "No radios are enabled"
#elif !RADIO_2400_EN && !RADIO_915_EN && RADIO_433_EN
#ifdef __LPC8XX__
struct radioMethods dogeRadios[NUM_RADIOS] = {
   {
      empty_sending, empty_get_data, empty_send_data, empty_get_rssi
   }
};
#elif defined(MSP430)
struct radioMethods dogeRadios[NUM_RADIOS] = {
   {
      empty_sending, empty_get_data, empty_send_data, empty_get_rssi
   }
};
#endif
#elif !RADIO_2400_EN && RADIO_915_EN && !RADIO_433_EN
#ifdef __LPC8XX__
struct radioMethods dogeRadios[NUM_RADIOS] = {
   {
      empty_sending, empty_get_data, empty_send_data, empty_get_rssi
   }
};
#elif defined(MSP430)
struct radioMethods dogeRadios[NUM_RADIOS] = {
   {
      Radio.busy, Radio.receiverOn, Radio.transmit, Radio.getRssi
   }
};
#endif
#elif !RADIO_2400_EN && RADIO_915_EN && RADIO_433_EN
#ifdef __LPC8XX__
struct radioMethods dogeRadios[NUM_RADIOS] = {
   {
      empty_sending, empty_get_data, empty_send_data, empty_get_rssi
   },
   {
      empty_sending, empty_get_data, empty_send_data, empty_get_rssi
   },
   {
      empty_sending, empty_get_data, empty_send_data, empty_get_rssi
   }
};
#elif defined(MSP430)
struct radioMethods dogeRadios[NUM_RADIOS] = {
   {
      empty_sending, empty_get_data, empty_send_data, empty_get_rssi
   },
   {
      Radio.busy, Radio.receiverOn, Radio.transmit, Radio.getRssi
   },
   {
      empty_sending, empty_get_data, empty_send_data, empty_get_rssi
   }
};
#endif
#elif RADIO_2400_EN && !RADIO_915_EN && !RADIO_433_EN
#ifdef __LPC8XX__
struct radioMethods dogeRadios[NUM_RADIOS] = {
   {
      nrf24_isSending, nrf24_timeoutRead, nrf24_send, empty_get_rssi
   }
};
#elif defined(MSP430)
struct radioMethods dogeRadios[NUM_RADIOS] = {
   {
      nrf24_isSending, nrf24_timeoutRead, nrf24_send, empty_get_rssi
   }
};
#endif
#elif RADIO_2400_EN && !RADIO_915_EN && RADIO_433_EN
#ifdef __LPC8XX__
struct radioMethods dogeRadios[NUM_RADIOS] = {
   {
      empty_sending, empty_get_data, empty_send_data, empty_get_rssi
   },
   {
      nrf24_isSending, nrf24_timeoutRead, nrf24_send, empty_get_rssi
   },
   {
      empty_sending, empty_get_data, empty_send_data, empty_get_rssi
   }
};
#elif defined(MSP430)
struct radioMethods dogeRadios[NUM_RADIOS] = {
   {
      empty_sending, empty_get_data, empty_send_data, empty_get_rssi
   },
   {
      nrf24_isSending, nrf24_timeoutRead, nrf24_send, empty_get_rssi
   },
   {
      empty_sending, empty_get_data, empty_send_data, empty_get_rssi
   }
};
#endif
#elif RADIO_2400_EN && RADIO_915_EN && !RADIO_433_EN
#ifdef __LPC8XX__
struct radioMethods dogeRadios[NUM_RADIOS] = {
   {
      empty_sending, empty_get_data, empty_send_data, empty_get_rssi
   },
   {
      nrf24_isSending, nrf24_timeoutRead, nrf24_send, empty_get_rssi
   },
   {
      empty_sending, empty_get_data, empty_send_data, empty_get_rssi
   }
};
#elif defined(MSP430)
struct radioMethods dogeRadios[NUM_RADIOS] = {
   {
      Radio.busy, Radio.receiverOn, Radio.transmit, Radio.getRssi
   },
   {
      nrf24_isSending, nrf24_timeoutRead, nrf24_send, empty_get_rssi
   },
   {
      empty_sending, empty_get_data, empty_send_data, empty_get_rssi
   }
};
#endif
#elif RADIO_2400_EN && RADIO_915_EN && RADIO_433_EN
#ifdef __LPC8XX__
struct radioMethods dogeRadios[NUM_RADIOS] = {
   {
      empty_sending, empty_get_data, empty_send_data, empty_get_rssi
   },
   {
      empty_sending, empty_get_data, empty_send_data, empty_get_rssi
   },
   {
      nrf24_isSending, nrf24_timeoutRead, nrf24_send, empty_get_rssi
   },
   {
      empty_sending, empty_get_data, empty_send_data, empty_get_rssi
   }
};
#elif defined(MSP430)
struct radioMethods dogeRadios[NUM_RADIOS] = {
   {
      empty_sending, empty_get_data, empty_send_data, empty_get_rssi
   },
   {
      Radio.busy, Radio.receiverOn, Radio.transmit, Radio.getRssi
   },
   {
      nrf24_isSending, nrf24_timeoutRead, nrf24_send, empty_get_rssi
   },
   {
      empty_sending, empty_get_data, empty_send_data, empty_get_rssi
   }
};
#endif
#endif

#ifdef __LPC8XX__
int8_t empty_get_rssi()
{
   return 0;
}
#endif
