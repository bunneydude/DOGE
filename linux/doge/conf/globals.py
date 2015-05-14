'''
Created on Apr 25, 2015

@author: Daveman
'''
from collections import defaultdict

config = defaultdict()

# Debug Flag (project-wide)
config['debug'] = True
config['debug_no_sketch'] = True #Don't send data to sketch via IPCBuffer
config['debug_test_network'] = False #Use a preset network for neighbor/routing edges


# Preset Neighbor Table Nodes
nte_nodes = [
             {'node_id': 2, 'mcu_name': 'msp430g2553', 'mm_num': 1},
             {'node_id': 3, 'mcu_name': 'msp430g2553', 'mm_num': 1},
             {'node_id': 4, 'mcu_name': 'msp430g2553', 'mm_num': 1},
             {'node_id': 5, 'mcu_name': 'msp430g2553', 'mm_num': 1},
             {'node_id': 6, 'mcu_name': 'msp430g2553', 'mm_num': 1},
             {'node_id': 7, 'mcu_name': 'msp430g2553', 'mm_num': 1},
             {'node_id': 8, 'mcu_name': 'msp430g2553', 'mm_num': 1},
             {'node_id': 9, 'mcu_name': 'msp430g2553', 'mm_num': 1},
             {'node_id': 10, 'mcu_name': 'msp430g2553', 'mm_num': 1}
            ]

demo_routes = [
               [3, 2],
               [4, 2],
               [5, 2],
               [6, 2],
               [7, 3],
               [8, 2],
               [9, 2],
               [10,3],
              ]


config['preset_nte_nodes'] = nte_nodes
config['preset_routes'] = demo_routes

# Relevant config file location paths
doge_paths = {
              'root_directory_path':    '.',
              'intel_credentials_file': 'doge/cloud/inteL_credentials.txt',
              'mm_map_default_profile': 'doge/radio/mm_msp430_v1.txt',
             }
config['config_file_paths'] = doge_paths

# Other configs to go down here
