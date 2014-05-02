#/usr/bin/bash

mpiexec -np 3 risk_exec data/orderAdvantage/smallestFirst2_10000.txt 10000 2 > /dev/null;
mpiexec -np 3 risk_exec data/orderAdvantage/smallestFirst3_10000.txt 10000 3 > /dev/null;
mpiexec -np 3 risk_exec data/orderAdvantage/smallestFirst4_10000.txt 10000 4 > /dev/null;
mpiexec -np 3 risk_exec data/orderAdvantage/smallestFirst5_10000.txt 10000 5 > /dev/null;
mpiexec -np 3 risk_exec data/orderAdvantage/smallestFirst6_10000.txt 10000 6 > /dev/null;
