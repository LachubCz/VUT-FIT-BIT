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
##### python3 main.py -env env -eps eps -mode mode [-alg alg] [-mem mem] [-net net] [-pu pu] [-mdl mdl] [-init] [-num_of_frames num_of_frames] [-save_f save_f] [-update_f update_f] [-vids]
###### Parametry:

    -env env           |   
    -eps eps           |   
    -mode mode         |   
    -alg alg           |   
    -mem mem           |   
    -net net           |   
    -pu pu             |   
    -mdl mdl           |   
    -init              |   
    -num_of_frames num_of_frames    |   
    -save_f save_f     |   
    -update_f update_f |   
    -vids              |   

Příklady použítí
-----------------
    python3 main.py -mode train -env CartPole-v0 -eps 5000 -alg DDQN -net dueling -mem prioritized -pu CPU -init -save_f 25
    python3 main.py -mode test -env Acrobot-v1 -eps 100 -mdl trained_models/basic_net/Acrobot-v1.h5
    python3 main.py -mode render -env SpaceInvaders-ram-v0 -eps 200 -mdl trained_models/basic_net/SpaceInvaders-ram-v0.h5 -num_of_frames 4 -vids

Knihovny
-----------------
**Standardní knihovny:**
* os
* time
* re
* argparse
* sys
* math
* collections
* random
* warnings


**Third party knihovny:**
* keras
* tensorflow
* gym
* gym_2048
* numpy
* tqdm
* scipy
* sklearn
* matplotlib

Soubory:
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

###### Vytvořil: Petr Buchal
