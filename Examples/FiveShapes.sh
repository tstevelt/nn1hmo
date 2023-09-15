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
		nn1hmo -p FiveShapes.parm -algo
		;;
	'-train' )
		nn1hmo -p FiveShapes.parm -train | tee FiveShapesTrain.txt
		ls -l FiveShapes.model
		echo "Press [ENTER] to view model detail"
		read x
		nn1hmo -dump FiveShapes.model | more
		;;
	'-test' )
		nn1hmo -p FiveShapes.parm -test | tee FiveShapesTest.txt
		;;
	'-predict' )
		nn1hmo -p FiveShapes.parm -predict -f ../Data/FiveShapesPredict.csv | tee FiveShapesPredict.txt
		;;
	'-clean' )
		rm -f FiveShapesTrain.txt FiveShapesTest.txt FiveShapesPredict.txt FiveShapes.model
		;;
	* )
		echo "USAGE: FiveShapes.sh {-algo|-train|-test|-predict|-clean}"
		echo ""
		echo "This demonstrates neural network with multiple output and one hidden layer."
		echo "There are 72 neurons in the input layer, 25 in the hidden layer and 5 in"
		echo "the output layer."
		echo ""
		echo "Data represents bitmapped images of five different shapes: square, vertical"
		echo "line, horizontal line, circle and random noise."
		echo ""
		;;
esac

