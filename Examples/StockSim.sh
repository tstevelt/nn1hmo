#!/bin/sh


case "$1" in
	'-algo' )
		echo "Run a Genetic Algoritm to find optimum number"
		echo "of neurons for hidden layer, as well as reasonable"
		echo "learning rate.  GA runs for 5 epochs, which usually"
		echo "starts to show convergence."
		echo ""
		echo "Press [ENTER] to view model detail"
		read x
		nn1hmo -p StockSim.parm -algo
		;;
	'-train' )
		nn1hmo -p StockSim.parm -train | tee StockSimTrain.txt
		ls -l StockSim.model
		echo "Press [ENTER] to view model detail"
		read x
		nn1hmo -dump StockSim.model | more
		;;
	'-test' )
		nn1hmo -p StockSim.parm -test | tee StockSimTest.txt
		;;
	'-predict' )
		nn1hmo -p StockSim.parm -predict -f ../Data/StockSimPredict.csv | tee StockSimPredict.txt
		;;
	'-clean' )
		rm -f StockSimTrain.txt StockSimTest.txt StockSimPredict.txt StockSim.model
		;;
	* )
		echo "USAGE: StockSim.sh {-algo|-train|-test|-predict|-clean}"
		echo ""
		echo "Simulated stock market data to indicate BUY or NOT BUY signals."
		echo "The neural network has 75 input neurons, 45 hidden neurons in one"
		echo "layer and 1 output neuron."
		echo ""
		;;
esac


