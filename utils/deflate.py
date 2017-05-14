#!/usr/bin/env python
import argparse
import zlib

def compress(in_fn, out_fn):
    compressor = zlib.compressobj(9, zlib.DEFLATED, -zlib.MAX_WBITS, zlib.DEF_MEM_LEVEL, 0)
    with open(in_fn) as in_f:
        dat = in_f.read()
        deflated = compressor.compress(dat)
        deflated += compressor.flush()
        if out_fn == '-':
            print deflated
        else:
            with open(out_fn, 'w+') as out_f:
                out_f.write(deflated)

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("input")
    parser.add_argument("output")
    args = parser.parse_args()
    compress(args.input, args.output)

if __name__ == '__main__':
    main()
