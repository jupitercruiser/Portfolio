/*
 * Authors: Brandy Cervantes & Mia Mellem
 * 
 * This class is a part of the Snake Game view.
 */
using System.Net.Security;

namespace SnakeGame;

public partial class MainPage : ContentPage
{
    private GameController gc;
    // private Server server;

    /// <summary>
    /// This method begins the drawing process and runs
    /// the Snake Game. 
    /// </summary>
    public MainPage()
    {
        InitializeComponent();
        graphicsView.Invalidate();

        gc = new GameController();

        // server = new Server();

        // informs the view to start drawing stuff
        gc.MessagesArrived += OnFrame;

        // informs the view there was an error when connecting
        gc.Error += ErrorConnect;

        // informs the view that the connection was successful
        gc.Connected += OnConnect;
        
        // Sets the worldPanel's world
        worldPanel.SetWorld(gc.theWorld);
    }

    /// <summary>
    /// This event handler refocuses the keyboard when a button is tapped
    /// </summary>
    void OnTapped(object sender, EventArgs args)
    {
        keyboardHack.Focus();
    }

    /// <summary>
    /// This event handler calls the Game Controller's move snake method
    /// based on the input from the user.
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="args"></param>
    void OnTextChanged(object sender, TextChangedEventArgs args)
    {
        keyboardHack.Focus();
        Entry entry = (Entry)sender;
        String text = entry.Text.ToLower();
        if (text == "w")
        {
            // Move up
            gc.MoveSnake(text);
        }
        else if (text == "a")
        {
            // Move left
            gc.MoveSnake(text);

        }
        else if (text == "s")
        {
            // Move down
            gc.MoveSnake(text);
        }
        else if (text == "d")
        {
            // Move right
            gc.MoveSnake(text);
        }
        entry.Text = "";
    }

    /// <summary>
    /// This event handler is used when a client is disconnected from 
    /// the server unintentionally
    /// </summary>
    private void NetworkErrorHandler()
    {
        DisplayAlert("Error", "Disconnected from server", "OK");
    }

    /// <summary>
    /// Event handler for the connect button
    /// We will put the connection attempt interface here in the view.
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="args"></param>
    private void ConnectClick(object sender, EventArgs args)
    {
        String name = nameText.Text;
        String addr = serverText.Text.ToLower();

        if (serverText.Text == "")
        {
            DisplayAlert("Error", "Please enter a server address", "OK");
            return;
        }
        if (nameText.Text == "")
        {
            DisplayAlert("Error", "Please enter a name", "OK");
            return;
        }
        if (nameText.Text.Length > 16)
        {
            DisplayAlert("Error", "Name must be less than 16 characters", "OK");
            return;
        }

        // Try to connect to the server
        try
        {
            gc.Connect(addr, name);
        }
        // Display an error message to the user if the connection to the server fails
        catch (Exception)
        {
            NetworkErrorHandler();
        }

        keyboardHack.Focus();
    }

    /// <summary>
    /// This event handler used when the controller has updated the world
    /// </summary>
    public void OnFrame()
    {
        Dispatcher.Dispatch(() => graphicsView.Invalidate());
    }

    /// <summary>
    /// This event handler used when there is an error connecting to the server
    /// </summary>
    public void ErrorConnect(string msg)
    {
        Dispatcher.Dispatch(() => DisplayAlert("Error", msg, "OK")); 
    }

    /// <summary>
    /// This event handler used when the connection to the server was successful
    /// </summary>
    public void OnConnect()
    {
        Dispatcher.Dispatch(() => connectButton.IsEnabled = false);
    }

    /// <summary>
    /// This event handler displays an alert showing the user what the controls
    /// are for the game
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void ControlsButton_Clicked(object sender, EventArgs e)
    {
        DisplayAlert("Controls",
                     "W:\t\t Move up\n" +
                     "A:\t\t Move left\n" +
                     "S:\t\t Move down\n" +
                     "D:\t\t Move right\n",
                     "OK");
    }

    /// <summary>
    /// This event handler displays an alert with the game design information
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void AboutButton_Clicked(object sender, EventArgs e)
    {
        DisplayAlert("About",
      "SnakeGame solution\nArtwork by Jolie Uk and Alex Smith\nGame design by Daniel Kopta and Travis Martin\n" +
      "Implementation by Brandy Cervantes and Mia Mellem\n" +
        "CS 3500 Fall 2023, University of Utah", "OK");
    }

    /// <summary>
    /// This event handler focuses the keyboard when the connect button is enabled
    /// </summary>
    /// <param name="sender"></param>
    /// <param name="e"></param>
    private void ContentPage_Focused(object sender, FocusEventArgs e)
    {
        if (!connectButton.IsEnabled)
            keyboardHack.Focus();
    }
}