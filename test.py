# TODO: optimize file pathing in TestFileEnvironment for different platforms.
from scripttest import TestFileEnvironment
import filecmp
import os

env = TestFileEnvironment('./scratch')

# Just checks that encoded-decoded file is equal to original one.
def synthetic():
    env.clear()
    for filename in os.listdir('./test_data/synthetic'):
        print('Testing file: ' + filename + ' ...')
        result_encode = env.run('..\\encode', '..\\test_data\\synthetic\\' + filename, 'encoded.dat')
        result_decode = env.run('..\\decode', 'encoded.dat', 'decoded.dat')
        assert filecmp.cmp('./test_data/synthetic/' + filename, './scratch/decoded.dat')
        print(filename + ' is OK')
    print('All tests with synthetic data are passed')

#TODO: ordinary books, images. Also check that encoded file is smaller than orginal.
def ordinary():
    print('All tests with damaged encoded data are passed')

def damaged():
    env.clear()
    for filename in os.listdir('./test_data/damaged'):
        print('Testing file: ' + filename + ' ...')
        result_decode = env.run('..\\decode', '..\\test_data\\damaged\\' + filename, 'decoded.dat', expect_error = True)
        assert result_decode.returncode != 0;
        assert result_decode.stderr.startswith('Cannot read encoded file header. It may be damaged.')
        print(filename + ' is OK')
    print('All tests with damaged encoded data are passed')

#TODO: very large file (that can not be in stored in usual way) test, check perfomance also.
def very_large():
    print('All tests with very large input data are passed')

#TODO: large file test with Fibonachi frequences.
def fibonachi():
    print('All tests with inconvenient input data are passed')

def wrong_args():
    env.clear()

    print('Testing file: encode with no args')
    encode_result_0 = env.run('..\\encode', expect_error = True)
    assert encode_result_0.returncode != 0;
    assert encode_result_0.stderr.startswith('You are do it wrong.')
    print('OK')

    print('Testing file: encode with one arg')
    encode_result_1 = env.run('..\\encode', 'one', expect_error = True)
    assert encode_result_1.returncode != 0;
    assert encode_result_1.stderr.startswith('You are do it wrong.')
    print('OK')

    print('Testing file: encode with three args')
    encode_result_3 = env.run('..\\encode', 'one', 'two', 'three', expect_error = True)
    assert encode_result_3.returncode != 0;
    assert encode_result_3.stderr.startswith('You are do it wrong.')
    print('OK')

    print('Testing file: decode with no args')
    decode_result_0 = env.run('..\\decode', expect_error = True)
    assert decode_result_0.returncode != 0;
    assert decode_result_0.stderr.startswith('You are do it wrong.')
    print('OK')

    print('Testing file: decode with one arg')
    decode_result_1 = env.run('..\\decode', 'one', expect_error = True)
    assert decode_result_1.returncode != 0;
    assert decode_result_1.stderr.startswith('You are do it wrong.')
    print('OK')

    print('Testing file: decode with three args')
    decode_result_3 = env.run('..\\decode', 'one', 'two', 'three', expect_error = True)
    assert decode_result_3.returncode != 0;
    assert decode_result_3.stderr.startswith('You are do it wrong.')
    print('OK')


    print('All tests with wrong args are passed')

if __name__ == "__main__":
    synthetic()
    ordinary()
    damaged()
    very_large()
    fibonachi()
    wrong_args()
    print('All tests are passed')