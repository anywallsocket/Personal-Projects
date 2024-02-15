### Here we explore the Fourier Transform
(as inspired by 3B1B: https://www.3blue1brown.com/lessons/fourier-transforms)

### As this is *interactive* one needs to run the notebook live, or see the colab: <br> 
https://colab.research.google.com/drive/1noGd81q0NLOZPOUWMhcoBK5iUUAHkOvY?usp=sharing

First we build a waveform, here it is done with two sine waves of two different frequencies

$f(t)=sin(2\pi tf_1)+sin(2\pi tf_2)$

Where $f_1$ and $f_2$ correspond to hz1 and hz2

Then we define a frequency range with which to calculate the $FT$, here Hz

$F(f)=\int{f(t)e^{i2\pi ft} dt}$

* note that the code taks the mean for visualization, whereas the true $FT$ does not

We can then extract the real and imaginary components form $FT$

* These correspond to the even and odd (cos and sin) eigenstates

[]('g2.gif')
