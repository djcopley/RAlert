# RAlert

After receiving a music noise complaint in my dorm, I set out to create a solution that would reduce the risk of getting another infraction. I lived on the third floor, with two outdoor stairwells being the only access points to my dorm. I created wirelessly connected, passive infrared detecting nodes and set them up inconspicuously in each stairwell. A "base station" attached via serial port to our music-playing laptop listened for regular updates from the stairwell. Upon detection, the base station would fire a music pause event and automatically alert us to the possible presence of an RA.

Bill of Materials:

1. Arduino microcontrollers
2. NRF24 wireless transceivers
3. Passive infrared sensors
