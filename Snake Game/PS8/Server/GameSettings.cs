using SnakeGame;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;

namespace SnakeGame
{
    [DataContract(Namespace = "")]
    public class GameSettings
    {
        [DataMember]
        public int MSPerFrame;

        [DataMember]
        public int RespawnRate;

        [DataMember]
        public int UniverseSize;

        [DataMember]
        public List<Wall> Walls;


        private GameSettings()
        {
            Walls = new List<Wall>();
        }

        /// <summary>
        /// Create a new GameSettings
        /// </summary>
        public GameSettings(int msPerFrame, int respawnRate, int universeSize, List<Wall> walls)
        {
            MSPerFrame = msPerFrame;
            RespawnRate = respawnRate;
            UniverseSize = universeSize;
            Walls = walls;
        }
    }
}
