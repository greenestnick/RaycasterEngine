# Raycaster Engine
This projects tries to reimplement the 2.5D Graphics technique in games like Wolfenstein. These class of games us a 2D grid combined with raycasting to draw cubes where the wall height is related to the distance to the player. 

## Current Features:
- Basic Raycasting Map
- Textured walls and floors by tile
- Multi-textured walls depending on side
- Sprites
- Transparent Sprites
- Doors (Thin Wall)
- Keyboard and Mouse Controls
- Looking up and down
- Transparent Walls & Doors
- Rending Muliple Levels

## Future Features:
- "3D" sprites
- Proper Animation
- Proper Texture Loading,Access System
- Proper Map System
- Better way to design and load map data
- Proper Fog (Might mix with lighting in some way)
    - If we know the fog will block walls/sprites, we can simply not render them as well

### Smaller Updates:
- Render the sides of transparent walls, or add option too
- Use simple shading for walls based on the side
- Fix mouse control (mostly horizontal direction)
    - One fix is to have the mouse move around the screen as if the screen was torus
- Add player height control option
- Appending many columns to the list is quite slow (perhaps passing a rayhit by copy is just too expensive)

### Other possible ideas:
- Vertically Opening doors (could just use tranparent texture animations)
- Lighting System
- Floors/Ceiling slabs, depth
- Moving Walls
    - Horizontal requires new non-cubic wall intersection check
    - Vertical walls require rendering ceiling slabs so the bottom of the wall is solid
- Mirrors/Portals (raycasting aspect is fine but to render the floor and sprites from the portals POV requires redoing a modified render pass for each portal)
