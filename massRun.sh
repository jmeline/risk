#/usr/bin/bash

FILE="preyOnWeak"
SIZE=10000

mpiexec -np 3 risk_exec data/orderAdvantage/${FILE}2_${SIZE}.txt ${SIZE} 2;
echo done 2
mpiexec -np 3 risk_exec data/orderAdvantage/${FILE}3_${SIZE}.txt ${SIZE} 3 > /dev/null;
echo done 3
mpiexec -np 3 risk_exec data/orderAdvantage/${FILE}4_${SIZE}.txt ${SIZE} 4 > /dev/null;
echo done 4
mpiexec -np 3 risk_exec data/orderAdvantage/${FILE}5_${SIZE}.txt ${SIZE} 5 > /dev/null;
echo done 5
mpiexec -np 3 risk_exec data/orderAdvantage/${FILE}6_${SIZE}.txt ${SIZE} 6 > /dev/null;
echo done 6
