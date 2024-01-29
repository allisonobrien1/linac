#import the packages
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
from scipy.optimize import curve_fit

#define buildup-tail function
def PDD(d, u, n, c):
    return c*np.exp(-u*d)*d/(np.sqrt(d**2+n))

#define function to calculate total dose by distance in object
def tdd(x, y, sim_data, dat, number, energy): 
    for i in x:
        y.append(sim_data.query(f"z == {i} and x < 17 and x > 12 and y < 17 and y > 12")['dose\r'].sum())
    max_dose = np.max(y)
    popt, pcov = curve_fit(PDD, x, y, p0=[0.0515,0.208,max_dose])
    dat.append([number, *popt, energy, max_dose])
    plt.plot(x,y/max_dose)
    plt.xlabel("Distance (cm)")
    plt.ylabel("Dose (%)")

    
#setup the physical world
xdata = np.linspace(0, 29, 30)
data = []

#load the data
p1 = pd.read_csv('inhomog/totDose_100000000.txt', sep=' ', lineterminator='\n')
y1 = []
tdd(xdata, y1, p1, data, 100000000, 6)
plt.show()