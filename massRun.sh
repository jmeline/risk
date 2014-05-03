#/usr/bin/bash

mpiexec -np 3 risk_exec data/orderAdvantage/aggressive2_10000.txt 10000 2 > /dev/null;
echo done 2
mpiexec -np 3 risk_exec data/orderAdvantage/aggressive3_10000.txt 10000 3 > /dev/null;
echo done 3
mpiexec -np 3 risk_exec data/orderAdvantage/aggressive4_10000.txt 10000 4 > /dev/null;
echo done 4
mpiexec -np 3 risk_exec data/orderAdvantage/aggressive5_10000.txt 10000 5 > /dev/null;
echo done 5
mpiexec -np 3 risk_exec data/orderAdvantage/aggressive6_10000.txt 10000 6 > /dev/null;
echo done 6
