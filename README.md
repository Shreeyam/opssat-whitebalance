# opssat-whitebalance
White balance binary for ESA OPS-SAT mission. Based on GIMP white balancing algorithm
https://docs.gimp.org/2.10/en/gimp-layer-white-balance.html

Used in Kacker, S., Meredith, A., Cahoy, K., & Labrèche, G. (2022). Machine Learning Image Processing Algorithms Onboard OPS-SAT.
https://digitalcommons.usu.edu/smallsat/2022/all2022/65/ 
```bibtex
@article{kacker2022machine,
  title={Machine Learning Image Processing Algorithms Onboard OPS-SAT},
  author={Kacker, Shreeyam and Meredith, Alex and Cahoy, Kerri and Labr{\`e}che, Georges},
  year={2022}
}
```

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
