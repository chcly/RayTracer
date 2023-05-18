# Viewer 

Is a program that allows rendering a simple scene.  

## Invoking

```
Usage: Viewer.exe <options>

  <options>:

    -h, --help    Display this help message.
    -r, --backend Specify the backend renderer.
                   - Where the value is one of the following values:
                     - CPU Backend: 0
                     - GPU Backend: 1

    -o, --output  Specify an output file.
                   - Where the value is a file-system pathname.

```


## Controls

If the primary camera in the scene is set to interactive then the following input is valid.


| Input                     | Description                                                     |
|:--------------------------|:----------------------------------------------------------------|
| Left Mouse + Mouse Motion | Pivots the camera around a fixed point in front of it.          |
| Ctrl + Left + Mouse(y)    | Moves the camera along a vector between it and the fixed point. |
| Shift + Left + Mouse      | Moves the fixed point relative to the camera.                   |
| Wheel +-                  | Moves the camera along vector between it and the fixed point.   |
| C-Key                     | Centers the fixed point at the origin.                          |
| Num-pad 1-Key             | Orients the camera looking toward the +y axis                   |
| Num-pad 3-Key             | Orients the camera looking toward the -x axis                   |
| Num-pad 7-Key             | Orients the camera looking toward the -z axis                   |


## Render Flags 

The render flags can be used to control how the scene is rendered.

| Flag | Description                    |
|:-----|:-------------------------------|
| 1    | Render light and materials.    |
| 2    | Render the ray's hit distance. |
| 4    | Render the hit normal.         |
| 8    | Render edges.                  |
| 16   | Invert the color.              |

### Setting the Flags

| Keyboard Input | Description  |
|:---------------|:-------------|
| 1-key          | Sets flag 1  |
| 2-key          | Sets flag 2  |
| 3-key          | Sets flag 4  |
| 4-key          | Sets flag 8  |
| 5-key          | Sets flag 16 |

Holding `Shift` while pressing one of the mode keys will toggle them together.

