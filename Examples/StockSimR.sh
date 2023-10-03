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
		nn1hmo -p StockSimR.parm -algo
		;;
	'-train' )
		nn1hmo -p StockSimR.parm -train | tee StockSimRTrain.txt
		ls -l StockSimR.model
		echo "Press [ENTER] to view model detail"
		read x
		nn1hmo -dump StockSimR.model | more
		;;
	'-test' )
		nn1hmo -p StockSimR.parm -test | tee StockSimRTest.txt
		;;
	'-predict' )
		nn1hmo -p StockSimR.parm -predict -f ../Data/StockSimRPredict.csv | tee StockSimRPredict.txt
		;;
	'-clean' )
		rm -f StockSimRTrain.txt StockSimRTest.txt StockSimRPredict.txt StockSimR.model
		;;
	* )
		echo "USAGE: StockSimR.sh {-algo|-train|-test|-predict|-clean}"
		echo ""
		echo "Simulated stock market data to indicate BUY or NOT BUY signals."
		echo "The neural network has 75 input neurons, 45 hidden neurons in one"
		echo "layer and 1 output neuron."
		echo ""
		;;
esac


