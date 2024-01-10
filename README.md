# Raycaster Engine
This projects tries to reimplement the 2.5D Graphics technique in games like Wolfenstein. These class of games us a 2D grid combined with raycasting to draw cubes where the wall height is related to the distance to the player. 

## Current Features:
- Textured walls and floors using tile-map
- Multi-textured walls depending on side
- Sprites
    - Transparent Sprites
    - Directional Sprites for a "3D" effect
    - Simple Animations
- Doors (Thin Wall)
- Keyboard and Mouse Controls
- Looking up and down
- Transparent Walls & Doors

## Future Features:
- Better way to design and load map data
- Proper Fog (Might mix with lighting in some way)
    - If we know the fog will block walls/sprites, we can simply not render them as well
- Multiple Levels (requiring multiple raycasting passes for each)

### Smaller Updates:
- Render the sides of transparent walls, or add option too
- Use simple shading for walls based on the side
- Fix mouse control
- Add player height control option

### Possible Optimizations
 - Build the SortStructure array while raycasting/transforming sprites to avoid having to reread all that memory again
    - Must handle pointers to realloc-ed data that are now dead. Use index instead
 - Build a render_item struct hold only the data needed to render (texCol, perpDist, column, Texture*)
    - We can then eliminate the multi-wall and door_texcol calculations from the render loop
 - move from AOS to SOA
 - create an arena to pack booleans instead of using uint8_t which wastes 7 bits

### Other possible ideas:
- Vertically Opening doors (could just use tranparent texture animations)
- Lighting System
- Floors/Ceiling slabs, depth
- Moving Walls
    - Horizontal requires new non-cubic wall intersection check
    - Vertical walls require rendering ceiling slabs so the bottom of the wall is solid
- Mirrors/Portals (raycasting aspect is fine but to render the floor and sprites from the portals POV requires redoing a modified render pass for each portal)
