#CartPole-v1
sed "2 c\#PBS -N new-CartPole-v1-2500-obnova-none" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

scratch='$SCRATCHDIR'
sed "12 c\cp /storage/brno2/home/xbucha02/tests_for_metacentrum/CartPole-v1/obnova-none/* $scratch" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

sed "16 c\name='new-CartPole-v1-2500-obnova-none'" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

qsub job.sh
qsub job.sh
qsub job.sh
qsub job.sh
qsub job.sh
##############################################################################
sed "2 c\#PBS -N new-CartPole-v1-2500-obnova-ki" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

scratch='$SCRATCHDIR'
sed "12 c\cp /storage/brno2/home/xbucha02/tests_for_metacentrum/CartPole-v1/obnova-ki/* $scratch" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

sed "16 c\name='new-CartPole-v1-2500-obnova-ki'" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

qsub job.sh
qsub job.sh
qsub job.sh
qsub job.sh
qsub job.sh
##############################################################################
sed "2 c\#PBS -N new-CartPole-v1-2500-obnova-tn" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

scratch='$SCRATCHDIR'
sed "12 c\cp /storage/brno2/home/xbucha02/tests_for_metacentrum/CartPole-v1/obnova-tn/* $scratch" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

sed "16 c\name='new-CartPole-v1-2500-obnova-tn'" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

qsub job.sh
qsub job.sh
qsub job.sh
qsub job.sh
qsub job.sh
##############################################################################
sed "2 c\#PBS -N new-CartPole-v1-2500-obnova-tn-ki" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

scratch='$SCRATCHDIR'
sed "12 c\cp /storage/brno2/home/xbucha02/tests_for_metacentrum/CartPole-v1/obnova-tn-ki/* $scratch" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

sed "16 c\name='new-CartPole-v1-2500-obnova-tn-ki'" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

qsub job.sh
qsub job.sh
qsub job.sh
qsub job.sh
qsub job.sh
##############################################################################
sed "2 c\#PBS -N new-CartPole-v1-2500-neobnova-none" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

scratch='$SCRATCHDIR'
sed "12 c\cp /storage/brno2/home/xbucha02/tests_for_metacentrum/CartPole-v1/neobnova-none/* $scratch" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

sed "16 c\name='new-CartPole-v1-2500-neobnova-none'" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

qsub job.sh
qsub job.sh
qsub job.sh
qsub job.sh
qsub job.sh
##############################################################################
sed "2 c\#PBS -N new-CartPole-v1-2500-neobnova-ki" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

scratch='$SCRATCHDIR'
sed "12 c\cp /storage/brno2/home/xbucha02/tests_for_metacentrum/CartPole-v1/neobnova-ki/* $scratch" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

sed "16 c\name='new-CartPole-v1-2500-neobnova-ki'" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

qsub job.sh
qsub job.sh
qsub job.sh
qsub job.sh
qsub job.sh
##############################################################################
sed "2 c\#PBS -N new-CartPole-v1-2500-neobnova-tn" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

scratch='$SCRATCHDIR'
sed "12 c\cp /storage/brno2/home/xbucha02/tests_for_metacentrum/CartPole-v1/neobnova-tn/* $scratch" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

sed "16 c\name='new-CartPole-v1-2500-neobnova-tn'" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

qsub job.sh
qsub job.sh
qsub job.sh
qsub job.sh
qsub job.sh
##############################################################################
sed "2 c\#PBS -N new-CartPole-v1-2500-neobnova-tn-ki" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

scratch='$SCRATCHDIR'
sed "12 c\cp /storage/brno2/home/xbucha02/tests_for_metacentrum/CartPole-v1/neobnova-tn-ki/* $scratch" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

sed "16 c\name='new-CartPole-v1-2500-neobnova-tn-ki'" job.sh > job1.sh
rm job.sh
mv job1.sh job.sh
chmod -R o=rX,g= job.sh

qsub job.sh
qsub job.sh
qsub job.sh
qsub job.sh
qsub job.sh
##############################################################################