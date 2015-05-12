from doge.core.DogeHub import *
from doge.radio.RadioInterface import RadioInterface
from doge.radio.Node import *
from doge.radio import mmFields
import sys


star_nte = [
             {'node_id': 2, 'mcu_name': 'msp430g2553', 'mm_num': 1},
             {'node_id': 3, 'mcu_name': 'msp430g2553', 'mm_num': 1},
             {'node_id': 4, 'mcu_name': 'msp430g2553', 'mm_num': 1},
             {'node_id': 5, 'mcu_name': 'msp430g2553', 'mm_num': 1},
            ]

demo_nte = [
             {'node_id': 2, 'mcu_name': 'msp430g2553', 'mm_num': 1},
             {'node_id': 3, 'mcu_name': 'msp430g2553', 'mm_num': 1},
             {'node_id': 4, 'mcu_name': 'msp430g2553', 'mm_num': 1},
             {'node_id': 5, 'mcu_name': 'msp430g2553', 'mm_num': 1},
             {'node_id': 6, 'mcu_name': 'msp430g2553', 'mm_num': 1},
             {'node_id': 7, 'mcu_name': 'msp430g2553', 'mm_num': 1},
             {'node_id': 8, 'mcu_name': 'msp430g2553', 'mm_num': 1},
             {'node_id': 9, 'mcu_name': 'msp430g2553', 'mm_num': 1},
             {'node_id': 10, 'mcu_name': 'msp430g2553', 'mm_num': 1},
	   ]

single_pair = [
             {'node_id': 2, 'mcu_name': 'msp430g2553', 'mm_num': 1},
             {'node_id': 3, 'mcu_name': 'msp430g2553', 'mm_num': 1},
            ]

single_node = [
             {'node_id': 3, 'mcu_name': 'msp430g2553', 'mm_num': 1},
            ]


config['preset_nte_nodes'] = demo_nte

config['debug'] = True
config['debug_test_network'] = True

print("In debug mode? {0}".format(config['debug']))

rp_run()
