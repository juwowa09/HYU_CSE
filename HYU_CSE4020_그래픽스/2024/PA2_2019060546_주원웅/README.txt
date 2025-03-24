1. Added Global Variables
	cowpos = np.zeros((4,4)): A 4x4 matrix that stores the current position and orientation of the cow.
	animStartTime = 0: Stores the time when the animation starts.
	pick = -1: An index that indicates the currently selected position.
	pick_pos = []: A list that stores the selected positions.
	currentPos = []: Stores the current position of the cow.

These variables manage the state related to the animation and are used to update the cow's position based on user interaction. Global handling and additional initialization tasks have been performed for functions that require these variables.

2.Added Functions for Updating Cow's Position and Orientation
	- spline function generates the animation path using a spline function to ensure smooth movement of the cow.
	- cowlocate function updates the cow's position and applies rotation to the cow during movement.

3.Added Animation Logic
	- display function updates the cow's position and orientation during the animation. It updates the cow's position based on the current pick state, calculates the path using the spline function, and moves the cow accordingly.
	- When the animation ends, it resets the pick variable and clears the pick_pos list.

4.Added Mouse Input Handling
	- onMouseButton function handles mouse button events. When the left button is pressed, it changes the drag state and selects a position.
	- If cursorOnCowBoundingBox is true, it selects a position, and when 6 positions are selected, it starts the animation.
	- In the onMouseDrag function, Vertical Dragging has been added. In vertical drag mode, it updates the cow's y-coordinate based on the mouse position.