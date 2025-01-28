/*
* Authors: Brandy Cervantes & Mia Mellem
* 
* World Panel that draws the snakes, powerups, walls, and background
*/
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using IImage = Microsoft.Maui.Graphics.IImage;
#if MACCATALYST
using Microsoft.Maui.Graphics.Platform;
#else
using Microsoft.Maui.Graphics.Win2D;
#endif
using Color = Microsoft.Maui.Graphics.Color;
using System.Reflection;
using Microsoft.Maui;
using System.Net;
using Font = Microsoft.Maui.Graphics.Font;
using SizeF = Microsoft.Maui.Graphics.SizeF;
using System.Text.Json.Serialization;
using Microsoft.Maui.Graphics;

namespace SnakeGame;
/// <summary>
/// This class draws all of the Snake Game objects
/// </summary>
public class WorldPanel : ScrollView, IDrawable
{
    public delegate void ObjectDrawer(object o, ICanvas canvas);
    private GraphicsView graphicsView = new();
    public World theWorld;

    private IImage wall;
    private IImage background;
    private IImage explosion;

    private bool initializedForDrawing = false;

    /// <summary>
    /// This method loads images 
    /// </summary>
    /// <param name="name">name of the image</param>
    private IImage loadImage(string name)
    {
        Assembly assembly = GetType().GetTypeInfo().Assembly;
        string path = "SnakeClient.Resources.Images";
        using (Stream stream = assembly.GetManifestResourceStream($"{path}.{name}"))
        {
#if MACCATALYST
            return PlatformImage.FromStream(stream);
#else
            return new W2DImageLoadingService().FromStream(stream);
#endif
        }
    }

    /// <summary>
    /// Constructs a world panel
    /// </summary>
    public WorldPanel()
    {
        graphicsView.Drawable = this;
        graphicsView.HeightRequest = 500;
        graphicsView.WidthRequest = 500;
        this.Content = graphicsView;
    }

    /// <summary>
    /// This method sets the world in WorldPanel to the
    /// world in the argument
    /// </summary>
    /// <param name="w">The current world object</param>
    public void SetWorld(World w)
    {
        theWorld = w;
    }

    /// <summary>
    /// This method implicitly calls the draw method
    /// </summary>
    public void Invalidate()
    {
        Dispatcher.Dispatch(() => graphicsView.Invalidate());
    }

    /// <summary>
    /// This method loads the images needed for the Snake Game
    /// </summary>
    private void InitializeDrawing()
    {
        wall = loadImage("wallsprite.png");
        background = loadImage("background.png");
        explosion = loadImage("explosion.png");
        initializedForDrawing = true;
    }

    /// <summary>
    /// This method performs a translation and rotation to draw an object.
    /// </summary>
    /// <param name="canvas">The canvas object for drawing onto</param>
    /// <param name="o">The object to draw</param>
    /// <param name="worldX">The X component of the object's position in world space</param>
    /// <param name="worldY">The Y component of the object's position in world space</param>
    /// <param name="angle">The orientation of the object, measured in degrees clockwise from "up"</param>
    /// <param name="drawer">The drawer delegate. After the transformation is applied, the delegate is invoked to draw whatever it wants</param>
    private void DrawObjectWithTransform(ICanvas canvas, object o, double worldX, double worldY, double angle, ObjectDrawer drawer)
    {
        // "push" the current transform
        canvas.SaveState();

        canvas.Translate((float)worldX, (float)worldY);
        canvas.Rotate((float)angle);
        drawer(o, canvas);

        // "pop" the transform
        canvas.RestoreState();
    }

    /// <summary>
    /// A method that can be used as an ObjectDrawer delegate
    /// </summary>
    /// <param name="o">The player to draw</param>
    /// <param name="canvas">Canvas to draw on</param>
    private void SnakeSegmentDrawer(object o, ICanvas canvas)
    {
        double snakeSegmentLength = 0;

        if (o != null)
        {
            snakeSegmentLength = (double)o;
        }

        // Draw the segment
        canvas.DrawLine(0, 0, 0, -(float)snakeSegmentLength);
    }

    /// <summary>
    /// A method that can be used as an ObjectDrawer delegate
    /// </summary>
    /// <param name="o">The player to draw</param>
    /// <param name="canvas">Canvas to draw on</param>
    private void SnakeInfoDrawer(object o, ICanvas canvas)
    {
        Snake currSnake = o as Snake;

        if (o != null)
        {
            canvas.StrokeColor = ColorFromID((int)currSnake.snake);
            canvas.StrokeSize = 10;
            canvas.FontColor = Colors.White;
            canvas.FontSize = 10;
            canvas.Font = Font.Default;

            canvas.DrawString(currSnake.name + ": " + currSnake.score, 0, 15, HorizontalAlignment.Center);
        }
    }

    /// <summary>
    /// A method that can be used as an ObjectDrawer delegate
    /// </summary>
    /// <param name="o">The powerup to draw</param>
    /// <param name="canvas">Canvas to draw on</param>
    private void PowerupDrawer(object o, ICanvas canvas)
    {
        int width = 16;
        canvas.FillColor = Colors.DarkMagenta;

        canvas.FillEllipse(-width / 2, -width / 2, width, width);
    }

    /// <summary>
    /// A method that can be used as an ObjectDrawer delegate
    /// </summary>
    /// <param name="o">The powerup to draw</param>
    /// <param name="canvas">canvas to draw on</param>
    private void WallDrawer(object o, ICanvas canvas)
    {
        canvas.DrawImage(wall, -wall.Width / 2, -wall.Height / 2, wall.Width, wall.Height);
    }

    /// <summary>
    /// A method that can be used as an ObjectDrawer delegate
    /// </summary>
    /// <param name="o">The powerup to draw</param>
    /// <param name="canvas">canvas to draw on</param>
    private void ExplosionDrawer(object o, ICanvas canvas)
    {
        canvas.DrawImage(explosion, -25, -25, 50, 50);
    }

    /// <summary>
    /// This runs whenever the drawing panel is invalidated and draws the game
    /// </summary>
    /// <param name="canvas">canvas to draw on</param>
    public void Draw(ICanvas canvas, RectF dirtyRect)
    {
        if (theWorld.Players.Count >= 1 && (theWorld.Walls.Count >= 1 || theWorld.SnakeID != -1))
        {
            // Wait until draw is called to begin loading images, and only do it once
            if (!initializedForDrawing)
            {
                InitializeDrawing();
            }

            // undo any leftover transformations from last frame
            canvas.ResetState();

            if (theWorld.Players.ContainsKey(theWorld.SnakeID))
            {
                float playerX = (float)theWorld.Players[theWorld.SnakeID].body.Last().X;
                float playerY = (float)theWorld.Players[theWorld.SnakeID].body.Last().Y;

                canvas.Translate(-playerX + (900 / 2), -playerY + (900 / 2));
            }

            // Draw background
            canvas.DrawImage(background, -theWorld.Size / 2, -theWorld.Size / 2, theWorld.Size, theWorld.Size);

            lock (theWorld)
            {
                // draw the powerups
                foreach (var p in theWorld.Powerups.Values)
                {
                    if (!p.died)
                    {
                        DrawObjectWithTransform(canvas, p,
                                            p.loc.GetX(), p.loc.GetY(), 0,
                                            PowerupDrawer);
                    }
                }

                // draw the walls
                foreach (var w in theWorld.Walls.Values)
                {
                    Vector2D diff = w.p1 - w.p2;
                    double length = diff.Length();
                    diff.Normalize();

                    // X = 0, vertical
                    if (diff.X == 0)
                    {
                        if (diff.Y < 0)
                        {
                            // Draw forwards
                            for (int i = 0; i <= length; i += 50)
                            {
                                DrawObjectWithTransform(canvas, diff, w.p1.X, w.p1.Y + i, diff.ToAngle(), WallDrawer);
                            }
                        }
                        else if (diff.Y > 0)
                        {
                            // Draw backwards
                            for (double i = length; i >= 0; i -= 50)
                            {
                                DrawObjectWithTransform(canvas, diff, w.p1.X, w.p1.Y - i, diff.ToAngle(), WallDrawer);
                            }
                        }
                    }
                    // Y = 0, horizontal
                    else
                    {

                        if (diff.X < 0)
                        {
                            // Draw forwards
                            for (int i = 0; i <= length; i += 50)
                            {
                                DrawObjectWithTransform(canvas, diff, w.p1.X + i, w.p1.Y, diff.ToAngle(), WallDrawer);
                            }
                        }
                        else if (diff.X > 0)
                        {
                            // Draw backwards
                            for (double i = length; i >= 0; i -= 50)
                            {
                                DrawObjectWithTransform(canvas, diff, w.p1.X - i, w.p1.Y, diff.ToAngle(), WallDrawer);
                            }
                        }
                    }
                }

                // draw the snakes
                foreach (var s in theWorld.Players.Values)
                {
                    if (s.alive)
                    {
                        // Set the Stroke Color based on snake's ID
                        // Set snake appearance
                        canvas.StrokeColor = ColorFromID((int)s.snake);
                        canvas.StrokeSize = 10;
                        canvas.StrokeLineCap = LineCap.Round;

                        // Loop through snake's body
                        for (int i = 0; i < s.body.Count - 1; i++)
                        {
                            Vector2D diff = s.body[i + 1] - s.body[i];

                            double snakeSegmentLength = 0;

                            // Ys are different
                            if (diff.X == 0)
                            {
                                // calculate segment length
                                snakeSegmentLength = diff.Length();

                                diff.Normalize();
                                DrawObjectWithTransform(canvas, snakeSegmentLength, s.body[i].X, s.body[i].Y, diff.ToAngle(), SnakeSegmentDrawer);

                            }
                            // Xs are different
                            else
                            {
                                // calculate segment length
                                snakeSegmentLength = diff.Length();
                                diff.Normalize();

                                DrawObjectWithTransform(canvas, snakeSegmentLength, s.body[i].X, s.body[i].Y, diff.ToAngle(), SnakeSegmentDrawer);
                            }
                        }
                        DrawObjectWithTransform(canvas, s, s.body.Last().X, s.body.Last().Y, 0, SnakeInfoDrawer);
                    }
                    else if (!s.dc)
                    {
                        DrawObjectWithTransform(canvas, s, s.body.Last().X, s.body.Last().Y, 0, ExplosionDrawer);
                    }
                }
            }
        }
    }

    /// <summary>
    /// Picks a stroke color for the snake based on the snake's ID
    /// </summary>
    /// <param name="id">Snake's ID</param>
    /// <returns>Stroke color for snake</returns>
    private Color ColorFromID(int id)
    {
        int i = id % 8;

        if (i == 0)

            return Colors.HotPink;

        else if (i == 1)
            return Colors.LightPink;

        else if (i == 2)
            return Colors.Magenta;

        else if (i == 3)
            return Colors.Pink;

        else if (i == 4)
            return Colors.PeachPuff;

        else if (i == 5)
            return Colors.MistyRose;

        else if (i == 6)
            return Colors.Fuchsia;

        else
            return Colors.DeepPink;
    }
}