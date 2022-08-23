# opssat-whitebalance
White balance binary for ESA OPS-SAT mission.

## How to build:
1. Clone repo
2. `mkdir bin`
3. `cmake ../`
4. `cmake --build .`

## Options
```
wb [options] ...
    --input    / -i     the file path of the input image
    --output   / -o     the file path of the output image. default: input with .wb appended. handles image conversions.
    --thresh   / -t     change histogram thresh. Default 0.005 (0.5\%)
    --help     / -?     this information
```
