import sys
import cv2
import os
from sys import platform
import argparse
import time

try:
    dir_path = os.path.dirname(os.path.realpath(__file__))
    try:
        sys.path.append('../../python')
        from openpose import pyopenpose as op
    except ImportError as e:
        print(
            'Error: OpenPose library could not be found. Did you enable `BUILD_PYTHON` in CMake and have this Python '
            'script in the right folder?')
        raise e

    parser = argparse.ArgumentParser()

    params = dict()
    params["model_folder"] = "../../../models/"

    # Starting OpenPose
    opWrapper = op.WrapperPython()
    opWrapper.configure(params)
    opWrapper.start()

    # get all files in /DigitalICU/record_rgb
    input_folder = './input-folder'
    output_folder = './output-folder'
    os.makedirs(output_folder, exist_ok=True)

    files = [f for f in os.listdir(input_folder) if f.endswith(".png")]
    for file in files:
        path = os.path.join(input_folder, file)
        print(f'Processing {path}')
        datum = op.Datum()
        imageToProcess = cv2.imread(path)
        datum.cvInputData = imageToProcess
        opWrapper.emplaceAndPop(op.VectorDatum([datum]))

        output_path = os.path.join(output_folder, file)
        cv2.imwrite(output_path, datum.cvOutputData)
        print(f'Saved output to {output_path}')


except Exception as e:
    print(e)
    sys.exit(-1)
