/*
 * Authors: Brandy Cervantes & Mia Mellem
 * 
 * Snake Game World object belonging to the model
 */
namespace SnakeGame
{
    /// <summary>
    /// This class makes a World object
    /// </summary>
    public class World
    {
        public Dictionary<long, Snake> Players;
        public Dictionary<long, Power> Powerups;
        public Dictionary<long, Wall> Walls;
        public long SnakeID { get; set; }
        public bool FirstSnake { get; set; }
        public int Size { get; set; }

        /// <summary>
        /// Constructs a world object. 
        /// </summary>
        /// <param name="_size">Size of the world</param>
        public World(int _size)
        {
            Players = new Dictionary<long, Snake>();
            Powerups = new Dictionary<long, Power>();
            Walls = new Dictionary<long, Wall>();
            SnakeID = -1;
            Size = _size;
            FirstSnake = true;
        }
       
    }
}
