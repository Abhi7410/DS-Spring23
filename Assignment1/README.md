<h1>Assignment 1</h1>



<h3>Question 1</h3>



1. **Performance Scaling** : Giving input as N = 100, M = 100, K = 1000 , the distribution looks like :

    ```tex
    1 Process total time taken: 0.253618
    2 Process total time taken: 0.200653
    3 Process total time taken: 0.122752
    4 Process total time taken: 0.124204
    5 Process total time taken: 0.10728
    6 Process total time taken: 0.0892409
    7 Process total time taken: 0.0862814
    8 Process total time taken: 0.0752455
    9 Process total time taken: 0.113145
    10 Process total time taken: 0.112353
    11 Process total time taken: 0.113585
    12 Process total time taken: 0.0964231
    ```

   

2. **Time Complexity** : The complete grid of size N*M is divided among all processes and every process handles almost equal amount of coordinates except if NM is not divisible by number of processes. For every coordinate, K iterations are done to check the absoulte value of complex number is smaller than 2 or not.

   * $$
     O(\frac{NMK}{(\text{numProcess)}})
     $$

     

3. **Space Complexity** : 1. Local grid for every process , 2. This is used for Gathering (recvcount and Displs array) 3. It is the final matrix which store the all local grids.

   * $$
     O(\frac{NM}{\text{numProcess}}) + O(\text{numProcess}) + O(\text{NM})
     $$

     

4. **Message Complexity**

   * MPI_Gatherv : Divided local grids gather to make complete output grid
     $$
     O(NM)
     $$

     $$
     
     $$

<h3>Question 2</h3>



1. **Performance Scaling ** : N = 300, M = 600 , K = 1000, T = 1000

   ```tex
   1 Process total time taken: 1.62928
   2 Process total time taken: 1.99421
   3 Process total time taken: 1.16433
   4 Process total time taken: 0.945283
   5 Process total time taken: 0.715125
   6 Process total time taken: 0.796402
   7 Process total time taken: 0.653437
   8 Process total time taken: 0.694045
   9 Process total time taken: 0.855539
   10 Process total time taken: 0.90006
   11 Process total time taken: 0.862893
   12 Process total time taken: 0.831708
   ```

   

2. **Time Complexity** : For every time stamps, Iterate through all particles distributed to that particulalr process . First handle the boundary cases , then send particles to different process if they are going out of boundary of local grid. Handle the movement of the particles and then handle the colliison.

   * $$
     O(\text{TimeSteps*numParticles})
     $$

     

3. **Space Complexity** : For every process , there is particles vector which can have atmost all particles. In the collision handling , there are 3 maps, one handles all particles indexess which lie on same coordinates, second maps the real id of the particles to the process particles vector index and third handles no multiple collisions at same index. They all have numParticles space and other factor is for Gathering purpose.

   * $$
     O(\text{numParticles}) + O(\text{numProcess}) \approx O(\text{numParticles})
     $$

     

4. **Message Complexity** : Sending & Receiving particles among one above and below processes rank, Using Gather for all particles.

   * $$
     O(c*\text{numProcess}) + O(\text{numParticles})
     $$

     

<h3>Question 3</h3>



1. **Performance Scaling **: N = 500, with keys and frequency to be random 

   ```tex
   1 Process total time taken: 0.0197499
   2 Process total time taken: 0.00875252
   3 Process total time taken: 0.00636879
   4 Process total time taken: 0.0065633
   5 Process total time taken: 0.00950052
   6 Process total time taken: 0.0103781
   7 Process total time taken: 0.0272465
   8 Process total time taken: 0.0131094
   9 Process total time taken: 0.0278577
   10 Process total time taken: 0.0242353
   11 Process total time taken: 0.0712143
   12 Process total time taken: 0.0323755
   ```

   

2. **Time Complexity** : The total time complexity will be same for merge sort , parallellization will reduce the total processing time but does not change aymptotic time complexity. The other factor is finding OBST using dynamic programming by diagonal decomposition. There is DFS for finding the parent root of the node also.

   * $$
     O(\text{numNodes}*\log(\text{numNodes})) + O(\text{numNodes}^3)
     $$

     

3. **Space Complexity** : The dynamic programming approach consists of cost matrix of size numNodes*numNodes , except this in merge sort there are arrays of size numNodes/numProcs and complete array to store split arrays but they are smaller than this factor.

   * $$
     O(\text{numNodes}^2)
     $$

     

4. **Message Complexity **: First factor is for Gather and Scatter, second is for AllReduce.

   * $$
     O(\text{numNodes}) + O(\text{numNodes*(numNodes-1)})
     $$

     