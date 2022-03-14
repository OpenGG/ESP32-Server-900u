from littlefs import LittleFS
import argparse
import os
import glob

parser = argparse.ArgumentParser(description='mklfs')
parser.add_argument('--block-size', '-b', type=int, default=4096, help='block size in bytes, default: 4096')
parser.add_argument('--read-size', '-r', type=int, default=128, help='read size in bytes, default: 128')
parser.add_argument('--prog-size', '-p', type=int, default=128, help='prog_size in bytes, default: 128')
parser.add_argument('--total-size', '-t', type=int, required=True, help='total size in bytes')
parser.add_argument('--directory', '-d', type=str, default='./data', help='data directory, default: ./data')
parser.add_argument('--output', '-o', type=str, default='./lfs.out', help='data directory, default: ./lfs.out')

args = parser.parse_args()
block_size = args.block_size
read_size = args.read_size
prog_size = args.prog_size
total_size = args.total_size
directory = args.directory
output = args.output

# print(block_size)
# print(read_size)
# print(prog_size)
# print(total_size)
# print(directory)
# print(output)

if (total_size % block_size != 0):
    raise Exception('Total size is not multiple of block size')

block_count = int(total_size / block_size)

fs = LittleFS(
    block_size=block_size,
    block_count=block_count,
    read_size=read_size,
    prog_size=prog_size
)

cwd = os.getcwd()

os.chdir(directory)
files = glob.glob('./**/*', recursive=True)
files.sort()

for fileName in files:
    if (os.path.isdir(fileName)):
        fs.mkdir(fileName)
    elif (os.path.isfile(fileName)):
        file = open(fileName, mode='rb')
        fileContent = file.read()
        file.close()
        with fs.open(fileName, 'wb') as fh:
            fh.write(fileContent)
            fh.close()
    else:
        raise Exception('Path %s is not directory or file' % fileName)

os.chdir(cwd)

# Dump the filesystem content to a file
with open(output, 'wb') as fh:
    fh.write(fs.context.buffer)
