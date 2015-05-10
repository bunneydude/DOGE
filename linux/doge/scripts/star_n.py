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

single_pair = [
             {'node_id': 2, 'mcu_name': 'msp430g2553', 'mm_num': 1},
             {'node_id': 3, 'mcu_name': 'msp430g2553', 'mm_num': 1},
            ]

single_node = [
             {'node_id': 3, 'mcu_name': 'msp430g2553', 'mm_num': 1},
            ]


config['preset_nte_nodes'] = single_node

config['debug'] = False
config['debug_test_network'] = False

print("In debug mode? {0}".format(config['debug']))

rp_run()
