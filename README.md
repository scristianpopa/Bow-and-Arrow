Homework EGC
Popa Stefan Cristian
331CA

IMPLEMENTATION DETAILS

    Every mesh on screen is rendered using a single C++ object for every type
of on-screen object and moved to the desired position using a model matrix.
Every object has its own header and cpp file with its class implementation.
The general game logic is written in BowArrow.cpp, in the Update() method.

    Baloons and shurikens are spawned randomly. Red ballons have the highest
chance of appearing, then yellow, blue and finally pink. Shurikens can have
different sizes, within a certain limit, with bigger shurikens moving slower
than smaller ones.

    Each object also has a properties struct which keeps all the relevant
information for that type of object. This way, to render all the object of a
certain type we just have to iterate through the array of properties of that
object type.

    Every frame, we have to draw:

 - the bow: the string moves further back the more we charge the arrow. It can
move up and down and it always points to the mouse cursor. The string is
comprised of 2 lines, while the actual bow is a circle arc, with 2 smaller 
circle arcs at the edges for the recurve tips. We also always have an arrow
ready to be shot, unless we've just shot another one, in which case there is a
small cooldown until we can shoot again.

 - baloons: they move upward and they follow a sinusoidal wave on the Ox
axis. Each color has a different affect on the game and a different speed.
When a baloon is hit by an arrow, it will get smaller at firsts and when it
has deflated enough it will use a different mesh. They fall down in a straight
line, affected by gravity. An intact baloon is comprised of an ellipse, a
trapezoid for the part where the baloon is tied and a polyline for the string.
A deflated baloon changes the ellipse body for a mesh made up of 2 ellipse
sections and a circle section.

 - shurikens: these just rotate around their centre and move to the left of the
screen, towards the bow. When hit by an arrow, they start falling down and the
triangles are now broken to symbolize the fact that it's been hit. If the bow
is hit by one of these shurikens, the players loses a life and the shuriken
disappears. Shurikens are formed by rotating a right isosceles triangle by 90
degrees 3 times. The shot down shuriken just changes the angles between the
blades.

 - arrows: the previously shot arrows, which are affected by gravity. They are
made up of a rectangle for the body and a triangle for the arrowhead.

 - lives: a number of hearts equal to the number of remaining lives

 - the score

 - the game guide

    The game gets progressively harder every time the score is a multiple of 5
by speeding up the baloons and the shurikens.

    When the player loses all lives, a game over screen appears on screen and
the last state of the game is preserved.


CONTROLS

    W and S - move up and down
    hold SPACE or LEFT MOUSE BUTTON - charge the arrow up to a maximum point
    move MOUSE CURSOR - direct the bow and arrow