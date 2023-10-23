## Bullets

We use differents types of bullets in the game. Each type of bullet has its own characteristics.
Bullets are defined in the `bullets.json` file.

### Json structure

We use the following structure to define bullets:

```json
{
    "bullets": [
        {
            "id": "classic",
            "velocity": {
                "speedX": 120,
                "speedY": 0
            },
            "spritePath": "assets/R-TypeSheet/WaterBullets.png",
            "soundPath": "assets/Audio/Sounds/laser.ogg",
        },
        {
            "id": "perforant",
            "velocity": {
                "speedX": 400,
                "speedY": 0
            },
            "spritePath": "assets/R-TypeSheet/FireBullets.png",
            "soundPath": "assets/Audio/Sounds/laser2.ogg",
        }
    ]
}
```

It is composed of an array of bullets. Each bullet has its own id and its own characteristics.

### Characteristics

- `id`: The id of the bullet. It is used to identify the bullet in the game.
- `damage`: The damage of the bullet. It is used to damage the enemies.
- `health`: The health of the bullet. It is used to destroy the bullet when it collides with an enemy.
- `velocity`: The velocity of the bullet. It is used to move the bullet.
- `physics`: The physics of the bullet. It is used to move the bullet.
- `waitTimeBullet`: The wait time of the bullet. It is used to wait before shooting again.
- `collisionRect`: The collision rectangle of the bullet. It is used to detect collisions with enemies.
- `spritePath`: The sprite path of the bullet. It is used to display the bullet.
- `soundPath`: The sound path of the bullet. It is used to play the sound of the bullet.
- `spriteRect`: The sprite rectangle of the bullet. It is used to display the bullet.
- `animRect`: The animation rectangle of the bullet. It is used to display the bullet.


### Physics

Physics is represented by an array of strings. Each string represents a physics. You can put as many physics as you want. So far, we have the following physics:
- `zigzag`: The bullet moves in a zigzag way.
- `boncing`: The bullet bounces on the top and bottom of the screen.


```json
{
    "bullets": [
        {
            "id": "perforant",
            "physics": [
                "zigzag"
            ]
        }
    ]
}
```

### Bullet types

For now, we have 4 types of bullets:
- `classic`: The classic bullet.
- `fast`: The fast bullet.
- `bounce`: The bouncing bullet.
- `perforant`: The perforant bullet.

To add a new bullet type, you have to add it in the `missileTypes_e` enum in the `Missile.hpp` file. Then, you have to add it in the `bulletKeyMap` map in the `EventsSystems.cpp` file.
