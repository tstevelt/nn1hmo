nn1hmo – A neural network program written in C

What's with the funny name?  No doubt you guessed “nn” stands for Neural Network. The “1h” indicates program has One Hidden layer and “mo” means Multiple Outputs.

Here is the usage message:

USAGE: nn1hmo {-train|-test|-predict|-algo}
USAGE: nn1hmo [options] -p file [options]
USAGE: nn1hmo  -dump model
USAGE: nn1hmo  -foo
options:
 -f file  data file
 -g file  generate default parameter file
 -e #     epochs (for -train), default 2500
 -a #.#   target accuracy, default is 0.99
 -n flag  normalize data. flag n/r/c/a
 -d       debug

Data for training, testing and algo is comma separated with input columns followed by 1 or more output columns. Data for prediction is comma separated with one ID column followed by the same number of input columns. 

A parameter file is required in order to specify architecture. See Parameter File Structure on Page ### for details.

The -train, -test and -predict do what any other Neural Network program would do.  Training results in model file creation.

The -algo option runs a Genetic Algorithm on the training data in order to find reasonable values for the learning rate and also the number of neurons in the hidden layer. GA runs for 5 epochs, which usually starts to show convergence.

-p file: Specifies the parameter file

-f file: The data file to read for training, testing or prediction 

-dump model: Print the values in the model file (which is stored in binary format).

-g file: Generate a default parameter file.

-e #: Number of epochs to run.

-a #.#: Target accuracy, training stops when the is reached on five consecutive epochs.

-n flag: Normalize input data.  n=not, r=row wise, c=column wise and a=all over.

Note that position of parameter file will override previous arguments.  Likewise, arguments following the parameter file override the corresponding setting from the parameter file.

Training can be interrupted with either SIGINT or SIGUSR1 which will create a model file (of progress to date) and terminate the training session.  This is useful if you specified high number of epochs and can see accuracy is not improving.
Sample Data and Examples
The Data directory contains some sample CSV data files.  The Examples directory contains some shell scripts which demonstrate various features of nn1hmo.

FiveShapes.sh
This demonstrates neural network with multiple output and one hidden layer. There are 72 neurons in the input layer, 25 in the hidden layer and 5 in the output layer.

Data represents bitmapped images of five different shapes: square, vertical line, horizontal line, circle and random noise.

StockSim.sh
Simulated stock market data to indicate BUY or NOT BUY signals.  The neural network has 75 input neurons, 45 hidden neurons in one layer and 1 output neuron.

Parameter File Structure
Use the -g file option to create a default parameter file, as shown below.  Edit this file to specify the option shown in ALL CAPS. Adjust other values as needed. Leading pound signs indicate comments, which have no effects.
 
$ nn1hmo -g project.parm

# project.parm parameter file (auto generated).
#mode algo
mode train
#mode test
#mode predict
datafile /var/local/datasets/DIR/FILE
split	0.8 0.2
normalize n/r/c/a
# Maxinum INPUTS is 784
inputs	INPUTS
# Hidden Layers and Number of neuron in each layer
layers	1	NEURONS
# Maxinum OUTPUTS is 10
outputs	OUTPUTS
learnrate	0.1
epochs	2500
accuracy	0.99
model data.model
References

Massive thank you to Nicolai Nielsen for his YouTube and Github showing that it is quite possible to write ML in C language:
https://www.youtube.com/@NicolaiAI/featured
https://github.com/niconielsen32

I have not pursued development of a version supporting more than one hidden layer.  There is a bit of code in nn1hmo to lay the ground work for this, but I probably won’t pursue this unless I find a compelling need.  Here is an interesting discussion on StackExchange about multiple hidden layers:
https://stackoverflow.com/questions/34723489/how-do-multiple-hidden-layers-in-a-neural-network-improve-its-ability-to-learn
