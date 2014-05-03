#/usr/bin/bash

mpiexec -np 3 risk_exec data/orderAdvantage/ImmediateBest2_10000.txt 10000 2 > /dev/null;
echo done 2
mpiexec -np 3 risk_exec data/orderAdvantage/ImmediateBest3_10000.txt 10000 3 > /dev/null;
echo done 3
mpiexec -np 3 risk_exec data/orderAdvantage/ImmediateBest4_10000.txt 10000 4 > /dev/null;
echo done 4
mpiexec -np 3 risk_exec data/orderAdvantage/ImmediateBest5_10000.txt 10000 5 > /dev/null;
echo done 5
mpiexec -np 3 risk_exec data/orderAdvantage/ImmediateBest6_10000.txt 10000 6 > /dev/null;
echo done 6
