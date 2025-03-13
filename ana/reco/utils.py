import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import MinMaxScaler
from tensorflow import keras
from tensorflow.keras import layers
from tensorflow.keras import regularizers
from tensorflow.keras.callbacks import EarlyStopping, ReduceLROnPlateau


class VertexReconstruction:

    def __init__(self, file_in: str):
        self.file_in = file_in
        self.data = self.load_data()
        self.X, self.y = self.prepare_data()
        self.scaler = MinMaxScaler(feature_range=(-1, 1))
        self.model = None

    def load_data(self) -> pd.DataFrame:
        '''This function loads the data from the source file.
        The data structure must be something like
        x1 y1 z1 ... x4 y4 z4 z_vertex_sim'''
        return pd.read_csv(self.file_in, delimiter=' ', header=None)

    def prepare_data(self):
        '''This function Transforms the data to take the four 3D points
        as input and the simulated z-vertex as output.'''
        X = self.data.iloc[:, :12].values
        y = self.data.iloc[:, 12].values
        return X, y

    def load_model(self, model_path: str):
        '''This function loads a model given its path (it has to point
        to a .h5 file).'''
        self.model = keras.models.load_model(model_path, compile=False)
        self.model.compile(optimizer='adam', loss='huber')

        # The model has been trained with scaled data so we have to do the same
        self.X = self.scaler.fit_transform(self.X)

        # Predict over the whole set of data
        self.y_predicted = self.model.predict(self.X).flatten()
        self.y_real = self.y

    def train_model(self):
        '''This function will take X and y data an use it to train a NN.
        Only the 20% of the data will be used for testing, and therefore
        only this part of the data will be presented as the result'''
        # Split the data in train and test sets
        X_train, X_test, y_train, y_test = train_test_split(
            self.X, self.y, test_size=0.2, random_state=42)

        # Scale the data
        X_train = self.scaler.fit_transform(X_train)
        X_test = self.scaler.transform(X_test)

        # Create a new model
        self.model = keras.Sequential([
            layers.Dense(512, activation='relu', input_shape=(
                12,), kernel_regularizer=regularizers.l2(0.00005)),
            layers.Dropout(0.2),
            layers.Dense(256, activation='relu',
                         kernel_regularizer=regularizers.l2(0.00005)),
            layers.Dropout(0.2),
            layers.Dense(128, activation='relu',
                         kernel_regularizer=regularizers.l2(0.00005)),
            layers.Dropout(0.1),
            layers.Dense(64, activation='relu',
                         kernel_regularizer=regularizers.l2(0.00005)),
            layers.Dropout(0.1),
            layers.Dense(32, activation='relu',
                         kernel_regularizer=regularizers.l2(0.00005)),
            layers.Dense(1)
        ])
        self.model.compile(optimizer=keras.optimizers.Adam(
            learning_rate=1e-4), loss='huber')

        # Callbacks
        early_stopping = EarlyStopping(
            monitor='val_loss', patience=20, restore_best_weights=True)
        reduce_lr = ReduceLROnPlateau(
            monitor='val_loss', factor=0.5, patience=10, min_lr=1e-6)

        # Training
        self.model.fit(X_train, y_train, epochs=200,
                       batch_size=64, validation_split=0.1,
                       callbacks=[early_stopping, reduce_lr])

        # Save the model in the current directory
        self.model.save("model.keras")

        # Predict only for the test set
        self.y_predicted = self.model.predict(X_test).flatten()
        self.y_real = y_test

    def predict(self):
        '''This function returns the result of the vertex reconstruction
        i.e. the vector with the predicted data and the vector with the
        real data.'''
        if self.model is None:
            print("No model has been loaded yet. A new one is going to be trained:\n")
            self.train_model()

        return self.y_predicted, self.y_real

    @staticmethod
    def save_results(file_out: str, y_predicted, y_real):
        """Save the predicted and real vertex values to a file."""
        with open(file_out, "w") as f:
            for zrec, zsim in zip(y_predicted, y_real):
                f.write(f'{zrec} {zsim}\n')
