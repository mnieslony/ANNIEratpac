#!/bin/sh

for i in {0..100}
do
	echo ${i}
	./genie2txt -i /pnfs/annie/persistent/users/mnieslon/genie/SANDI_1k_10-15-20/gntp.${i}.ghep.root -o /pnfs/annie/persistent/users/mnieslon/genie/GENIE2RAT/SANDI_1k_10-15-20.${i}.txt
done
