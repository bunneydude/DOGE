'''
Created on Apr 25, 2015

@author: Daveman
'''
from collections import defaultdict

config = defaultdict()

# Debug Flag (project-wide)
config['debug'] = True
config['debug_no_sketch'] = False #Don't send data to sketch via IPCBuffer
config['debug_test_network'] = False #Use a preset network for neighbor/routing edges

config['radio_log_level'] = 1 

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
             {'node_id': 10, 'mcu_name': 'msp430g2553', 'mm_num': 1},
            ]

demo_routes = [
               [10,2],
               [9,2],
               [8,2],
               [7,2],
               [6,2],
               [5,2],
               [4,2],
               [3,2],
              ]

dual_box = {2:{"narray":[[1,2,2,1], [3,2,2,1], [5,2,2,1], [6,2,2,1]], "rarray":[[10, 2, 1], [10, 2, 2], [10, 2, 3]]},
            3:{"narray":[[1,3,2,1], [2,3,2,1], [4,3,2,1]], "rarray":[[1,3,1], [10,3,2]]},
            4:{"narray":[[1,4,2,1], [3,4,2,1], [7,4,2,1]], "rarray":[[1,4,1], [10,4,2]]},
            5:{"narray":[[1,5,2,1], [2,5,2,1], [8,5,2,1]], "rarray":[[1,5,1], [10,5,2]]},
            6:{"narray":[[1,6,2,1], [2,6,2,1], [7,6,2,1], [9,6,2,1]], "rarray":[[1,6,1], [10,6,2], [10,6,3]]},
            7:{"narray":[[1,7,2,1], [4,7,2,1], [6,7,2,1], [10,7,2,1]], "rarray":[[1,7,1], [10,7,2]]},
            8:{"narray":[[1,8,2,1], [5,8,2,1], [9,8,2,1]], "rarray":[[1,8,1], [10,8,2]]},
            9:{"narray":[[1,9,2,1], [6,9,2,1], [8,9,2,1], [10,9,2,1]], "rarray":[[10,9,1], [1,9,2]]},
            10:{"narray":[[1,10,2,1], [7,10,2,1], [9,10,2,1]], "rarray":[[1,10,1], [1,10,2]]},
#            11:{"narray":[[1,11,2,2]], "rarray":[]},
#            12:{"narray":[[1,12,2,2]], "rarray":[]},
#            13:{"narray":[[1,13,2,2]], "rarray":[]},
           }
            


config['preset_nte_nodes'] = nte_nodes
config['preset_routes'] = demo_routes
config['preset_node_tables'] = dual_box

# Relevant config file location paths
doge_paths = {
              'root_directory_path':    '.',
              'intel_credentials_file': 'doge/cloud/inteL_credentials.txt',
              'mm_map_default_profile': 'doge/radio/mm_msp430_v1.txt',
             }
config['config_file_paths'] = doge_paths

# Other configs to go down here
