### Part2

Here we explore the so-called 'Preditor-Prey' equations.

#### a), b)

For a thorough examination, see the associated jupyter notebook.

[Part2.ipynb](Part2.ipynb)

There we explore the basic 2-variabled coupled equations, its implications, and also look at 3-variabled Lorentz system. Note that this notebook relies on random numbers, and so will be different each run.

#### c), d)

Using AnyLogic, we can implement the same dynamics as above. The  Lotka-Volterra equations are achieved easily in AnyLogic, using similar logic as in Part1, however for more fun I've demonstrated this mechanic with the Lorentz equations instead.

<img src="../images/any2.png" width="300">

Here we have $x, y, z$ as $u, v, w$ respecively. These variables are encoded in 'Stacks' like before, but now we feed in parameter values $\rho, \beta, \sigma$ for the Lorentz constants $28, 10, \frac{8}{3}$ respectively.

Comparison to Python solutions can now be made easily.

<img src="../images/any3.png" width="500">

a) shows the Python interpretation, and b) the AnyLogic.

-------------

Unlike the equation in Part1, these equations are *non-linear*. This is because the variables rely on eachother -- expressed in terms like $xy$ in the Lotka–Volterra and Lorentz equations. This is important because basically all natural systems are non-linear to some extent -- it is only in mathematical ideaization that simple linear relations exist. 

This can be understood by the famous quote:

>*using a term like 'nonlinear science' is like referring to the bulk of zoology as the study of 'non-elephant animals' ~ Stanislaw Ulam*

Arguably even complex physical models like Quantum Mechanics are linear at their core -- making patent use of the principle of superposition, but we must remind ourselves this is still only a model. Any time a system interacts with itself, it gains non-linear quality. This opens up the door to very interesting behavior in dynamical systems, such as chaos, recursion, resonance, phase transitions, and many more interesting emergent phenomena.

#/END
