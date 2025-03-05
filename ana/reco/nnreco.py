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

import sys
from utils import VertexReconstruction


def main():

    # Get the file inputs
    file_in = sys.argv[1]
    file_out = sys.argv[2]
    model_path = sys.argv[3] if len(sys.argv) == 4 else None

    # Create an object from utils.py
    vertex_reconstructor = VertexReconstruction(file_in)

    # If a model has been provided, we loaded it
    if model_path:
        vertex_reconstructor.load_model(model_path)

    # Predict the results from the input file
    # Notice that if no model is loaded this step will
    # train a new one, but the predicted results will be decreased
    # in a 80 %.
    y_predicted, y_real = vertex_reconstructor.predict()

    # Store the info in the output file
    vertex_reconstructor.save_results(file_out, y_predicted, y_real)


if __name__ == "__main__":
    main()
