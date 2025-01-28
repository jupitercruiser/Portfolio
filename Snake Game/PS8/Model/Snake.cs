/*
 * Authors: Brandy Cervantes & Mia Mellem
 * 
 * Snake Game Snake object representing a Snake
 */
using System.Runtime.Serialization;
using System.Text.Json.Serialization;

namespace SnakeGame
{
    /// <summary>
    /// This class makes a Snake object
    /// </summary>
    [DataContract(Namespace = "")]
    public class Snake
    {
        public long snake { get; set; }
        public string name { get;  set; }
        public List<Vector2D> body { get; set; }
        public Vector2D dir { get; set; }

        public int score { get; set; }

        public bool died { get; set; }

        public bool alive { get; set; }
        public bool  dc { get; set; } 

        public bool join { get; set; }

        [JsonIgnore]
        public Vector2D velocity { get; set; }

        [JsonIgnore]
        public int growSnakeDelay { get; set; }

        /// <summary>
        /// Default constructor for a snake object
        /// </summary>
        public Snake()
        {
            snake = 0;
            name = "";
            body = new List<Vector2D>();
            dir = new Vector2D();
            score = 0;
            died = false;
            alive = false;
            dc = false;
            join = false;
            velocity = new Vector2D();
        }

        public Snake(long id, string name)
        {
            snake = id;
            this.name = name;
            body = new List<Vector2D>();
            dir = new Vector2D();
            score = 0;
            died = false;
            alive = false;
            dc = false;
            join = false;
            velocity = new Vector2D();
        }

        /// <summary>
        /// Constructs a snake object using the snake's ID, the snake's 
        /// coordinates, and its direction.
        /// </summary>
        /// <param name="id">Snake's ID</param>
        /// <param name="x">Snake's x coordinate</param>
        /// <param name="y">Snake's y coordinate</param>
        /// <param name="angle">Direction of the snake</param>
        public Snake(long id, int x, int y, Vector2D angle)
        {
            snake = id;
            name = "";
            body = new List<Vector2D>() { new Vector2D(x,y) };
            dir = angle;
            score = 0;
            died = false;
            alive = false;
            dc = false;
            join = false;
            velocity = new Vector2D();
        }
    }
}

