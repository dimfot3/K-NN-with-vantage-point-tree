## scripts

Here there are some basic python scripts.
In order to run without problem in depedencies download the requirement packages by running ``pip install -r requirements.txt``.
The scripts in this folder are:

1. ``create_data.py`` : this script can create data for the C program and save them in data folder in root. The data can come from three distributions. Run ``python3 create_data.py --help`` to see the options.
2. ``datasets.py`` : this script can create data for the C program and save them in data folder in root. The data can come from MNIST dataset. Run ``python3 datasets.py --help`` to see the options.
3. ``evaluate_data.py``: this script contains the python implementation for both vantage point tree and KNN search. For the second we use the sklearn's implementation uppon KD trees.
4. ``demo_script.sh`` : If you have download the sample data that we provide, you can run this scipt and will execute all program's functions


