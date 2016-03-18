#!/usr/bin/python

import numpy as np
import matplotlib.pyplot as plt

infile = 'testairspy'
fd = open(infile, 'rb')

offset = 0
fft_size = 1000000
# move to position in file
fd.seek(offset,0)

# straight to numpy data (no buffering) 
# Adjust next line depending on data type
numpy_data = np.fromfile(fd, dtype = np.dtype('>i2'), count = fft_size)

plt.plot(numpy_data)
plt.show()
