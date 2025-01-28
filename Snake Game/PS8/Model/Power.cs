/*
 * Authors: Brandy Cervantes & Mia Mellem
 * 
 * Snake Game PowerUp object representing a powerup
 */
using System.Runtime.Serialization;
using System.Text.Json.Serialization;

namespace SnakeGame
{
    /// <summary>
    /// This class creates a powerup object
    /// </summary>
    [DataContract(Namespace = "")]
    public class Power
    {
        // ID
        [DataMember]
        public int power { get; set; } = 0;
        [DataMember]
        public Vector2D loc { get; set; }
        [DataMember]
        public bool died { get; set; }

        [JsonConstructor]
        public Power()
        {
            loc = new Vector2D();
        }

        /// <summary>
        /// This method makes a powerup object
        /// </summary>
        /// <param name="x">x coordinate of the position of the powerup</param>
        /// <param name="y">y coordinate of the position of the powerup</param>
        public Power(int x, int y)
        {
            loc = new Vector2D(x, y);
            died = false;
            power++;
        }
    }
}

