


cp SPY_org.model SPY.model

nn1hmo -dump SPY.model > model_before.txt

for DATA in `ls xx*.dat`
do
	echo "=== $DATA ==="

	nn1hmo -p SPY.parm -f $DATA -learn -split n | tail 

	nn1hmo -p SPY.parm -f SPY_test.dat -test

	echo "Press [RETURN] to continue"
	read x
done

nn1hmo -dump SPY.model > model_after.txt

diff model_before.txt model_after.txt | more


