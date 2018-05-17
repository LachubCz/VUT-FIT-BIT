DQN a její vylepšení
====================
Popis aplikace
-----------
Aplikace slouží k trénování zpětnovazebního agenta pomocí DQN a několika jeho vylepšení na prostředích z toolkitu Open AI Gym.

**Podporovaná prostředí:**

* CartPole-v0
* CartPole-v1
* MountainCar-v0
* Acrobot-v1
* 2048-v0
* SpaceInvaders-v0
* SpaceInvaders-ram-v0
* Breakout-v0
* Breakout-ram-v0
* BeamRider-v0
* BeamRider-ram-v0

Použití
-----
##### python main.py [-lr] [-dt] [-rf] [-d d] [-train] [-test] [-eval] [-train_test] [-s] [-lrm lrm] [-dtm dtm] [-rfm rfm] [-dic dic]
###### Parameters: 

    -lr         |   script will use Logistic Regression model (it's possible to run multiple modes simultaneously)
    -dt         |   script will use Decision Tree model (it's possible to run multiple modes simultaneously)
    -rf         |   script will use Random Forest model (it's possible to run multiple modes simultaneously)
    -d d        |   dataset name (location) to load
    -train      |   script will run in train mode, model will train on hole dataset, no output
    -test       |   script will run in test mode, model will process hole dataset and compare own predictions with correct labels, output is performace
    -eval       |   script will run in eval mode, model will predict labels for hole dataset and return them, if you want to use this mode, you have to load taugh models, output is array of predicted labels
    -train_test |   script will run in train_test mode, first half of dataset will be used as training data and second half as a testing data, output is performace
    -s          |   script will save models
    -lrm lrm    |   name of Logistic Regression model (location) to load
    -dtm dtm    |   name of Decision Tree model (location) to load
    -rfm rfm    |   name of Random Forest model (location) to load
    -dic dic    |   name of dictionary (location) to load

Examples
-----------------
    python main.py -d datasets/churn.all -lr -rf -dt -train_test -s
    python main.py -d datasets/churn_for_train.all -lr -train -s
    python main.py -d datasets/churn_for_test.all -lr -test -lrm models/log_regression.pkl -dic models/dictionary.pkl
    python main.py -d datasets/churn_for_eval.all -lr -dt -eval -lrm models/log_regression.pkl -dtm models/decision_tree.pkl -dic models/dictionary.pkl

Libraries
-----------------
**Standard libraries:**
* os
* time
* re
* argparse
* sys
* math
* collections
* random
* warnings


**Third party libraries:**
* keras
* tensorflow
* gym
* gym_2048
* numpy
* tqdm
* scipy
* sklearn
* matplotlib

Files:
---------------------------
**./:**

* agent.py
* main.py
* memory.py
* network.py
* playing.py
* README.md
* task.py
* visualization.py

**./trained_models/basic_net:**

* 2048-v0.h5
* Acrobot-v1.h5
* BeamRider-v0
* BeamRider-ram-v0
* Breakout-v0
* Breakout-ram-v0
* CartPole-v0.h5
* CartPole-v1.h5
* MountainCar-v0.h5
* SpaceInvaders-v0.h5
* SpaceInvaders-ram-v0.h5

**./trained_models/dueling_net:**

* Acrobot-v1.h5
* CartPole-v0.h5
* CartPole-v1.h5
* MountainCar-v0.h5

****

###### Created by: Petr Buchal
