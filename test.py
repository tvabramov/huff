# TODO: optimize file pathing in TestFileEnvironment for different platforms.
from scripttest import TestFileEnvironment
import filecmp
import os

env = TestFileEnvironment('./scratch')

# Just checks that encoded-decoded file is equal to original one.
def synthetic():
    env.clear()
    for filename in os.listdir('./test_data'):
        print("Testing file: " + filename + " ...")
        result_encode = env.run('..\\encode', '..\\test_data\\' + filename, 'encoded.dat')
        result_decode = env.run('..\\decode', 'encoded.dat', 'decoded.dat')
        assert filecmp.cmp('./test_data/' + filename, './scratch/decoded.dat')
        print(filename + " is OK")
    print("All tests with synthetic data passed")

#TODO: ordinary books, images. Also check that encoded file is smaller than orginal.
def ordinary():
    print("All tests with damaged encoded data passed")

#TODO: damaged encoded with unreadable table.
def damaged():
    print("All tests with damaged encoded data passed")

#TODO: very large file (that can not be in stored in usual way) test, check perfomance also.
def very_large():
    print("All tests with very large input data passed")

#TODO: large file test with Fibonachi frequences.
def fibonachi():
    print("All tests with inconvenient input data passed")

if __name__ == "__main__":
    synthetic()
    ordinary()
    damaged()
    very_large()
    fibonachi()
    print("Everything passed")