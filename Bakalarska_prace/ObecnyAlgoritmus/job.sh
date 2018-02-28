#!/bin/bash
#PBS -N experiment
#PBS -l select=1:ncpus=4:mem=2gb:scratch_local=1gb
#PBS -l walltime=4:00:00

trap 'clean_scratch' TERM EXIT

cd $SCRATCHDIR || exit 1

source /storage/brno2/home/xbucha02/.bash_profile

cp /storage/brno2/home/xbucha02/sources/* $SCRATCHDIR

python3 main.py >results.out

folder=$(date +%F-%T)
mkdir ~/$folder

cp * /storage/brno2/home/xbucha02/ || export CLEAN_SCRATCH=false