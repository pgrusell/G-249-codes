'''
This python script makes an exhaustive analysis of the desired algorithm
performance. Particularly it will represent:

- The simulated distribution and the reconstructed distribution. 
- The error on the vertex reconstruction.
- The correlation between reconstructed and real componentes.
'''

import sys
import matplotlib.pyplot as plt
import numpy as np
from scipy.stats import pearsonr

# The file must be the first argument after the pyhton program
file_in = sys.argv[1]

z_real_list = []
z_rec_list = []

with open(file_in, "r") as f:
    for line in f:
        z_rec, z_real = line.split(' ')

        z_real = float(z_real)
        z_rec = float(z_rec)
        # we will just analyse what happens inside the vacuum chamber
        if (abs(z_real) < 20) and (abs(z_rec) < 50):
            z_real_list.append(z_real)
            z_rec_list.append(z_rec)


# Vertexes distributions
fig, ax = plt.subplots()
ax.hist(z_real_list, bins=np.linspace(-20, 20, 100),
        edgecolor='black', alpha=0.2, label='real')
ax.hist(z_rec_list, bins=np.linspace(-20, 20, 100),
        edgecolor='black', alpha=0.2, label='reconstructed')
ax.legend()
plt.show()

# Error distribution
error = [reci - reali for reci, reali in zip(z_rec_list, z_real_list)]

fig, ax = plt.subplots()
ax.hist(error, bins=np.linspace(-20, 20, 100), edgecolor='black',
        alpha=0.2, label=f'mean = {np.mean(error): .3f}\nstd = {np.std(error): .3f}')
ax.legend()
plt.show()

# Calcular el coeficiente de correlación de Pearson
correlation, _ = pearsonr(z_real_list, z_rec_list)

# Graficar el histograma 2D
fig, ax = plt.subplots()
h = ax.hist2d(z_real_list, z_rec_list, bins=np.linspace(0, 10, 100))

# Agregar la leyenda con el coeficiente de correlación
plt.colorbar(h[3], ax=ax, label="Frecuencia")
ax.set_title(f"Coeficiente de correlación: {correlation:.4f}")
ax.set_xlabel("z_real")
ax.set_ylabel("z_rec")

plt.show()
