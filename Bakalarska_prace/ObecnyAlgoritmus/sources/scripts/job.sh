#!/bin/bash
#PBS -N MountainCar-v0-2500-obnova-tn-ki
#PBS -l select=1:ncpus=1:mem=8gb:scratch_local=1gb
#PBS -l walltime=24:00:00

trap 'clean_scratch' TERM EXIT

cd $SCRATCHDIR || exit 1

source /storage/brno2/home/xbucha02/.bash_profile

cp /storage/brno2/home/xbucha02/tests_for_metacentrum/MountainCar-v0/obnova-tn-ki/* $SCRATCHDIR

python3 main.py >results.out


folder=$(date +%F-MountainCar-v0-2500-obnova-tn-ki-%T)
mkdir /storage/brno2/home/xbucha02/$folder

cp * /storage/brno2/home/xbucha02/$folder || export CLEAN_SCRATCH=false