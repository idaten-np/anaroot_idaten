#!/bin/bash
set -e
set -x

#runname=idatentest
runname=idaten

InputPath=/home/gamma/IDATEN/2024_spring/vme
OutputPath=/home/gamma/IDATEN/2024_spring/ROOT/vme


makepidtree()
{
	jjjj=${1}
	InputFile=${InputPath}/${runname}${jjjj}.ridf
	#OutputFile=${OutputPath}/${runname}${jjjj}_full.root
	#MakeFullBigRIPSTree -i ${InputFile} -o ${OutputFile}
	OutputFile=${OutputPath}/${runname}${jjjj}.root
	MakePIDTree -i ${InputFile} -o ${OutputFile}
	#touch --reference=${InputFile} ${OutputFile}
	#chgrp gamma ${OutputFile}
}



for iiii in {0003..0003}
do
	makepidtree ${iiii} 
	sleep 1
done
