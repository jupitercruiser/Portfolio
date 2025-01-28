Authors: Brandy Cervantes & Mia Mellem

- DESIGN DECISIONS
	Game Settings:
	We added a game settings class to our server so that we would be able 
	to deserialize the XML file and access the objects/vars that we needed
	from the new Game Settings object.

	Read Settings: 
	We added a helper method to complete the process of reading the XML file. 

	Collision Detection: 
	We decided to make separate methods for every type of collision and called 
	them in the update method. We did this to facilitate debugging and to make 
	the code more readable. We set most of these methods to return a boolean 
	which we later checked in our update method. 
	We checked the following collisions: 
		Snake Self
		Snake Snake
		Powerup Wall
		Snake Wall
		Powerup Whole Snake
		Powerup Powerup
		Snake Powerup
		& Wall Whole Snake

	Random Spawn Locations:
	We took similar approaches to spawning powerups and snakes. We would make a 
	new vector using rngs that wouldn't go over the world size, then we would 
	ensure that this vector didn't collide with any other object in the world, 
	and then we would set the object's location to this vector. 
	If the rng location vector ever did collide with an object, we would generate
	new numbers and try again. 
	
	Snake Class:
	We added an instance variable to this class that measured the frame delay for
	growing the snake so that multiple snakes wouldn't grow if only one ate. 

- PROBLEMS
	Wrap-around:
	We were not able to get our wrap-around to work

	Extra Feature: 
	We did not have time to implement an extra feature
