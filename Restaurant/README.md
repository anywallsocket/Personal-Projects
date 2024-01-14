### AnyLogic

# Sloppy Joe's

This simulation exhibits a minimal simplistic design for testing purposes related to a large sit-down style restaurant.

## Introduction

The goal here is to examine the differences between robot and human servers.
This is done by making agents with slight variations.
Here we make the robots slightly slower than the human servers.
Of course this means they will not preform as well in terms of serving as many customers and reducing wait times, but it is a fair approximation to make, and allows us to examine how they interact with human servers regardless. For example, it is entirely possibly that having many fast moving agents would cause more disruptions -- like the internal friction of a hot gas. And that having some fast and some slow actually provides a more efficient dynamic. Therefore, despite some of the obvious conclusions involving faster agents being able to serve more guests (which is true across all our restaurants), we nevertheless are interested in examining the robot-server relationship admid various environments and parameterizations.

The basic layout of this restaurant is shown below.

<img src="images/label.png" alt="" width="500">

One can imagine having the option to close or open the middle cross to enable a more or less exclusive experience, and so we can explore how this change in layout affects the overall dynamic.

An example animation of runtime simulation is shown below.

<img src="images/gif.gif" alt="" width="250">

## Logic

Using AnyLogic software, we're able to model all the relevant dynamics with flow-chart style connections. The basic elements are agents, movements, waiting delays, random choices, queues, and function calls.

### Customer Logic

Below we see an example logic flow of customers.

<img src="images/c1_logic.png" alt="" width="500">


A) 
- Customers enter the scene and join the outer queue.
  
B)
- This simulates the common situation where the restaurant might already be well-occupied.
- Notice that they can leave the queue if they have been waiting too long.
  
C)
- Once inside, they meet with the hostess, who can only interact with one agent at a time.
- This means that the customers sometimes have to wait even after they enter.
  
D)
- Now the customer is free to pick a table.
- These seeming complex connections are merely nested random choices.

Now that our customers have chosen a seat, we can model the logic of the dining experience.

<img src="images/c2_logic.png" alt="" width="550">

E)
- The customers wait of a minute and decide on S (human server) or R (robot server). These probabilities are up to the simulatee.
- Once they have chosen, they must wait for an available server.
- If none are available, they ask themselves whether or not they are mad.
- This is controlled by a patience parameter, which is reduced everytime they ask for a server, yet none are available. After this value reaches 0, they are officially mad and will leave the resaurant.
  
F)
- Upon succesfully calling a server, they free them from their idle state, and are sent over to the customer's location.
- Now they order, after which the server returns to the kitchen and they enter a 'food queue'. This ensures that those who order first, recieve their food first -- which may not be entirely realistic, yet the food queue allows us to track how many customers are waiting on food as one of our key metrics.
  
G)
- Once the server has obtained their food, they release the customer from a hold, and allow them to eat.
- After they eat, they decide based on their hunger level whether or not they want to order again. This hunger level is another parameter from 1-3, meaning each customer will eat 1 to 3 means within a random uniform distribution.
- After this cycle completes, assuming they don't get mad, they simply leave happy.

Therefore, we track happy and mad customers. Note as well that there are timing blocks before and after this logic sequence, allowing us to track how long each customer spends in the restaurant -- calculated from the moment they sit down, to the moment they stand up.

### Server Logic

This logic applies to both human and robot servers for sake of simplicity. Certainly one can add extra duities for humans and robot servers alike, but with this given set-up we have a nice foundation for modeling the basic dynamics we're interested.

<img src="images/server_logic.png" alt="" width="400">

A)
- The servers wait at their designated location, until called upon. Althoguh it appears they can leave, in the given implementation, they cannot, and our obliged to work the full hour of simulation run-time.
- Once a customer calls them, they break free of waiting, and check if indeed a customer is available to be served (this avoids possible NULL exceptions of high-frequency events).
  
B)
- The server goes to the customer, collects their order, returns to the kitchen, and waits for a service queue.
- The kitchen service queue merely simulates the server waiting for the kitchen to provide the order.
  
C)
- Once recieved, the servers bring the order to the customer.
- Finally, they return to their designated waiting location, and remain idle until another customer demands their attention.

One drawback here is that customers do not necessarily recieve the same server for their orders. However, this is arguably a more efficient design (although less friendly), because any server available to serve will do so.

## Evaluation

### Metrics

As noted, time is kept track of for how long customers spend inside the restaurant, as exemplified below by a snap-shot of a running histogram.

<img src="images/time.png" alt="" width="300">

The units here are seconds, and so the mean here translates to ~32 minutes, which is somewhat brief for a full sit-down experience, yet we were more interested in modeling as many guests as possible, within an hour, rather than providing a true-to-life simulation.

Additionally, we keep track of 3 dynamics arrays:

<img src="images/metrics.png" alt="" width="250">

A)
- The food queue tracks how many customers are waiting on their food.
- They enter the queue after placing an order, and leave it after recieving the order.
  
B)
- Hungry customers tracks how many customers remain unsatisfied.
- They enter the simulation with a certain hunger level, as explained previously.
- After they eat, their hunger drops, and only after this reaches zero are they removed from the HC queue.
- This allows us to track the total number of customers not leaving the restaurant, and is a broader set than those simply waiting on their food.
  
C)
- Available servers is just that, how many servers are available.
- They begin available, and are unavailable when serving a customer.
- This could be because they are taking the order, moving, or waiting on the kitchen.

### Results

Results for the two layouts, human robot ratios, and arrival rate times can be seen in my previous assignment submission -- note however that in that assignment, times were boosted 60x real-time, in order to maximize the hour-long simulation cap, yet here simulation time = real time.

To summarize, it was found that each layout has an optimal number of arrival rates for a given number of servers -- as should be expected. This optimal state was found to be highly sensitive to robot human ratios, despite their slight speed differences, in favor of maximizing humans and minimizing robots (note as well that only one robot can enter each table node at a time, simulating a saftey procaution, such that they do not interfere with eachother).

Most surprisingly, in this updated model, it was found that human-robot combinations sometimes outperformed pure humans on our closed layout -- please see the main results for these details and more.

----------------------

#### Simulated Resturant -- The Efficacy of Robotic Servers

Our resturant simulates a typical experience:
* Customers arrive in the area
* They wait in an external queue
* Then they enter and are serviced by a hostess
* They pick a table
* A server then comes to take their order
* The server returns to the back where the order is placed
* They then bring food or whatever out to the customer
* After eating, the customer decides if they want more
* This repeats utill they are satisfied
* Then they leave

When customers enter they do so with a certain 'hunger level', which ranges from 1-3, indicating how many meals they get, and essentially how long they stay. While waiting for their food, they are part of the Hungry Customers (HC) list, and after they have spoken to an Available Server (AS), they are entered into the 'Food Queue' (FQ). This is designed such that the first customer that sits gets server first, as per the typical FIFO ordering. The FQ is tracked over time, and has a limit of 30 customers -- however, the upper limit is actually the number of AS which starts out at 20. This is because if no server is available, they do not enter the FQ, they instead continue to wait, and their 'patience level' drops. Their patience ranges from 5-10 and once it hits zero they will leave mad, rather than happy. If customers are eating, or if they have left, they are removed from the HC list.

Total customers served during the simulation is the number of happy + the number of mad. Ideally we are trying to minimize the angry customers, but these parameters are for testing purposes only, and are only intuitively estimated. What we're really interested in is the dynamics. Given how the customers and servers interact with eachother and the environment, many dynamics can arise, and we are interested in how these dynamics behave on a statistical level. For example, too many customers per server will bottle-neck the resturant flow. The parameters are adjusted therefore to be reasonably accurate yes, but as well such they control the flow of the resturant. What we're looking for in the dynamics is a steady-state, which should occur with reasonable flexibility after some intitial period of time. The steady state can then be optimized, via the geometry of the environment, the internal parameterization, or the logic of the interactions.

#### A) & B)
In an effort to understand this optimization, we examine the resturant's dynamical flow under 3 main adjustments:

* How the geometry of the resturant is designed
* * *closed* or *open*
* The customer arival rate
* * 100/h, 200/h, or exp(0.06)/h
* What kind of servers we are simulating
* * robots, servers (humans), or both

Therefore we test 2 geometries x 3 arrival rates x 3 server options, which makes for 18 possibly interesting combinations.

First, we look at 100 customers per hour*

![](images/1.png)

Next we look at 200 customers per hour

![](images/2.png)

Finally we look at exp(0.06) customers per hour

![](images/3.png)

Three obvious trends emerge. 

Most notebaly, open geometries generally do better than closed -- this is because servers and robots have extra space to move around. 

Additionally, servers in general out-preform the robots, with them working together as a compromise in preformance. This is because of the design: the robots are made slightly slower than the human servers, and, importantly, only one of them can enter each of the 4 table nodes at a time (this is to simulate the real world concern about too many robots getting in customer's, servers, and eachother's way). 

Finally, 'steady' states are seen in addition to 'oscillatory' states. There is the *good* steady state, where all values hover around a mean -- see servers and robots in the first plot. *Bad* steady states are when the FQ remains around its 20 server limit, and the servers remain mostly unavailable. Oscillatory states are also seen, and are most interesting. In general, both the FQ and the HC values run opposite to AS values, due to the underlying logic, and it is this attraction-repulsion dynamic which leads to the oscillations -- another cause could be the inter-arrival time of the final test (exp(0.06)/h). 

What we desire is for the HC list to not rise, because that is the most direct evidence for the servers failing to keep up with demand. We see this happening most often in closed, robot situations, but these are not necessarily failure cases, only the least efficient. 

To get a better grip, we show direct histograms of customer's over-all wait time*, along with the tally of happy and mad customers. They are ordered just like the plots above, and labeled 1,2,3, for the arrival times, S, R, RS for server, robot, robot and server, and O, C, for open and closed geometries.


<table>
  <tr>
    <td><img src="images/1_S_O.png" width="200" /><br>1_S_O</td>
    <td><img src="images/1_R_O.png" width="200" /><br>1_R_O</td>
    <td><img src="images/1_RS_O.png" width="200" /><br>1_RS_O</td>
  </tr>
  <tr>
    <td><img src="images/1_S_C.png" width="200" /><br>1_S_C</td>
    <td><img src="images/1_R_C.png" width="200" /><br>1_R_C</td>
    <td><img src="images/1_RS_C.png" width="200" /><br>1_RS_C</td>
  </tr>
  <tr>
    <td><img src="images/2_S_O.png" width="200" /><br>2_S_O</td>
    <td><img src="images/2_R_O.png" width="200" /><br>2_R_O</td>
    <td><img src="images/2_RS_O.png" width="200" /><br>2_RS_O</td>
  </tr>
  <tr>
    <td><img src="images/2_S_C.png" width="200" /><br>2_S_C</td>
    <td><img src="images/2_R_C.png" width="200" /><br>2_R_C</td>
    <td><img src="images/2_RS_C.png" width="200" /><br>2_RS_C</td>
  </tr>
  <tr>
    <td><img src="images/3_S_O.png" width="200" /><br>3_S_O</td>
    <td><img src="images/3_R_O.png" width="200" /><br>3_R_O</td>
    <td><img src="images/3_RS_O.png" width="200" /><br>3_RS_O</td>
  </tr>
  <tr>
    <td><img src="images/3_S_C.png" width="200" /><br>3_S_C</td>
    <td><img src="images/3_R_C.png" width="200" /><br>3_R_C</td>
    <td><img src="images/3_RS_C.png" width="200" /><br>3_RS_C</td>
  </tr>
</table>


#### C) 

The robot's main issue is that they aren't as nimble as human servers. However, if alotted their own lanes, they could more than make up for this with high speeds. They could also be designed to be smaller than human servers, and thus not get in the way as much. However, as they are modeled now, they can only help the servers, by providing additional support, and cannot replace them.


---

*please note that while an effort was made to maximize simulation time, such that seconds=minutes and minutes=hours, certain internal dynamics (such as pedestrian movement speed) do not allow for a complete 60x simulation time to real time. This means that the time scale is somewhat arbitrary.




