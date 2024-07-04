#!/bin/bash
set -e
set -x

runname=idaten

InputPath=/raid/r0901/idaten24raw/vme
OutputPath=/raid/r0901/idaten24/rootfiles


makepidtree()
{
	jjjj=${1}
	InputFile=${InputPath}/${runname}${jjjj}.ridf
	#OutputFile=${OutputPath}/${runname}_${jjjj}_full.root
	OutputFile=${OutputPath}/${runname}${jjjj}.root
	MakePIDTree -i ${InputFile} -o ${OutputFile}
	touch --reference=${InputFile} ${OutputFile}
}



for iiii in {0089..0094}
do
	makepidtree ${iiii} &
	sleep 1
done
