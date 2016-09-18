# Overwatch Aim Assistance
A program written in C++ that helps a player aim at a target in Overwatch (includes trigger bot functionality).

Demo: https://www.youtube.com/watch?v=0Su_ytp4-7I

# How it Works
The program uses Windows GDI to capture a screenshot. The screenshot is scanned in an attempt to find a player's health bar. 
Upon finding a health bar, the program will scan for the player's border from which it can obtain the coordinates of the player's head or body.

The program can then either move the cursor smoothly to the desired location so as to simulate human like movements or just snap directly to the target location. Trigger bot functionality was also implemented, however just like all other external colour recognition software, the crosshair must cross the target's border in order to fire.

![alt tag](https://raw.githubusercontent.com/jpxue/Overwatch-Aim-Assist/master/how-it-works.png)

# Limitations
- Shield/Armor bars were not included in the function that calculates the width of the health bar. Should be easy to implement.
- Program only scans for red borders which can occasionally change. Can use opencv's canny function to overcome this or scan for different border colours.
- Mouse movements are based on a constant (0.116) which is not perfectly accurate with varying distances.
- Trigger bot needs to be optimised to exclude clustered pixels which are not part of a border. This is problematic in maps with a lot of red such as route 66.
- No threading.
