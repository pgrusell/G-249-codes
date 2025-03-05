'''
This python script will use a NN to reconstruct the vertex from 
simulated data. Should be used as follows:

$ python file_in file_out model_path

:file_in: File that contains the results of the simulation
:file_out: File that will store the results of the reconstruction. The 
            structure will be z-reconstructed z-simulated
:model_path: .h5 file that contains a trained NN. This is optional and, if not
            provided, the majority part of simulated will be used for the 
            training and only the 20% will be displayed as a result.
'''

import numpy as np
import pandas as pd
import sys
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from tensorflow import keras
from tensorflow.keras import layers

# ------ Data access --------
file_in = sys.argv[1]
data = pd.read_csv(file_in, delim_whitespace=True, header=None)

# Source file must have the form
# x1 y1 z1 ... x4 y4 z4 z_vertex_sim
X = data.iloc[:, :12].values
y = data.iloc[:, 12].values

# ------ Model -----------
model_path = sys.argv[3] if len(sys.argv) == 4 else None

# If a trained NN is provided, use it to reconstruct the vertex
if model_path:
    model = keras.models.load_model(model_path, compile=False)
    model.compile(optimizer='adam', loss='mse')

    # Normalizing the input data with StandardScaler
    scaler = StandardScaler()
    # Assuming model was trained with normalized data
    X = scaler.fit_transform(X)

    # Predict over the whole set of data
    y_predict = model.predict(X).flatten()
    y_real = y

# If no model is provided, train a new one
else:
    # ------- Data modification ---------
    X_train, X_test, y_train, y_test = train_test_split(
        X, y, test_size=0.2, random_state=42)

    # Data normalization
    scaler = StandardScaler()
    X_train = scaler.fit_transform(X_train)
    X_test = scaler.transform(X_test)

    # Defining a more complex neural network
    model = keras.Sequential([
        layers.Dense(256, activation='relu', input_shape=(12,)),
        layers.Dense(512, activation='relu'),
        layers.Dense(256, activation='relu'),
        layers.Dense(128, activation='relu'),
        layers.Dense(64, activation='relu'),
        layers.Dense(32, activation='relu'),
        layers.Dense(1)
    ])

    model.compile(optimizer='adam', loss='mse')

    # Training
    model.fit(X_train, y_train, epochs=100,
              batch_size=32, validation_split=0.1)

    # Save the model
    model.save('model.h5')

    # Predict only for the test set
    y_predict = model.predict(X_test).flatten()
    y_real = y_test

# -------- Real output ---------
file_out = sys.argv[2]
with open(file_out, "w") as f:
    for pred, real in zip(y_predict, y_real):
        f.write(f'{pred} {real}\n')
