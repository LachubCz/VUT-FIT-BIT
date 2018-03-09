#!/bin/bash
#PBS -N new-CartPole-v0-2500-neobnova-tn-ki
#PBS -l select=1:ncpus=1:mem=8gb:scratch_local=1gb
#PBS -l walltime=24:00:00

trap 'clean_scratch' TERM EXIT

cd $SCRATCHDIR || exit 1

source /storage/brno2/home/xbucha02/.bash_profile

cp /storage/brno2/home/xbucha02/tests_for_metacentrum/CartPole-v0/neobnova-tn-ki/* $SCRATCHDIR

python3 main.py >results.out

name='new-CartPole-v0-2500-neobnova-tn-ki'
folder=$name'-'$(date +%F-%T)
mkdir /storage/brno2/home/xbucha02/$folder

cp * /storage/brno2/home/xbucha02/$folder || export CLEAN_SCRATCH=false