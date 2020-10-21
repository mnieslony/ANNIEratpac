#!/bin/sh

for i in {0..100}
do
	echo ${i}
	./txt2rat -i /pnfs/annie/persistent/users/mnieslon/genie/GENIE2RAT/SANDI_1k_10-15-20.${i}.txt -o /pnfs/annie/persistent/users/mnieslon/genie/GENIE2RAT/SANDI_1k_10-15-20.${i}.root
done
