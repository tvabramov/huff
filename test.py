# TODO: optimize file pathing in TestFileEnvironment for different platforms
from scripttest import TestFileEnvironment
import filecmp
import os

env = TestFileEnvironment('./scratch')

# Just checks that encoded-decoded file is equal to original one.
def ordinary_cases():
    env.clear()
    for filename in os.listdir('./test_data'):
        print("Testing file: " + filename + " ...")
        result_encode = env.run('..\\encode', '..\\test_data\\' + filename, 'encoded.dat')
        result_decode = env.run('..\\decode', 'encoded.dat', 'decoded.dat')
        assert filecmp.cmp('./test_data/' + filename, './scratch/decoded.dat')
        print(filename + " is OK")
#damaged encoded

if __name__ == "__main__":
    ordinary_cases()
    print("Everything passed")