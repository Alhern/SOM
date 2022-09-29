# SOM

This implementation of the SOM (Self-Organizing Map) algorithm in C is based on the paper "Self-Organizing Maps" by Teuvo Kohonen.

SOM is an unsupervised and competitive neural network algorithm used to cluster data. It's based on the idea of self-organizing systems, the algorithm is able to automatically learn the structure of the data and cluster data points that are close together in the data space.

This program uses Ronald Aylmer Fisher's famous Iris dataset, you can find it in the `data` folder.
The dataset contains 150 samples (50 for each class) of 3 different species of Iris flowers (Iris setosa, Iris virginica and Iris versicolor), with 4 features each: 
- sepal length in cm, 
- sepal width in cm, 
- petal length in cm,
- petal width in cm.


## Usage
The program's configuration is done in the `tools.h` file.

Compile with `make` and run with `./som`

Use `make clean` to remove the executable and -o files.

## Results

Versicolor and Virginica can overlap but Setosa is clearly separated from the other two.

```
[1] = Setosa
[2] = Versicolor
[3] = Virginica

 [2]  [2]  [1]  [1]  [1]  [1]  [1]
 [2]  [2]  [1]  [1]  [1]  [1]  [1]
 [2]  [2]  [2]  [1]  [1]  [1]  [1]
 [2]  [2]  [2]  [2]  [1]  [1]  [1]
 [3]  [3]  [2]  [2]  [2]  [2]  [2]
 [3]  [3]  [2]  [2]  [2]  [2]  [2]
 [3]  [3]  [3]  [2]  [2]  [2]  [2]
 [3]  [3]  [3]  [3]  [2]  [2]  [2]
 [3]  [3]  [3]  [3]  [3]  [2]  [2]
 [3]  [3]  [3]  [3]  [3]  [2]  [2]