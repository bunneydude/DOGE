class platform:
    def __init__(self, name, define):
        self.name = name
        self.define = define
        self.radios = []
    def add_radio(self, radio):
        self.radios.append(radio)
    def get_radio(self, radio_id):
        radio = next(radio for radio in self.radios if radio.id == radio_id)
        return radio

class radio:
    def __init__(self, comment, id, sending, get_data, send_data, get_rssi):
        self.comment = radio_comment
        self.id = radio_id
        self.sending = radio_sending
        self.get_data = radio_get_data
        self.send_data = radio_send_data
        self.get_rssi = radio_get_rssi
    def __repr__(self):
        return "{} {} {} {}".format(self.id, self.sending, self.get_data, self.send_data)

MAX_RADIOS = 3

#initialize platforms/radios
platforms = []
platforms.append(platform("LPC812", "__LPC8XX__"))
platforms.append(platform("MSP430", "MSP430"))

for platform in platforms:
    #433MHz Radio
    if platform.name == "LPC812" or platform.name == "MSP430":
        radio_comment    = "/**@brief 433MHz Radio ID */"
        radio_id         = "RADIO_ID_433"
        radio_sending    = "empty_sending"
        radio_get_data   = "empty_get_data"
        radio_send_data  = "empty_send_data"
        radio_get_rssi   = "empty_get_rssi"
        platform.add_radio(radio(radio_comment, radio_id, radio_sending, radio_get_data, radio_send_data, radio_get_rssi))
    
    #915MHz Radio
    if platform.name == "LPC812":
        radio_comment    = "/**@brief 915MHz Radio ID */"
        radio_id         = "RADIO_ID_915"
        radio_sending    = "empty_sending"
        radio_get_data   = "empty_get_data"
        radio_send_data  = "empty_send_data"
        radio_get_rssi   = "empty_get_rssi"
        platform.add_radio(radio(radio_comment, radio_id, radio_sending, radio_get_data, radio_send_data, radio_get_rssi))
    elif platform.name == "MSP430":
        radio_comment    = "/**@brief 915MHz Radio ID */"
        radio_id         = "RADIO_ID_915"
        radio_sending    = "Radio.busy"
        radio_get_data   = "Radio.receiverOn"
        radio_send_data  = "Radio.transmit"
        radio_get_rssi   = "Radio.getRssi"
        platform.add_radio(radio(radio_comment, radio_id, radio_sending, radio_get_data, radio_send_data, radio_get_rssi))
 
    #2.4GHz Radio
    if platform.name == "LPC812":
        radio_comment   = "/**@brief 2400MHz Radio ID */"
        radio_id        = "RADIO_ID_2400"
        radio_sending   = "nrf24_isSending"
        radio_get_data  = "nrf24_timeoutRead"
        radio_send_data = "nrf24_send"
        radio_get_rssi  = "empty_get_rssi"
        platform.add_radio(radio(radio_comment, radio_id, radio_sending, radio_get_data, radio_send_data, radio_get_rssi))
    elif platform.name == "MSP430":
        radio_comment    = "/**@brief 2.4GHz Radio ID */"
        radio_id        = "RADIO_ID_2400"
        radio_sending   = "nrf24_isSending"
        radio_get_data  = "nrf24_timeoutRead"
        radio_send_data = "nrf24_send"
        radio_get_rssi  = "empty_get_rssi"
        platform.add_radio(radio(radio_comment, radio_id, radio_sending, radio_get_data, radio_send_data, radio_get_rssi))
    
    #All Radios
    if platform.name == "LPC812" or platform.name == "MSP430":
        radio_comment   = "/**@brief Radio ID for all frequency bands */"
        radio_id        = "RADIO_ID_ALL"
        radio_sending   = "empty_sending"
        radio_get_data  = "empty_get_data"
        radio_send_data = "empty_send_data"
        radio_get_rssi  = "empty_get_rssi"
        platform.add_radio(radio(radio_comment, radio_id, radio_sending, radio_get_data, radio_send_data, radio_get_rssi))

header = open("doge_radio.h", "w")
source = open("doge_radio.c", "w")
condition = "#if "
for i in range(2**MAX_RADIOS):
    num_radios = 0
    radio_2400_en = False
    radio_915_en = False
    radio_433_en = False
    radio_defines = []
    next_radio_idx = 0
  
    if (i & 4):
        radio_2400_en = True
        num_radios += 1
        condition += "RADIO_2400_EN && "
    else:
        condition += "!RADIO_2400_EN && "
    
    if (i & 2):
        radio_915_en = True
        num_radios += 1
        condition += "RADIO_915_EN && "
    else:
        condition += "!RADIO_915_EN && "
    
    if (i & 1):
        radio_433_en = True
        num_radios += 1
        condition += "RADIO_433_EN"
    else:
        condition += "!RADIO_433_EN"
  
    header.write(condition + "\n")
    source.write(condition + "\n")
    if radio_433_en:
        radio_defines.insert(next_radio_idx, "RADIO_ID_433")
        next_radio_idx += 1
    if radio_915_en:
        radio_defines.insert(next_radio_idx, "RADIO_ID_915")
        next_radio_idx += 1
    if radio_2400_en:
        radio_defines.insert(next_radio_idx, "RADIO_ID_2400")
        next_radio_idx += 1
    
    radio_defines.insert(next_radio_idx, "RADIO_ID_ALL")
    
    if num_radios > 1:
        num_radios += 1 #add RADIO_ID_ALL
        header.write("#define NUM_RADIOS {}\n".format(num_radios))
        for i in range(len(radio_defines)):
            header.write("#define {} {}\n".format(radio_defines[i], i))
        ifdef = "#ifdef {}\n"
        for platform in platforms:
            source.write(ifdef.format(platform.define))
            source.write("struct radioMethods dogeRadios[NUM_RADIOS] = {\n")
            for radio in platform.radios:
                source.write("   {\n")
                source.write("      {}, {}, {}, {}\n".format(radio.sending, radio.get_data, radio.send_data, radio.get_rssi))
                if (radio.id != "RADIO_ID_ALL"):
                    source.write("   },\n")
                else:
                    source.write("   }\n")
            source.write("};\n")
            ifdef = "#elif defined({})\n"
        source.write("#endif\n")
    elif num_radios == 1:
        header.write("#define NUM_RADIOS {}\n".format(num_radios))
        radio = platform.get_radio(radio_defines[0])
        header.write("#define {} {}\n".format(radio.id, 0))
        radio = platform.get_radio(radio_defines[1])
        header.write("#define {} {}\n".format(radio.id, 0))
        ifdef = "#ifdef {}\n"
        for platform in platforms:
            source.write(ifdef.format(platform.define))
            radio = platform.get_radio(radio_defines[0])
            source.write("struct radioMethods dogeRadios[NUM_RADIOS] = {\n")
            source.write("   {\n")
            source.write("      {}, {}, {}, {}\n".format(radio.sending, radio.get_data, radio.send_data, radio.get_rssi))
            source.write("   }\n")
            source.write("};\n")
            ifdef = "#elif defined({})\n"
        source.write("#endif\n")
    elif num_radios == 0:
        source.write("#error \"No radios are enabled\"\n")
        header.write("#error \"No radios are enabled\"\n")
    condition = "#elif "
source.write("#endif\n")
header.write("#endif\n")
source.close()
header.close()
print("header file written to doge_radio.h")
print("source file written to doge_radio.c")
