# ms-2dpnts
Clustering 2D Points using Mean Shift

## Usage
``` make sim1 ``` to run the simulation

## Script Format
* ```inp```
Used to generate normally distributed points
Each line is formatted as $N, \mu_x, \mu_y, \sigma$ as ```%u %lf %lf %lf```
The output is written to file ```oup```
* ```script```
Used to configure mean shift simulator
Set kernel, bandwidth, and other parameters

## Notes
```R``` is automatically called to plot the results.