# Clipping-Curves
This program clips curves in a circle window<br />
First, we create a circle using iterative parametric equation. This circle will act as our window.<br />
Then, we draw a curve using bezier and we use point-clipping algorithm to clip the curve.<br />
If the pixel of the curve is inside our circle window then it will be colored red.<br />
If it is outside our window, it'll be colored blue.<br />
<br />
To run the program, we give the following inputs: <br />
	1- 2 left button clicks to draw the circle, the first click is the center of the circle and the second click will be a point on the circle.<br />
	2- Another 4 points to draw the curve. The first and fourth points are on the curve and the 2nd and 3rd points guide the curve. These 4 points will be reflected to draw another curve.
