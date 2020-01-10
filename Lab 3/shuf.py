#!/usr/bin/python

import random, sys
from optparse import OptionParser

class shuf:
        def __init__(self, filename, args, count, input_range, repeat):
                self.arguments = args
                self.repeat = repeat
                self.count = count
                self.lines = []

                if input_range:
                        for x in range(len(input_range)):
                                self.lines.append(input_range[x] + '\n')
                        if not count and not repeat:
                                self.count = len(self.lines)
                elif filename == "None" or filename == "-":
                        self.lines = sys.stdin.readlines()
                        self.count = len(self.lines)
                else:
                        f = open(filename, 'r')
                        self.lines = f.readlines()
                        if not count and not repeat:
                                self.count = len(self.lines)
                        f.close()

                if count > len(self.lines) and not repeat:
                        self.count = len(self.lines)    

        def shuffleLines(self):
                if not self.repeat:
                        for index in range(self.count):
                                line = random.choice(self.lines)
                                sys.stdout.write(line)
                                self.lines.remove(line)
                else:
                        for index in range(self.count):
                                sys.stdout.write(random.choice(self.lines))
                return

        def repeatForever(self):
                while(self.repeat):
                        sys.stdout.write(random.choice(self.lines))
                return

def main():
        version_msg = "%prog 1.0"
        usage_msg = """%prog [OPTION]... FILE
        or: %prog -i INPUT_RANGE [OPTION]...
Write a random permutation of the input lines to standard output.

With no FILE, or when FILE is -, read standard input."""

        parser = OptionParser(version=version_msg, usage=usage_msg)
        parser.add_option("-n", "--head-count",
                        action="store", dest="count", default=0,
                        help="output at most COUNT lines")
        parser.add_option("-i", "--input-range",
                        action="store", dest="input_range", default="",
                        help="treat each number in INPUT_RANGE as an input line")
        parser.add_option("-r", "--repeat",
                        action="store_true", dest="repeat", default=False,
                        help="output lines can be repeated")

        options, args = parser.parse_args(sys.argv[1:])

        try:
                count = int(options.count)
        except:
                parser.error("invalid line count: {0}".format(options.count))
        
        try:
                if options.input_range:
                        args.append(options.input_range)
                        if options.input_range.count('-') == 1:
                                irange = options.input_range.split('-')
                                irange[0] = int(irange[0])
                                irange[1] = int(irange[1])
                                input_range = list(range(irange[0], irange[1]+1))
                                for x in range(len(input_range)):
                                        input_range[x] = str(input_range[x])
                        else:
                                raise ValueError
                else:
                        input_range = ""
        except:
                parser.error("invalid input range: {0}".format(options.input_range))
        
        try:
                repeat = bool(options.repeat)
        except:
                parser.error("invalid option -- {0}".format(options.repeat))

        if count < 0:
                parser.error("negative count: {0}".format(count))
        if len(args) > 1:
                parser.error("wrong number of operands")
    
        try:
                input_file = args[0]
        except:
                input_file = "None"

        try:
                generator = shuf(input_file, args, count, input_range, repeat)
                if generator.count != 0:
                        generator.shuffleLines()
                elif generator.repeat:
                        generator.repeatForever()
        except IOError as err:
                errno, strerror = err.args
                parser.error("I/O error({0}): {1}".format(errno, strerror))

if __name__ == "__main__":
    main()
