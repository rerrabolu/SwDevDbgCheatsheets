import glob
import os

#
# Iterate over multiple times if you want to collect the
# sequence of proc launch and their queue creations/deletions
#

# for iter in range(3600):
    #print('###################################################')
    #print('Iteration: ' + str(iter))
for pidDir in glob.glob('/sys/devices/virtual/kfd/kfd/proc/*'):
    pidVal = os.path.basename(pidDir)
    #print('Proc Id: ' + str(pidVal))
    for qidDir in glob.glob(pidDir + '/queues/*'):
        qidVal = os.path.basename(qidDir)
        #print('  Queue Id: ' + str(qidVal))
        with open(os.path.join(os.getcwd(), qidDir + '/gpuid'), 'r') as f:
            gpuIdVal = f.read()
            print('Proc Id: ' + str(pidVal) + ', ', end='')
            print('GPU Id: ' + str(gpuIdVal) + ', ', end='')
            print('Queue Id: ' + str(qidVal))



