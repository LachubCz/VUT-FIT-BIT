#Acrobot-v1
sed "2 c\#PBS -N Acrobot-v1-2500-newObnova-adam" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

scratch='$SCRATCHDIR'
sed "12 c\cp /storage/brno2/home/xbucha02/tests_002/Acrobot-v1/newObnova-adam/* $scratch" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

sed "16 c\name='Acrobot-v1-2500-newObnova-adam'" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

qsub job.sh
qsub job.sh
qsub job.sh
qsub job.sh
qsub job.sh
##############################################################################
sed "2 c\#PBS -N Acrobot-v1-2500-newObnova-adam-nejvyssiEps" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

scratch='$SCRATCHDIR'
sed "12 c\cp /storage/brno2/home/xbucha02/tests_002/Acrobot-v1/newObnova-adam-nejvyssiEps/* $scratch" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

sed "16 c\name='Acrobot-v1-2500-newObnova-adam-nejvyssiEps'" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

qsub job.sh
qsub job.sh
qsub job.sh
qsub job.sh
qsub job.sh
##############################################################################
sed "2 c\#PBS -N Acrobot-v1-2500-newObnova-adam-vyssiEps" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

scratch='$SCRATCHDIR'
sed "12 c\cp /storage/brno2/home/xbucha02/tests_002/Acrobot-v1/newObnova-adam-vyssiEps/* $scratch" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

sed "16 c\name='Acrobot-v1-2500-newObnova-adam-vyssiEps'" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

qsub job.sh
qsub job.sh
qsub job.sh
qsub job.sh
qsub job.sh
##############################################################################
sed "2 c\#PBS -N Acrobot-v1-2500-newObnova-RMSprop" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

scratch='$SCRATCHDIR'
sed "12 c\cp /storage/brno2/home/xbucha02/tests_002/Acrobot-v1/newObnova-RMSprop/* $scratch" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

sed "16 c\name='Acrobot-v1-2500-newObnova-RMSprop'" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

qsub job.sh
qsub job.sh
qsub job.sh
qsub job.sh
qsub job.sh
##############################################################################
sed "2 c\#PBS -N Acrobot-v1-2500-newObnova-RMSprop-nejvyssiEps" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

scratch='$SCRATCHDIR'
sed "12 c\cp /storage/brno2/home/xbucha02/tests_002/Acrobot-v1/newObnova-RMSprop-nejvyssiEps/* $scratch" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

sed "16 c\name='Acrobot-v1-2500-newObnova-RMSprop-nejvyssiEps'" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

qsub job.sh
qsub job.sh
qsub job.sh
qsub job.sh
qsub job.sh
##############################################################################
sed "2 c\#PBS -N Acrobot-v1-2500-newObnova-RMSprop-vyssiEps" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

scratch='$SCRATCHDIR'
sed "12 c\cp /storage/brno2/home/xbucha02/tests_002/Acrobot-v1/newObnova-RMSprop-vyssiEps/* $scratch" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

sed "16 c\name='Acrobot-v1-2500-newObnova-RMSprop-vyssiEps'" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

qsub job.sh
qsub job.sh
qsub job.sh
qsub job.sh
qsub job.sh