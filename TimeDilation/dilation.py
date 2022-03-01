#!/usr/bin/env python3

''' This code plots the total time dilation of a clock relative to a ground clock,
	as a function of their relative velocity and radii. 
	This total dilation is the product of kinetic and potential dilation.
	The ISS and a GPS satellite are plotted for reference.
	Additionally, the line of zero dilation (relative to ground clock),
	and the line of escape velocity are also plotted for analysis.
	It is seen that the ground-level escape velocity (purely kinetic) exactly matches
	the asymptote for zero dilation, establishing conversion to purely potential.
'''

import numpy as np
from numpy import sqrt, pi
import matplotlib.pyplot as plt
from matplotlib import cm

#FORMULAE
#Total time dilation is given as microseconds per day
def potential(r2): return sqrt(1-2*G*M/(r2*c**2))/sqrt(1-2*G*M/(r1*c**2))
def kinetic(v2): return sqrt(1-v2**2/c**2)/sqrt(1-v1**2/c**2)
def combined(r2, v2): return (spd*(potential(r2) * kinetic(v2)-1))*1E6

#CONSTANTS
c = 2.998E8 #speed of light (m/s)
G = 6.674E-11 #gravitational constant (m^3/kg/s^2)
M = 5.97E24 #Earth's mass (kg)
spd = 8.64E4 #seconds per day (s)
r1 = 6.371E6 #radius of Earth (m)
rGPS = r1 + 2.02E7 #orbit radius of GPS (m)
rISS = r1 + 4.289E5 #orbit radius of ISS (m)
v1 =  2*pi*r1/spd #tangental speed of clock at equator (m/s)
vGPS = 2*pi*rGPS/(spd/2) #GPS satellites orbit twice per day (m/s)
vISS = 7658 #ISS orbit velocity is an average (m/s)
res = 500 #plot resolution

#Radii and velocities minimized by our equitorial radius and velocity
Rrange = np.linspace(r1, 30*r1, res)
Vrange = np.linspace(v1, 30*v1, res)

#Make dilation surface
X, Y = np.meshgrid(Rrange, Vrange)
zs = np.array(combined(np.ravel(X), np.ravel(Y)))
Z = zs.reshape(X.shape)
colors = cm.jet(Z/np.amax(Z)) #linear function of range

#PLOTTING
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
fig.set_figheight(10)
fig.set_figwidth(10)

#Plot dilation surface
ax.plot_surface(X, Y, Z, facecolors=colors, alpha=0.5)

#Plotting zero dilation line (likely a better way)
zx = []; zy = []
for i in range(res):
	for j in range(res):
		if abs(Z[i][j]) < 0.1:
			zx.append(Vrange[i]); zy.append(Rrange[j])
ax.plot(zy, zx, [-100]*len(zy), color='orange', label='zero dilation')

#Escape velocity as function of altitude
escape = np.array([sqrt(2*G*M/r) for r in Rrange])
ax.plot(Rrange, escape, [-100]*res, color='purple', label='escape velocity')

#GPS and ISS
gps = np.round([rGPS, vGPS, combined(rGPS, vGPS)],1)
ax.plot([gps[0]], [gps[1]], [gps[2]], color='r', marker='o', ms=10)
ax.text(gps[0], gps[1], gps[2]*1.1, 'GPS', size=10)
iss = np.round([rISS, vISS, combined(rISS, vISS)],1)
ax.plot([iss[0]], [iss[1]], [iss[2]], color='b', marker='o', ms=10)
ax.text(iss[0], iss[1], iss[2]*1.1, 'ISS', size=10)

#Finish
ax.set_xlabel('Altitude (m)')
ax.set_ylabel('Velocity (m/s)')
ax.set_zlabel('Total time dilation per day ($\mu$s)')
plt.legend()
plt.show()

###anywallsocket@github.com
