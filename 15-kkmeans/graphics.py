import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import sys

df = pd.read_csv(sys.argv[1], header=None, sep=';')
X = np.array(df[[0, 1]])
y = np.array(df[[2]][2])

N = df.shape[0]

colors = np.zeros(N).astype(np.str)
colors[y == 0] = 'green'
colors[y == 1] = 'orange'
colors[y == 2] = 'blue'

plt.figure(figsize=(15,10))
plt.scatter(X[:,0], X[:,1], c=colors, s=15)
plt.show()
