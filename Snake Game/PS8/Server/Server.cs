using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;
using SnakeGame;
using NetworkUtil;
using System.Text.RegularExpressions;
using System.Runtime.Serialization;
using System.Xml;
using System.Xml.Linq;
using System.Diagnostics;
using System.Text.Json;
using System.Xml.Schema;
using static System.Runtime.InteropServices.JavaScript.JSType;
using System.ComponentModel;
using System.Security.Cryptography;

namespace SnakeGame
{
    /// <summary>
    /// This class creates a server for the snake game
    /// </summary>
    /// Authors: Brandy Cervantes and Mia Mellem
    public class Server
    {
        private World theServerWorld;
        private static long msPerFrame;
        private int respawnRate;
        private int size;
        private int snakeSpeed;
        private int maxPowerups;
        private int snakeRespawnDelay = 0;
        private int alivePowerupCounter;

        private List<SocketState> clients;

        // Make a server and update the world each frame
        public static void Main(string[] args)
        {
            // Construct a new server
            Server server = new();

            // Frame loop 
            // Start a new timer to control the frame rate
            System.Diagnostics.Stopwatch watch = new System.Diagnostics.Stopwatch();
            watch.Start();
            Console.WriteLine("Server is running. Accepting new clients");

            while (true)
            {
                // wait until the next frame
                while (watch.ElapsedMilliseconds < msPerFrame)
                {
                    /* empty loop body */
                }

                watch.Restart();

                server.Update();
            }
        }

        /// <summary>
        /// Constructs a server object
        /// </summary>
        public Server()
        {
            // set the world size
            theServerWorld = new World(0);

            // Read the XML settings file
            ReadSettings();

            // Set the snakeSpeed
            snakeSpeed = 6;

            // Start listening for connections 
            Networking.StartServer(HandleAcceptNewClient, 11000);
            lock (theServerWorld)
            {
                clients = new List<SocketState>();
            }
            maxPowerups = 20;
        }

        /// <summary>
        /// Uses a DataContractSerializer to read the contents of an XML file into a
        /// GameSettings object to get the world size and other important information.
        /// </summary>
        private void ReadSettings()
        {
            DataContractSerializer ser = new(typeof(GameSettings));

            XmlReader reader = XmlReader.Create("settings.xml");
            GameSettings? gameSettings = (GameSettings?)ser.ReadObject(reader);

            if (gameSettings != null)
            {
                msPerFrame = gameSettings.MSPerFrame;
                respawnRate = gameSettings.RespawnRate;
                size = gameSettings.UniverseSize;
                theServerWorld = new World(size);

                foreach (Wall w in gameSettings.Walls)
                {
                    theServerWorld.Walls.Add(w.wall, w);
                }
            }
        }

        /// <summary>
        /// Handles accepting a client when they attempt to connect to the server
        /// </summary>
        /// <param name="clientSocketState"></param>
        public void HandleAcceptNewClient(SocketState clientSocketState)
        {
            // Change the callback for the socket state to a new method that 
            // receives the player's name and asks for more data
            clientSocketState.OnNetworkAction = ReceivePlayerName;
            Networking.GetData(clientSocketState);
        }

        /// <summary>
        /// Gets the client's name via their data. Then serialzes the player's ID, world size, and walls to Json and sends
        /// them to the client. Adds the client to a list of clients, and creates a snake object to add
        /// to the world. Then asks for more data from the client.
        /// </summary>
        /// <param name="clientSocketState"></param>
        private void ReceivePlayerName(SocketState clientSocketState)
        {
            string name = clientSocketState.GetData();

            lock (theServerWorld)
            {
                clientSocketState.RemoveData(0, clientSocketState.GetData().Length);
            }

            // Change callback to method that handles command requests from client
            clientSocketState.OnNetworkAction = HandleClientData;

            // Send the start up info to the client
            lock (theServerWorld)
            {
                // Send the snake's ID & Send the world's size 
                Networking.Send(clientSocketState.TheSocket, clientSocketState.ID.ToString() + "\n" + theServerWorld.Size.ToString() + "\n");
                // Send the wall info
                foreach (int wID in theServerWorld.Walls.Keys)
                {
                    Networking.Send(clientSocketState.TheSocket, JsonSerializer.Serialize<Wall>(theServerWorld.Walls[wID]) + "\n");
                }

                // Add the clients socket to list of all clients AFTER SENDING INFO
                clients.Add(clientSocketState);

                // Make a snake and add it to theServerWorld players list AFTER the handshake info has been sent
                Snake snake = new Snake(clients[clients.Count - 1].ID, 0, 0, new Vector2D(1, 0));
                snake.name = name.Remove(name.Length - 1);

                theServerWorld.Players.Add(snake.snake, snake);

                SpawnSnakeRandom(snake);
                Console.WriteLine("Player(" + clientSocketState.ID + "): \"" + name.Remove(name.Length - 1) + "\" joined");

                // Ask client for data (commands)
                Networking.GetData(clientSocketState);
            }
        }

        /// <summary>
        /// Receives client move commands. Depending on what key the client presses, decides what direction to set the snake to
        /// </summary>
        /// <param name="clientSocketState"></param>
        private void HandleClientData(SocketState clientSocketState)
        {
            // Processes the client's direction commands
            string totalData = clientSocketState.GetData();
            string[] parts = Regex.Split(totalData, @"(?<=[\n])");

            lock(theServerWorld)
                clientSocketState.RemoveData(0, clientSocketState.GetData().Length);

            foreach (string s in parts)
            {
                if (theServerWorld.Players.TryGetValue(clientSocketState.ID, out Snake? snake) && snake.alive)
                {

                    if (s == "{\"moving\":\"up\"}\n" && !theServerWorld.Players[clientSocketState.ID].dir.Equals(new Vector2D(0, 1)))
                    {
                        theServerWorld.Players[clientSocketState.ID].dir = new Vector2D(0, -1);
                    }

                    else if (s == "{\"moving\":\"down\"}\n" && !theServerWorld.Players[clientSocketState.ID].dir.Equals(new Vector2D(0, -1)))
                    {
                        theServerWorld.Players[clientSocketState.ID].dir = new Vector2D(0, 1);
                    }

                    else if (s == "{\"moving\":\"left\"}\n" && !theServerWorld.Players[clientSocketState.ID].dir.Equals(new Vector2D(1, 0)))
                    {
                        theServerWorld.Players[clientSocketState.ID].dir = new Vector2D(-1, 0);
                    }

                    else if (s == "{\"moving\":\"right\"}\n" && !theServerWorld.Players[clientSocketState.ID].dir.Equals(new Vector2D(-1, 0)))
                    {
                        theServerWorld.Players[clientSocketState.ID].dir = new Vector2D(1, 0);
                    }
                    else if (s == "{\"moving\":\"none\"}\n")
                        continue;
                }
            }

            if(theServerWorld.Players.Count > 0)
            {
                // Ask for more data
                Networking.GetData(clientSocketState);
            }
        }

        /// <summary>
        /// Moves the snake. Adds a head vector in the direction the snake is going, and subtracts from the tail
        /// as it follows the head to remain the same length, unless the snake has eaten a powerup and needs to grow, in
        /// which case the tail will pause moving for 24 frames while the head continues to move.
        /// </summary>
        /// <param name="snake"></param>
        private void MoveSnake(Snake snake)
        {
            Vector2D head = snake.body.Last();

            snake.body.Add(head + snake.dir * snakeSpeed);

            //if the snake at a powerup, decrement the delay instead of having the tail follow behind the head
            if (snake.growSnakeDelay > 0)
                snake.growSnakeDelay--;
            //if there is no delay, have the tail follow the head
            else
            {
                Vector2D tail = snake.body.First();
                Vector2D tailDirection = snake.body[1] - snake.body.First();
                tailDirection.Normalize();
                lock (theServerWorld)
                { 
                    if (!(tail + tailDirection * snakeSpeed).Equals(snake.body[1]))
                        snake.body[0] = tail + tailDirection * snakeSpeed;
                    //if the tail is caught up to the next segment of the snake, delete it.
                    else
                    {
                        snake.body.Remove(snake.body[0]);
                        Vector2D distance = snake.body[0] - snake.body[1];
                        if (distance.Length() >= size)
                            snake.body.Remove(snake.body[0]);
                    }
                }
            }
        }

        /// <summary>
        /// Updates the world and sends it to each client every frame
        /// Spawns more powerups if any have been eaten
        /// Detects collisions between objects, if any object or snake is dead or disconnected, and respawns them if
        /// they are still playing the game.
        /// </summary>
        private void Update()
        {
            // Create random powerups 
            lock (theServerWorld)
            {
                while (alivePowerupCounter < maxPowerups)
                {
                    Random r = new Random();
                    int x = r.Next(-size / 2, size / 2);
                    int y = r.Next(-size / 2, size / 2);
                    //if the coordinates are colliding with any objects in the world, regenerate them.
                    while(WallPowerupCollision(x, y) || PowerupPowerupCollision(x, y) || PowerupWholeSnakeCollision(x, y))
                    {
                        x = r.Next(-size / 2 + 60, size / 2 - 60);
                        y = r.Next(-size / 2 + 60, size / 2 - 60);
                    }
                    Vector2D loc = new Vector2D(x, y);

                    Power p = new Power((int)loc.X, (int)loc.Y);

                    p.power = theServerWorld.Powerups.Count;
                    if (theServerWorld.Powerups.ContainsKey(p.power))
                        theServerWorld.Powerups[p.power] = p;
                    else
                    {
                        theServerWorld.Powerups.Add(p.power, p);
                        alivePowerupCounter++;
                    }
                }
            }

            // loop through each client
            foreach (SocketState client in clients)
            {
                if (client.TheSocket.Connected && !client.ErrorOccurred)
                {
                    // Spawn the snake taking the snake delay into account
                    if (snakeRespawnDelay > 0)
                        lock (theServerWorld)
                        {
                            snakeRespawnDelay--;
                        }
                    else
                    {
                        if (theServerWorld.Players[client.ID].died && !theServerWorld.Players[client.ID].dc)
                        {
                            theServerWorld.Players[client.ID].died = false;
                            SpawnSnakeRandom(theServerWorld.Players[client.ID]);
                        }
                    }

                    
                    // Move the snake & check for collisions
                    if (!theServerWorld.Players[client.ID].died && !theServerWorld.Players[client.ID].dc)
                    {
                        MoveSnake(theServerWorld.Players[client.ID]);
                        SnakePowerupCollision(theServerWorld.Players[client.ID]);

                        //if snake collides with anything that would kill it
                        if (SnakeWallCollision(theServerWorld.Players[client.ID]) || SnakeSnakeCollision(theServerWorld.Players[client.ID])
                            || SnakeSelfCollision(theServerWorld.Players[client.ID]))
                        {
                            lock (theServerWorld)
                            {
                                theServerWorld.Players[client.ID].alive = false;
                                theServerWorld.Players[client.ID].died = true;
                                theServerWorld.Players[client.ID].score = 0;
                                snakeRespawnDelay = respawnRate;
                            }
                        }
                    }

                    // Send Players
                    foreach (int i in theServerWorld.Players.Keys)
                    {
                        if (!client.ErrorOccurred)
                        {
                            lock (theServerWorld)
                            {
                                Networking.Send(client.TheSocket, JsonSerializer.Serialize<Snake>(theServerWorld.Players[i]) + "\n");
                            }
                        }
                    }
                    // Send Powerups
                    foreach (int p in theServerWorld.Powerups.Keys)
                    {
                        if (!client.ErrorOccurred)
                        {
                            lock (theServerWorld)
                            {
                                Networking.Send(client.TheSocket, JsonSerializer.Serialize<Power>(theServerWorld.Powerups[p]) + "\n");
                            }
                        }
                    }
                }
                // check if a client has disconnected
                if (!client.TheSocket.Connected && theServerWorld.Players.ContainsKey(client.ID))
                {
                    lock (theServerWorld)
                    {
                        theServerWorld.Players[client.ID].dc = true;
                        theServerWorld.Players[client.ID].died = true;
                        theServerWorld.Players[client.ID].alive = false;
                        Console.WriteLine("Player (" + client.ID + "): \"" + theServerWorld.Players[client.ID].name + "\" disconnected");

                        Networking.Send(client.TheSocket, JsonSerializer.Serialize<Snake>(theServerWorld.Players[client.ID]) + "\n");
                        lock (theServerWorld)
                        {
                            theServerWorld.Players.Remove(client.ID);
                        }
                    }
                }
            }
        }

        /// <summary>
        /// Spawns a snake in a random location in a random direction (up, down, left, or right).
        /// If the random coordinates chosen collide with any other objects, regenerates until
        /// the snake will not collide with anything when it spawns
        /// </summary>
        /// <param name="s"></param>
        private void SpawnSnakeRandom(Snake s)
        {
            lock (theServerWorld)
            {
                if (!s.died)
                {
                    Random r = new Random();
                    int x = r.Next(-size / 2 + 200, size / 2 - 200);
                    int y = r.Next(-size / 2 + 200, size / 2 - 200);
                    string name = s.name;
                    s = new Snake(s.snake, x, y, new Vector2D(0, 0));

                    //if the coordinates are colliding with another object, regenerate.
                    while (WallWholeSnakeCollision(s) || SnakeSnakeCollision(s) || PowerupWholeSnakeCollision(x, y))
                    {
                        s.body[0].X = r.Next(-size / 2, size / 2);
                        s.body[0].Y = r.Next(-size / 2, size / 2);
                    }

                    Vector2D loc = new Vector2D(x, y);
                    Vector2D head = new Vector2D(0, 0);

                    Random r2 = new Random();

                    int xDir = r.Next(-1, 2);
                    int yDir;

                    if (xDir == 0)
                    {
                        yDir = xDir == 0 ? 1 : -1;
                        if (yDir == 1)
                            head.Y = y + 120;
                        else if (yDir == -1)
                            head.Y = y - 120;
                        head.X = x;
                    }
                    else
                    {
                        if (xDir == 1)
                            head.X = x + 120;
                        else if (xDir == -1)
                            head.X = x - 120;
                        yDir = 0;
                        head.Y = y;
                    }

                    Vector2D direction = new Vector2D(xDir, yDir);
                    direction.Normalize();

                    lock (theServerWorld)
                    {
                        s = new Snake(s.snake, (int)loc.X, (int)loc.Y, direction);
                        s.name = name;
                        theServerWorld.Players[s.snake] = s;
                        theServerWorld.Players[s.snake].body.Add(head);
                        theServerWorld.Players[s.snake].alive = true;
                    }
                }
            }
        }

        /// <summary>
        /// returns true if a wall and powerup are colliding, false if they are not
        /// </summary>
        /// <param name="x"></param>
        /// <param name="y"></param>
        /// <returns></returns>
        private bool WallPowerupCollision(int x, int y)
        {
            double xOnePrime;
            double yOnePrime;
            double xTwoPrime;
            double yTwoPrime;

            foreach (int wID in theServerWorld.Walls.Keys)
            {
                if (theServerWorld.Walls[wID].p1.X < theServerWorld.Walls[wID].p2.X)
                {
                    xOnePrime = theServerWorld.Walls[wID].p1.X - 35;
                    xTwoPrime = theServerWorld.Walls[wID].p2.X + 35;
                }
                else
                {
                    xOnePrime = theServerWorld.Walls[wID].p2.X - 35;
                    xTwoPrime = theServerWorld.Walls[wID].p1.X + 35;

                }
                if (theServerWorld.Walls[wID].p1.Y < theServerWorld.Walls[wID].p2.Y)
                {
                    yOnePrime = theServerWorld.Walls[wID].p1.Y - 35;
                    yTwoPrime = theServerWorld.Walls[wID].p2.Y + 35;
                }
                else
                {
                    yOnePrime = theServerWorld.Walls[wID].p2.Y - 35;
                    yTwoPrime = theServerWorld.Walls[wID].p1.Y + 35;

                }

                if ((xOnePrime < x && x < xTwoPrime) && (yOnePrime < y && y < yTwoPrime) ||
                    (x > size/2 - 60) || (x < -size / 2 + 60) || (y > size / 2 - 60) || (y < -size / 2 + 60))
                {
                    return true;
                }
            }
            return false;
        }

        /// <summary>
        /// Checks the snake's head against it's segments to see if it is hitting itself
        /// returns true if a snake is running into itself, false if not
        /// </summary>
        /// <param name="snake"></param>
        /// <returns></returns>
        private bool SnakeSelfCollision(Snake snake)
        {
            Vector2D head = snake.body.Last();
            double xOnePrime;
            double yOnePrime;
            double xTwoPrime;
            double yTwoPrime;
            if (snake.body.Count >= 3)
            {
                for (int i = snake.body.Count - 4; i > 0; i--)
                {
                    if (snake.body[i].X < snake.body[i - 1].X)
                    {
                        xOnePrime = snake.body[i].X - 2;
                        xTwoPrime = snake.body[i - 1].X + 2;
                    }
                    else
                    {
                        xOnePrime = snake.body[i - 1].X - 2;
                        xTwoPrime = snake.body[i].X + 2;

                    }
                    if (snake.body[i].Y < snake.body[i - 1].Y)
                    {
                        yOnePrime = snake.body[i].Y - 2;
                        yTwoPrime = snake.body[i - 1].Y + 2;
                    }
                    else
                    {
                        yOnePrime = snake.body[i - 1].Y - 2;
                        yTwoPrime = snake.body[i].Y + 2;

                    }

                    if ((xOnePrime < head.X && head.X < xTwoPrime) && (yOnePrime < head.Y && head.Y < yTwoPrime))
                    {
                        return true;
                    }
                }
                return false;
            }
            
            return false;
        }

        /// <summary>
        /// checks a powerup against an entire snake, not just its head.
        /// returns true if colliding, false if not.
        /// </summary>
        /// <param name="x"></param>
        /// <param name="y"></param>
        /// <returns></returns>
        private bool PowerupWholeSnakeCollision(int x, int y)
        {
            double xOnePrime;
            double yOnePrime;
            double xTwoPrime;
            double yTwoPrime;
            foreach (int p in theServerWorld.Players.Keys)
            {
                for (int i = 0; i < theServerWorld.Players[p].body.Count - 1; i++)
                {
                    if (theServerWorld.Players[p].body[i].X < theServerWorld.Players[p].body[i + 1].X)
                    {
                        xOnePrime = theServerWorld.Players[p].body[i].X - 10;
                        xTwoPrime = theServerWorld.Players[p].body[i + 1].X + 10;
                    }
                    else
                    {
                        xOnePrime = theServerWorld.Players[p].body[i + 1].X - 10;
                        xTwoPrime = theServerWorld.Players[p].body[i].X + 10;

                    }
                    if (theServerWorld.Players[p].body[i].Y < theServerWorld.Players[p].body[i + 1].Y)
                    {
                        yOnePrime = theServerWorld.Players[p].body[i].Y - 10;
                        yTwoPrime = theServerWorld.Players[p].body[i + 1].Y + 10;
                    }
                    else
                    {
                        yOnePrime = theServerWorld.Players[p].body[i + 1].Y - 10;
                        yTwoPrime = theServerWorld.Players[p].body[i].Y + 10;

                    }

                    if ((xOnePrime < x && x < xTwoPrime) && (yOnePrime < y && y < yTwoPrime))
                    {
                        return true;
                    }
                }
            }
            return false;
        }

        /// <summary>
        /// Returns true if a powerup is colliding with any other powerup in the world,
        /// and false if not.
        /// </summary>
        /// <param name="x"></param>
        /// <param name="y"></param>
        /// <returns></returns>
        private bool PowerupPowerupCollision(int x, int y)
        {
            Vector2D v = new Vector2D(x, y);
            foreach (int pID in theServerWorld.Powerups.Keys)
            {
                if ((theServerWorld.Powerups[pID].loc - v).Length() < 10)
                    return true;
            }
            return false;
        }

        /// <summary>
        /// Checks to see if the snake's head is colliding with a powerup so it can "eat" it
        /// If true, sets the delay for the snake's tail to grow to 24 frames.
        /// Returns false if not colliding.
        /// </summary>
        /// <param name="snake"></param>
        private void SnakePowerupCollision(Snake snake)
        {
            //foreach powerup
            foreach (int pID in theServerWorld.Powerups.Keys)
            {
                // if there is a snake powerup collision
                if ((theServerWorld.Powerups[pID].loc - snake.body.Last()).Length() < 10)
                {
                    // update the players score
                    if (!theServerWorld.Powerups[pID].died)
                        lock (theServerWorld)
                        {
                            alivePowerupCounter--;
                            snake.score++;
                            snake.growSnakeDelay = 24;
                            // set the power up to died
                            theServerWorld.Powerups[pID].died = true;
                        }
                }
            }
        }

        /// <summary>
        /// Checks against every snake in the world to see if two snakes are colliding.
        /// Returns true if a snake is hitting another snake, false if not.
        /// </summary>
        /// <param name="snake"></param>
        /// <returns></returns>
        private bool SnakeSnakeCollision(Snake snake)
        {
            Vector2D head = snake.body.Last();
            double xOnePrime;
            double yOnePrime;
            double xTwoPrime;
            double yTwoPrime;

            foreach (int sID in theServerWorld.Players.Keys)
            {
                if(sID != snake.snake && theServerWorld.Players[sID].alive && snake.alive)
                {
                    for (int i = 0; i < theServerWorld.Players[sID].body.Count - 1; i++)
                    {
                        if (theServerWorld.Players[sID].body[i].X < theServerWorld.Players[sID].body[i + 1].X)
                        {
                            xOnePrime = theServerWorld.Players[sID].body[i].X - 5;
                            xTwoPrime = theServerWorld.Players[sID].body[i + 1].X + 5;
                        }
                        else
                        {
                            xOnePrime = theServerWorld.Players[sID].body[i + 1].X - 5;
                            xTwoPrime = theServerWorld.Players[sID].body[i].X + 5;

                        }
                        if (theServerWorld.Players[sID].body[i].Y < theServerWorld.Players[sID].body[i + 1].Y)
                        {
                            yOnePrime = theServerWorld.Players[sID].body[i].Y - 5;
                            yTwoPrime = theServerWorld.Players[sID].body[i + 1].Y + 5;
                        }
                        else
                        {
                            yOnePrime = theServerWorld.Players[sID].body[i + 1].Y - 5;
                            yTwoPrime = theServerWorld.Players[sID].body[i].Y + 5;

                        }

                        if ((xOnePrime < head.X && head.X < xTwoPrime) && (yOnePrime < head.Y && head.Y < yTwoPrime))
                        {
                            return true;
                        }
                    }
                }
            }
            return false;
        }

        /// <summary>
        /// returns true if a snake is running into a wall with its head, false if not
        /// </summary>
        /// <param name="snake"></param>
        /// <returns></returns>
        private bool SnakeWallCollision(Snake snake)
        {
            Vector2D head = snake.body.Last();
            double xOnePrime;
            double yOnePrime;
            double xTwoPrime;
            double yTwoPrime;

            foreach (int wID in theServerWorld.Walls.Keys)
            {
                if (theServerWorld.Walls[wID].p1.X < theServerWorld.Walls[wID].p2.X)
                {
                    xOnePrime = theServerWorld.Walls[wID].p1.X - 30;
                    xTwoPrime = theServerWorld.Walls[wID].p2.X + 30;
                }
                else
                {
                    xOnePrime = theServerWorld.Walls[wID].p2.X - 30;
                    xTwoPrime = theServerWorld.Walls[wID].p1.X + 30;

                }
                if (theServerWorld.Walls[wID].p1.Y < theServerWorld.Walls[wID].p2.Y)
                {
                    yOnePrime = theServerWorld.Walls[wID].p1.Y - 30;
                    yTwoPrime = theServerWorld.Walls[wID].p2.Y + 30;
                }
                else
                {
                    yOnePrime = theServerWorld.Walls[wID].p2.Y - 30;
                    yTwoPrime = theServerWorld.Walls[wID].p1.Y + 30;

                }

                if ((xOnePrime < head.X && head.X < xTwoPrime) && (yOnePrime < head.Y && head.Y < yTwoPrime))
                {
                    return true;
                }
            }
            return false;
        }

        /// <summary>
        /// Helper for spawning snakes so they don't overlap with walls
        /// Returns true if any part of a snake is overlapping with a wall or within 200 units of a wall, and
        /// false if not
        /// </summary>
        /// <param name="snake"></param>
        /// <returns></returns>
        private bool WallWholeSnakeCollision(Snake snake)
        {
            Vector2D head = snake.body.Last();
            double xOnePrime;
            double yOnePrime;
            double xTwoPrime;
            double yTwoPrime;

            foreach (int wID in theServerWorld.Walls.Keys)
            {
                if (theServerWorld.Walls[wID].p1.X < theServerWorld.Walls[wID].p2.X)
                {
                    xOnePrime = theServerWorld.Walls[wID].p1.X - 200;
                    xTwoPrime = theServerWorld.Walls[wID].p2.X + 200;
                }
                else
                {
                    xOnePrime = theServerWorld.Walls[wID].p2.X - 200;
                    xTwoPrime = theServerWorld.Walls[wID].p1.X + 200;

                }
                if (theServerWorld.Walls[wID].p1.Y < theServerWorld.Walls[wID].p2.Y)
                {
                    yOnePrime = theServerWorld.Walls[wID].p1.Y - 200;
                    yTwoPrime = theServerWorld.Walls[wID].p2.Y + 200;
                }
                else
                {
                    yOnePrime = theServerWorld.Walls[wID].p2.Y - 200;
                    yTwoPrime = theServerWorld.Walls[wID].p1.Y + 200;

                }

                if ((xOnePrime < head.X && head.X < xTwoPrime) && (yOnePrime < head.Y && head.Y < yTwoPrime))
                {
                    return true;
                }
            }
            return false;
        }
    }
}