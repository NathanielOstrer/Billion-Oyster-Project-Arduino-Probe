# Billion Oyster Project Arduino Probe Code

How to setup your probe:

Connect the tentacle shield to your arduino. On the tentacle shield make sure the jumpers are set to serial and channels 0 - 3

Plug the pH stamp into the first stamp slot on the tentacle shield, the
DO stamp onto the second stamp slot on your shield and the EC stamp
to the third stamp slot on your shield.

Plug the probes' wires into their respective stamps/slots.

Now plug the arduino into your computer.
Download the sensor-calibration sketch to the arduino to calibrate sensors.

Open the serial monitor. Enter the command "0" to switch to the pH probe.

Enter "t,##.#" where ## is the temperature of the solution in degrees celsius (decimal allowed). Temperature is meant to be measured by a separate device.

Enter "Cal,clear" to clear calibration information

Wash off the sensor. Then pour some of the 7.0 pH solution in a container and mix the probe around in the solution for 1-2 minutes. Then enter "Cal,mid,7.00" to calibrate the device for mid range pHs. DO NOT put the solution you poured out back in the solution container.

(Remeasure/reset temperature as needed).

Wash off the sensor. Then pour some of the 4.0 pH solution in a container and mix the probe around in the solution for 1-2 minutes. Then enter "Cal,low,4.00" to calibrate the device for low range pHs. DO NOT put the solution you poured out back in the solution container.

(Remeasure/reset temperature as needed).

Wash off the sensor. Then pour some of the 10.0 pH solution in a container and mix the probe around in the solution for 1-2 minutes. Then enter "Cal,high,10.00" to calibrate the device for high range pHs.DO NOT put the solution you poured out back in the solution container.

Now the pH sensor is calibrated!

Time to calibrate the dissolved oxygen sensor.

Enter "1" to switch to the DO sensor.

Enter "Cal,clear" to clear any calibration data in the sensor.
Open the sensor to the general atmosphere.
Now enter "Cal" to calibrate the sensor to atmospheric oxygen levels.
Optionally put the probe in the 0 DO solution, mix around for 1-2 minutes and then enter "Cal,0" to set the 0 dissolved oxygen level.

Done!

Now to calibrate the conductivity sensor. 
Enter "2" to switch to the EC sensor.

Enter "T,##.#" to set the temperature of the solution.

Enter "Cal,clear" to reset the calibration. 

Make sure the probe is dry and enter "Cal,dry"

Clean off the probe, put in the lower mS solution and enter "Cal,low,12880"

Clean off the probe, put it in the higher mS solution and enter "Cal,high,80000"


And now that's done!