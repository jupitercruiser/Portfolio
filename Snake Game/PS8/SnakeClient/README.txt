Authors: Brandy Cervantes & Mia Mellem

- DESIGN DECISIONS
  Connect Button: 
	Once the client is connected, we disable the connect button 
	so that the user cannot break the program by continuosly clicking when 
	they're already connected.

  About Button:
	We edited this display alert to include our names.

  Draw Method:
	Our draw method doesn't draw until there is at least one player in 
	the World Player list and walls have been received from the server.
	
  Game Controller: 
	A client is not allowed to send the server commands on where to move if
	it has not yet received all of the necessary information and connected. 
	Up until then, the client's screen is blank. 

  Assigning Snake Colors: 
	For each new player that connected to the server, 
	we used a helper method that mods the Snake's ID by 
	8 and we chose a different color of pink based on the remainders.

  Powerups: 
	To draw powerups, we used a delegate that we passed into the DrawObjectWithTransform
	method. This delegate set a width size for the powerups of 16 since
	that was what was recommended. It also called canvas.FillEllipse to 
	actually draw the powerups. 

  Snakes:
	To draw snakes, we used a delegate that we passed into the DrawObjectWithTransform
	method. This delegate drew individual segments on the snakes which were made up of 
	two vectors. 

	We also had a separate delegate for the snake name and score. It displays
	the player's name and score above their head. We set the font to be default. 

	We set the stroke color of the snakes using the helper method mentioned above, and
	we set the width to be 10 since that was what was recommended. We defined these things
	before we called DrawObjectWithTransform on this snake. 

  Walls: 
	To draw walls, we used a lot of the same math as we did in drawing the snakes. We 
	would draw a wall image between its p1 and p2. We spaced them out by 50 pixels since 
	the edges of the walls are 25 pixels from the center. 

  Explosions: 
	We downloaded a pixelated explosion image and drew it when a player died. It 
	disapears when the player respawns. It does not appear if a player disconnects
	from the game without dying. 

- EVENTS
  OnFrame:
	This event handler was subscribed to our Game Controller. It was used to update every frame.

  ErrorConnect: 
	This event handler was subscried to our Game Controller. It was used to display an alert 
	whenever there was an issue with the connection to the server. It allows the user to click
	'Ok' and go back to the main page to retry the connection. 

   OnConnect: 
	This event handler was subscribed to our Game Controller. It was used to disable the 
	connect button once the connection was successful. 