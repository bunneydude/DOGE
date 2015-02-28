method = ARGV[0].to_s

if(method.casecmp("defines").zero?)
	defines = true
elsif(method.casecmp("list").zero?)
	defines = false
else
	abort("Unrecognized input: #{method.inspect}. Must be one of 'defines' or 'list'.")
end

driverName = "NRF24L01P"
# name, address, mask, shift, fields
nrfConfig = [{:name => "config", :address=>0x0, :fields => [{:name=>"mask_rx_dr",  :bits=>[6,6]},
                                                            {:name=>"mask_tx_ds",  :bits=>[5,5]},
                                                            {:name=>"mask_max_rt", :bits=>[4,4]},
                                                            {:name=>"en_crc",      :bits=>[3,3]},
                                                            {:name=>"crco",        :bits=>[2,2]},
                                                            {:name=>"pwr_up",      :bits=>[1,1]},
                                                            {:name=>"prim_rx",     :bits=>[0,0]}
                                                            ]},
             {:name => "en_aa", :address=>0x1, :fields => [{:name=>"enaa_p5", :bits=>[5,5]},
                                                           {:name=>"enaa_p4", :bits=>[4,4]},
                                                           {:name=>"enaa_p3", :bits=>[3,3]},
                                                           {:name=>"enaa_p2", :bits=>[2,2]},
                                                           {:name=>"enaa_p1", :bits=>[1,1]},
                                                           {:name=>"enaa_p0", :bits=>[0,0]}
                                                           ]},
             {:name => "en_rxaddr", :address=>0x2, :fields => [{:name=>"erx_p5", :bits=>[5,5]},
                                                              {:name=>"erx_p4", :bits=>[4,4]},
                                                              {:name=>"erx_p3", :bits=>[3,3]},
                                                              {:name=>"erx_p2", :bits=>[2,2]},
                                                              {:name=>"erx_p1", :bits=>[1,1]},
                                                              {:name=>"erx_p0", :bits=>[0,0]}
                                                              ]},
             {:name => "setup_aw", :address=>0x3, :fields => [{:name=>"aw", :bits=>[1,0]},
                                                              ]},
             {:name => "setup_retr", :address=>0x4, :fields => [{:name=>"ard", :bits=>[7,4]},
                                                                {:name=>"arc", :bits=>[3,0]}
                                                                ]},
             {:name => "rf_ch", :address=>0x5, :fields => [{:name=>"rf_ch", :bits=>[6,0]},
                                                              ]},
             {:name => "rf_setup", :address=>0x6, :fields => [{:name=>"cont_wave", :bits=>[7,7]},
                                                              {:name=>"rf_dr_low", :bits=>[5,5]},
                                                              {:name=>"pll_lock", :bits=>[4,4]},
                                                              {:name=>"rf_dr_high", :bits=>[3,3]},
                                                              {:name=>"rf_pwr", :bits=>[2,1]},
                                                              ]},
             {:name => "status", :address=>0x7, :fields => [{:name=>"rx_dr", :bits=>[6,6]},
                                                              {:name=>"tx_ds", :bits=>[5,5]},
                                                              {:name=>"max_rt", :bits=>[4,4]},
                                                              {:name=>"rx_p_no", :bits=>[3,1]},
                                                              {:name=>"status_tx_full", :bits=>[0,0]},
                                                              ]},
             {:name => "observe_tx", :address=>0x8, :fields => [{:name=>"plos_cnt", :bits=>[7,4]},
                                                                {:name=>"arc_cnt", :bits=>[3,0]}
                                                                ]},
             {:name => "rpd", :address=>0x9, :fields => [{:name=>"rpd", :bits=>[0,0]},
                                                              ]},
             {:name => "rx_addr_p0", :address=>0xa, :fields => []},
             {:name => "rx_addr_p1", :address=>0xb, :fields => []},
             {:name => "rx_addr_p2", :address=>0xc, :fields => []},
             {:name => "rx_addr_p3", :address=>0xd, :fields => []},
             {:name => "rx_addr_p4", :address=>0xe, :fields => []},
             {:name => "rx_addr_p5", :address=>0xf, :fields => []},
             {:name => "tx_addr", :address=>0x10, :fields => []},
             {:name => "rx_pw_p0", :address=>0x11, :fields => [{:name=>"rx_pw_p0", :bits=>[5,0]}]},
             {:name => "rx_pw_p1", :address=>0x12, :fields => [{:name=>"rx_pw_p1", :bits=>[5,0]}]},
             {:name => "rx_pw_p2", :address=>0x13, :fields => [{:name=>"rx_pw_p2", :bits=>[5,0]}]},
             {:name => "rx_pw_p3", :address=>0x14, :fields => [{:name=>"rx_pw_p3", :bits=>[5,0]}]},
             {:name => "rx_pw_p4", :address=>0x15, :fields => [{:name=>"rx_pw_p4", :bits=>[5,0]}]},
             {:name => "rx_pw_p5", :address=>0x16, :fields => [{:name=>"rx_pw_p5", :bits=>[5,0]}]},
             {:name => "fifo_status", :address=>0x17, :fields => [{:name=>"tx_reuse", :bits=>[6,6]},
                                                                  {:name=>"fifo_tx_full", :bits=>[5,5]},
                                                                  {:name=>"tx_empty", :bits=>[4,4]},
                                                                  {:name=>"rx_full", :bits=>[1,1]},
                                                                  {:name=>"rx_empty", :bits=>[0,0]},
                                                                  ]},
             {:name => "dynpd", :address=>0x1c, :fields => [{:name=>"dpl_p5", :bits=>[5,5]},
                                                            {:name=>"dpl_p4", :bits=>[4,4]},
                                                            {:name=>"dpl_p3", :bits=>[3,3]},
                                                            {:name=>"dpl_p2", :bits=>[2,2]},
                                                            {:name=>"dpl_p1", :bits=>[1,1]},
                                                            {:name=>"dpl_p0", :bits=>[0,0]}
                                                            ]},
             {:name => "feature", :address=>0x1d, :fields => [{:name=>"en_dbl",     :bits=>[2,2]},
                                                              {:name=>"en_ack_pay", :bits=>[1,1]},
                                                              {:name=>"en_dyn_ack", :bits=>[0,0]},
                                                              ]}
             ]


nrfCommands = [{:name=>"r_register", :value=>0x0},
		{:name=>"w_register", :value => 0x20},
		{:name=>"register_mask", :value => 0x1f},
		{:name=>"r_rx_payload", :value => 0x61},
		{:name=>"w_tx_payload", :value => 0xa0},
		{:name=>"flush_tx", :value => 0xe1},
		{:name=>"flush_rx", :value => 0xe2},
		{:name=>"reuse_tx_pl", :value => 0xe3},
		{:name=>"activate", :value => 0x50},
		{:name=>"r_rx_pl_wid", :value => 0x60},
		{:name=>"nop", :value => 0xff}]


configStrings = {:regAddress => [], :fields=>[], :shiftList => [], :maskList=>[], :shiftDefine=>[], :maskDefine=>[], :commands=>[]}

nrfConfig.sort do |a, b|
   a[:address] <=> b[:address]
end

nrfCommands.sort do |a, b|
   a[:value] <=> b[:value]
end


configStrings[:regAddress] << 'enum nrf24l01p_registers{' 
configStrings[:fields] << "enum nrf24l01p_fields{"
configStrings[:shiftList] << "static uint8_t nrf24l01p_shiftList[] = {"
configStrings[:maskList] << "static uint8_t nrf24l01p_maskList[] = {"

configStrings[:shiftDefine] << 'enum nrf24l01p_field_shift{' 
configStrings[:maskDefine] << 'enum nrf24l01p_field_mask{' 

configStrings[:commands] << 'enum nrf24l01p_commands{' 

id = 0
fieldID = 0


nrfConfig.each do |register|
   configStrings[:regAddress] << "   NRF24L01P_REG_#{register[:name].upcase} = 0x#{register[:address].to_s(16)}," + "\n"

# handle names
   configStrings[:shiftList] << "\n" + '/* ' + register[:name] + ' */' + "\n" if(!register[:fields].empty?)
   configStrings[:maskList] << "\n" + '/* ' + register[:name] + ' */' + "\n" if(!register[:fields].empty?)
   configStrings[:shiftDefine] << "\n" + '/* ' + register[:name] + ' */' + "\n" if !register[:fields].empty?
   configStrings[:maskDefine] << "\n" + '/* ' + register[:name] + ' */' + "\n" if !register[:fields].empty?

   register[:fields].each do |field|
      configStrings[:fields] << "   NRF24L01P_FIELD_#{field[:name].upcase} = #{fieldID},\n"

      configStrings[:shiftList] << "   #{field[:bits][1]}," + ' /* ' + field[:name] + ' */' + "\n"
      configStrings[:shiftDefine] << "   NRF24L01P_SHIFT_#{field[:name].upcase} = #{field[:bits][1]}," + ' /* ' + field[:name] + ' */' + "\n"
   
      length = field[:bits][0] - field[:bits][1] + 1
      mask = "0x#{((2**length - 1) << field[:bits][1]).to_s(16)}"

      configStrings[:maskList] << "   #{mask}," +  ' /* ' + field[:name] + ' */' + "\n"
      configStrings[:maskDefine] << "   NRF24L01P_MASK_#{field[:name].upcase} = #{mask}," +  ' /* ' + field[:name] + ' */' + "\n"
      fieldID += 1
   end

   id += 1
end

nrfCommands.each do |cmd|
   configStrings[:commands] << "   NRF24L01P_CMD_#{cmd[:name].upcase} = 0x#{cmd[:value].to_s(16)},\n" 
end

configStrings.each_pair do |key, value|
   value << "};\n\n"
end


dest = File.open("nrf24l01p-const-#{defines == true ? 'defines' : 'list'}.h", 'w')

dest.puts("#ifndef NRF24L01P_CONST\n#define NRF24L01P_CONST\n\n")
dest.puts("#include <stdint.h>\n\n")

dest.puts(configStrings[:commands])
dest.puts(configStrings[:regAddress])

if(defines == false)
   dest.puts(configStrings[:fields])
   dest.puts(configStrings[:shiftList])
   dest.puts(configStrings[:maskList]) 
elsif(defines == true)
   dest.puts(configStrings[:shiftDefine])
   dest.puts(configStrings[:maskDefine]) 
else
   puts("Error, shouldn't be here. defines was #{defines.inspect}")
end

dest.puts("#endif\n")
dest.close
