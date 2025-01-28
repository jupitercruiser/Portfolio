/*
* Authors: Brandy Cervantes & Mia Mellem
* 
* Snake Game Controller: Takes care of the networking 
* part of the client. 
*/

using System.Text.RegularExpressions;
using NetworkUtil;
using Newtonsoft.Json.Linq;
using SnakeGame;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;
using System.Reflection.Metadata;
using System.Diagnostics;
using System.Xml.Linq;

namespace SnakeGame;

/// <summary>
/// Game Controller class that connects the client
/// to the server, decodes the server's messages, 
/// and sends the server messages from the client.
/// </summary>
public class GameController
{
    // Controller events that the view can subscribe to
    // Used to inform the view that messages have arrived from 
    // the server.
    public delegate void MessageHandler();
    public event MessageHandler? MessagesArrived;

    // Used to inform the view that the connection was successful.
    public delegate void ConnectedHandler();
    public event ConnectedHandler? Connected;

    // Used to inform the view that there was an error when 
    // connecting to the server.
    public delegate void ErrorHandler(string err);
    public event ErrorHandler? Error;

    /// <summary>
    /// State representing the connection with the server
    /// </summary>
    SocketState? theServer = null;

    public World theWorld;
    private string name;

    public GameController()
    {
        theWorld = new World(0);
        name = "";
    }

    /// <summary>
    /// Begins the process of connecting to the server
    /// </summary>
    /// <param name="addr">Address of server to connect to</param>
    public void Connect(string addr, string name)
    {
        this.name = name;
        Networking.ConnectToServer(OnConnect, addr, 11000);
    }


    /// <summary>
    /// Method to be invoked by the networking library when a connection is made
    /// </summary>
    /// <param name="state">State representing the connection with the server</param>
    private void OnConnect(SocketState state)
    {
        if (state.ErrorOccurred)
        {
            // inform the view that there was an error
            Error?.Invoke("Error connecting to server! Try again");
            return;
        }

        theServer = state;

        // Send the client's name to the server
        MessageEntered(name);

        // Start an event loop to receive messages from the server
        state.OnNetworkAction = ReceiveMessage;

        Networking.GetData(state);

        // inform the view that the connection was successful
        Connected?.Invoke();
    }

    /// <summary>
    /// Method to be invoked by the networking library when 
    /// data is available
    /// </summary>
    /// <param name="state">State representing the connection with the server</param>
    private void ReceiveMessage(SocketState state)
    {
        if (state.ErrorOccurred)
        {
            // inform the view that there was an error connecting to the server
            Error?.Invoke("Lost connection to server");
            return;
        }

        ProcessMessages(state);

        Networking.GetData(state); 
    }

    /// <summary>
    /// Process any buffered messages separated by '\n'
    /// Then inform the view
    /// </summary>
    /// <param name="state"></param>
    private void ProcessMessages(SocketState state)
    {
            // Get the data from the socket state
            string totalData = state.GetData();

            // Split the data at ever new line
            string[] parts = Regex.Split(totalData, @"(?<=[\n])");
            

            // Update the model
            if (theWorld.FirstSnake)
            {
                // Set the current client's snake ID
                theWorld.SnakeID = int.Parse(parts[0]);
                lock (theWorld)
                {
                    state.RemoveData(0, parts[0].Length);
                }

                // Set the world size
                theWorld.Size = (int.Parse(parts[1]));
                lock (theWorld)
                {
                    state.RemoveData(0, parts[1].Length);
                }

                theWorld.FirstSnake = false;
                return;
            }

            if (theWorld.SnakeID != -1)
            {
                for (int i = 0; i < parts.Count() - 1; i++)
                {
                    JsonDocument doc = JsonDocument.Parse(parts[i]);
                    //Debug.WriteLine(parts[i]);

                    lock (theWorld)
                    {
                        state.RemoveData(0, parts[i].Length);
                    }

                    // if Json msg is a wall
                    if (doc.RootElement.TryGetProperty("wall", out _))
                    {
                        Wall? wall = JsonSerializer.Deserialize<Wall>(parts[i]);
                        if (wall != null)
                        {
                            if (theWorld.Walls.ContainsKey(wall.wall))
                            {
                                lock(theWorld)
                                {
                                    theWorld.Walls[wall.wall] = wall;
                                }
                            }   
                            else
                            {
                                lock (theWorld)
                                {
                                    theWorld.Walls.Add(wall.wall, wall);
                                }
                            }
                        }
                    }
                    // If Json msg is a snake
                    else if (doc.RootElement.TryGetProperty("snake", out _))
                    {
                        Snake? snake = JsonSerializer.Deserialize<Snake>(parts[i]);
                        if (snake != null)
                        {
                            if (theWorld.Players.ContainsKey(snake.snake))
                            {
                                lock(theWorld)
                                {
                                    theWorld.Players[snake.snake] = snake;
                                }
                            }
                            else
                            {
                                lock(theWorld)
                                {
                                    theWorld.Players.Add(snake.snake, snake);
                                }
                            }
                        }
                    }
                    // if Json is a Powerup
                    else if (doc.RootElement.TryGetProperty("power", out _))
                    {
                        Power? powerUp = JsonSerializer.Deserialize<Power>(parts[i]);
                        if (powerUp != null)
                        {
                            if (theWorld.Powerups.ContainsKey(powerUp.power))
                            {
                                lock (theWorld)
                                {
                                    theWorld.Powerups[powerUp.power] = powerUp;
                                } 
                            }
                            else
                            {
                                lock(theWorld)
                                {
                                    theWorld.Powerups.Add(powerUp.power, powerUp);
                                }

                            }
                        }
                    }
                }
            }
        
        // inform the view that messages arrived from the server
        MessagesArrived?.Invoke();
    }

    /// <summary>
    /// Closes the connection with the server
    /// </summary>
    public void Close()
    {
        theServer?.TheSocket.Close();
    }

    /// <summary>
    /// Sends a message to the server
    /// </summary>
    /// <param name="message">Message to send</param>
    public void MessageEntered(string message)
    {
        if (theServer is not null)
            Networking.Send(theServer.TheSocket, message + "\n");
    }

    /// <summary>
    /// Send a message to the server iff it has received the snake ID and the World Size
    /// </summary>
    /// <param name="message">Message to send</param>
    public void IdWorldSizeAndWallsReceived(string message)
    {
        if (theWorld.Size != 0 && theWorld.Walls.Count != 0 && theServer is not null)
        {
            Networking.Send(theServer.TheSocket, message + "\n");
            // Debug.WriteLine(message);
        }
    }

    /// <summary>
    /// This method takes in a letter and sends the server a message to tell them where the client
    /// wants to move to. It is used by the View.
    /// </summary>
    /// <param name="letter">Letter representing the direction of the movement</param>
    public void MoveSnake(string letter)
    {
        if (letter == "w")
        {
            IdWorldSizeAndWallsReceived("{\"moving\":\"up\"}");
        }
        else if (letter == "s")
        {
            IdWorldSizeAndWallsReceived("{\"moving\":\"down\"}");

        }
        else if (letter == "a")
        {
            IdWorldSizeAndWallsReceived("{\"moving\":\"left\"}");

        }
        else if (letter == "d")
        {
            IdWorldSizeAndWallsReceived("{\"moving\":\"right\"}");

        }
        else
        {
            IdWorldSizeAndWallsReceived("{\"moving\":\"none\"}");
        }
    }
}