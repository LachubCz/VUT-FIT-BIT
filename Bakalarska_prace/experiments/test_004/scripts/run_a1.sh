#Acrobot-v1
sed "2 c\#PBS -N Acrobot-v1-2500-DDQN-basicMem" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

scratch='$SCRATCHDIR'
sed "12 c\cp /storage/brno2/home/xbucha02/test_004/Acrobot-v1/DDQN-basicMem/* $scratch" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

sed "16 c\name='Acrobot-v1-2500-DDQN-basicMem'" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

qsub job.sh
qsub job.sh
qsub job.sh
qsub job.sh
qsub job.sh
##############################################################################
sed "2 c\#PBS -N Acrobot-v1-2500-DDQN-basicMem-dueling" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

scratch='$SCRATCHDIR'
sed "12 c\cp /storage/brno2/home/xbucha02/test_004/Acrobot-v1/DDQN-basicMem-dueling/* $scratch" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

sed "16 c\name='Acrobot-v1-2500-DDQN-basicMem-dueling'" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

qsub job.sh
qsub job.sh
qsub job.sh
qsub job.sh
qsub job.sh
##############################################################################
sed "2 c\#PBS -N Acrobot-v1-2500-DDQN-priorMem" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

scratch='$SCRATCHDIR'
sed "12 c\cp /storage/brno2/home/xbucha02/test_004/Acrobot-v1/DDQN-priorMem/* $scratch" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

sed "16 c\name='Acrobot-v1-2500-DDQN-priorMem'" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

qsub job.sh
qsub job.sh
qsub job.sh
qsub job.sh
qsub job.sh
##############################################################################
sed "2 c\#PBS -N Acrobot-v1-2500-DDQN-priorMem-dueling" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

scratch='$SCRATCHDIR'
sed "12 c\cp /storage/brno2/home/xbucha02/test_004/Acrobot-v1/DDQN-priorMem-dueling/* $scratch" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

sed "16 c\name='Acrobot-v1-2500-DDQN-priorMem-dueling'" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

qsub job.sh
qsub job.sh
qsub job.sh
qsub job.sh
qsub job.sh
##############################################################################
sed "2 c\#PBS -N Acrobot-v1-2500-DQN+TN-basicMem" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

scratch='$SCRATCHDIR'
sed "12 c\cp /storage/brno2/home/xbucha02/test_004/Acrobot-v1/DQN+TN-basicMem/* $scratch" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

sed "16 c\name='Acrobot-v1-2500-DQN+TN-basicMem'" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

qsub job.sh
qsub job.sh
qsub job.sh
qsub job.sh
qsub job.sh
##############################################################################
sed "2 c\#PBS -N Acrobot-v1-2500-DQN+TN-basicMem-dueling" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

scratch='$SCRATCHDIR'
sed "12 c\cp /storage/brno2/home/xbucha02/test_004/Acrobot-v1/DQN+TN-basicMem-dueling/* $scratch" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

sed "16 c\name='Acrobot-v1-2500-DQN+TN-basicMem-dueling'" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

qsub job.sh
qsub job.sh
qsub job.sh
qsub job.sh
qsub job.sh
##############################################################################
sed "2 c\#PBS -N Acrobot-v1-2500-DQN+TN-priorMem" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

scratch='$SCRATCHDIR'
sed "12 c\cp /storage/brno2/home/xbucha02/test_004/Acrobot-v1/DQN+TN-priorMem/* $scratch" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

sed "16 c\name='Acrobot-v1-2500-DQN+TN-priorMem'" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

qsub job.sh
qsub job.sh
qsub job.sh
qsub job.sh
qsub job.sh
##############################################################################
sed "2 c\#PBS -N Acrobot-v1-2500-DQN+TN-priorMem-dueling" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

scratch='$SCRATCHDIR'
sed "12 c\cp /storage/brno2/home/xbucha02/test_004/Acrobot-v1/DQN+TN-priorMem-dueling/* $scratch" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

sed "16 c\name='Acrobot-v1-2500-DQN+TN-priorMem-dueling'" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

qsub job.sh
qsub job.sh
qsub job.sh
qsub job.sh
qsub job.sh
##############################################################################
sed "2 c\#PBS -N Acrobot-v1-2500-DQN-basicMem" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

scratch='$SCRATCHDIR'
sed "12 c\cp /storage/brno2/home/xbucha02/test_004/Acrobot-v1/DQN-basicMem/* $scratch" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

sed "16 c\name='Acrobot-v1-2500-DQN-basicMem'" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

qsub job.sh
qsub job.sh
qsub job.sh
qsub job.sh
qsub job.sh
##############################################################################
sed "2 c\#PBS -N Acrobot-v1-2500-DQN-basicMem-dueling" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

scratch='$SCRATCHDIR'
sed "12 c\cp /storage/brno2/home/xbucha02/test_004/Acrobot-v1/DQN-basicMem-dueling/* $scratch" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

sed "16 c\name='Acrobot-v1-2500-DQN-basicMem-dueling'" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

qsub job.sh
qsub job.sh
qsub job.sh
qsub job.sh
qsub job.sh
##############################################################################
##############################################################################
sed "2 c\#PBS -N Acrobot-v1-2500-DQN-priorMem" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

scratch='$SCRATCHDIR'
sed "12 c\cp /storage/brno2/home/xbucha02/test_004/Acrobot-v1/DQN-priorMem/* $scratch" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

sed "16 c\name='Acrobot-v1-2500-DQN-priorMem'" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

qsub job.sh
qsub job.sh
qsub job.sh
qsub job.sh
qsub job.sh
##############################################################################
sed "2 c\#PBS -N Acrobot-v1-2500-DQN-priorMem-dueling" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

scratch='$SCRATCHDIR'
sed "12 c\cp /storage/brno2/home/xbucha02/test_004/Acrobot-v1/DQN-priorMem-dueling/* $scratch" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

sed "16 c\name='Acrobot-v1-2500-DQN-priorMem-dueling'" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

qsub job.sh
qsub job.sh
qsub job.sh
qsub job.sh
qsub job.sh
##############################################################################