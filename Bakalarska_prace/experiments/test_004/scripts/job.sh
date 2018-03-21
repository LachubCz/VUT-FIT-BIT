#!/bin/bash
#PBS -N Acrobot-v1-2500-DQN-priorMem-dueling
#PBS -l select=1:ncpus=1:mem=4gb:scratch_local=1gb
#PBS -l walltime=15:00:00

trap 'clean_scratch' TERM EXIT

cd $SCRATCHDIR || exit 1

source /storage/brno2/home/xbucha02/.bash_profile

cp /storage/brno2/home/xbucha02/test_004/Acrobot-v1/DQN-priorMem-dueling/* $SCRATCHDIR

python3 main.py >results.out

name='Acrobot-v1-2500-DQN-priorMem-dueling'
folder=$name'-'$(date +%F-%T)
mkdir /storage/brno2/home/xbucha02/$folder

cp * /storage/brno2/home/xbucha02/$folder || export CLEAN_SCRATCH=false