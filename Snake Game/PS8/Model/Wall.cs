/*
 * Authors: Brandy Cervantes & Mia Mellem
 * 
 * Snake Game Wall object representing a wall
 */
using System.Runtime.Serialization;
using System.Text.Json.Serialization;

namespace SnakeGame
{
    /// <summary>
    /// This class makes a wall object
    /// </summary>
    [DataContract(Namespace = "")]
    public class Wall
    {
        [JsonPropertyName("wall")]
        [DataMember(Name = "ID")]
        public int wall { get; set; }

        [DataMember(Name = "p1")]
        public Vector2D p1 { get; set; }

        [DataMember(Name = "p2")]
        public Vector2D p2 { get; set; }

        /// <summary>
        /// Default wall constructor for JSON
        /// </summary>
        [JsonConstructor]
        public Wall() 
        { 
            wall = 0;
            p1 = new Vector2D();
            p2 = new Vector2D();    
        }

        /// <summary>
        /// Constructs a wall using the provided ID and provided 
        /// coordinates.
        /// </summary>
        /// <param name="id">The wall object's ID</param>
        /// <param name="p1_x">X coordinate of the first point</param>
        /// <param name="p1_y">y coordinate of the first point</param>
        /// <param name="p2_x">x coordinate of the second point</param>
        /// <param name="p2_y">y coordinate of the second point</param>
        public Wall(int id, int p1_x, int p1_y, int p2_x, int p2_y) 
        {
            wall = id;
            p1 = new Vector2D(p1_x, p1_y);
            p2 = new Vector2D(p2_x, p2_y);
        }
    }
}
