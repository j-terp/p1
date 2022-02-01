# wastemanagment-pathfinding

## Usage

Compile with `mingw-make.exe`, and execute the `program.exe` file.

## Modules

The program is divided up into modules, where each module is combined to compile the program.

### ClusterModule

The Clustering module is for creating clusters for a list of elements, such that the program can divide up the list into smaller lists.

### ConvertGraph

The Converting Graph module transforms an asymmetric matrix into one that is symmetric.

### CSVModule

The CSV module takes input in form of `.csv` files, and will output in the same file format into `./Output/{subfolder}/`

### FilterModule

The Filter module filters matrices and lists using other lists.

### LinkedLists

The Linked List module is for using linked lists easier in other modules. The Cluster Module is dependent on this.

### MatrixModule

The Matrix module is for operations involving matrices, such as allocating and transposing.

### TSP

The TSP module calculates the shortest path using a list of points.

## License

This project is under MIT License